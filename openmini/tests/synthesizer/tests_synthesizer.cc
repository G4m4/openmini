/// @filename tests_synthesizer.cc
/// @brief Synthesizer specific tests
/// @author gm
/// @copyright gm 2013
///
/// This file is part of OpenMini
///
/// OpenMini is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenMini is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.

#include "openmini/tests/tests.h"

#include "openmini/src/synthesizer/parameters.h"
#include "openmini/src/synthesizer/synthesizer.h"

// Using declarations for tested class
using openmini::synthesizer::Synthesizer;

/// @brief Play sound, check that something is generated,
/// stop sound and check that we actually gets zeroes
///
/// Using default values everywhere (sampling rate, block size etc.)
/// Except of course for the decay timing (set to 0)
TEST(Synthesizer, NoteOnNoteOff) {
  // Random, weird block size
  const unsigned int kBlockSize(
    std::uniform_int_distribution<int>(32, 4096)(kRandomGenerator));
  std::vector<float> block(kBlockSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  float mean_square(0.0f);
  // We need to set the decay to zero if we want instant shutoff
  synth.SetValue(openmini::synthesizer::Parameters::kDecayTime, 0.0f);
  // Generating a signal during half the complete time
  synth.NoteOn(kMinKeyNote);
  while (sample_idx < kDataTestSetSize / 2) {
    synth.ProcessAudio(&block[0], block.size());
    sample_idx += kBlockSize;
    for (unsigned int i(0); i < block.size(); ++i) {
      mean_square += block[i] * block[i];
    }
  }
  // The generated signal mean square is supposed not to be null
  EXPECT_LT(0.0f, mean_square * 2.0f / kDataTestSetSize);

  mean_square = 0.0f;
  // Generating a silence during all the remaining time
  synth.NoteOff(kMinKeyNote);
  while (sample_idx < kDataTestSetSize) {
    synth.ProcessAudio(&block[0], block.size());
    sample_idx += kBlockSize;
    for (unsigned int i(0); i < block.size(); ++i) {
      mean_square += block[i] * block[i];
    }
  }
  // The generated signal mean square is supposed to be null
  // ...Or almost null, since we now have filters in our synthesizer
  const float kEpsilon(1e-4f);
  EXPECT_NEAR(0.0f, mean_square * 2.0f / kDataTestSetSize, kEpsilon);
}

/// @brief Update the synthesizer twice in a row
/// The generated sound should stay unchanged
TEST(Synthesizer, Reupdate) {
  std::vector<float> data(openmini::kBlockSize * 2);
  Synthesizer synth;

  unsigned int sample_idx(0);
  synth.NoteOn(kMinKeyNote);

  synth.SetValue(openmini::synthesizer::Parameters::kOsc1Waveform, 0.0f);
  while (sample_idx < kDataTestSetSize) {
    // not storing everything, only creating an "history"
    synth.ProcessAudio(&data[0], openmini::kBlockSize);
    sample_idx += openmini::kBlockSize;
  }
  synth.SetValue(openmini::synthesizer::Parameters::kOsc1Waveform, 0.0f);
  // Filling the remaining half
  synth.ProcessAudio(&data[openmini::kBlockSize], openmini::kBlockSize);

  // Check for clicks - this value is a bit higher (although still quite low)
  // due to the filter ringing effect
  const float kEpsilon(10.0f);
  EXPECT_FALSE(ClickWasFound(&data[0], data.size(), kEpsilon));
}

/// @brief Asking the synthesizer for small block size
/// (not being a multiple of a sample size)
/// The generated sound should stay OK
TEST(Synthesizer, SmallBlockSize) {
  const unsigned int kBlockSize(openmini::SampleSize * 2 - 1);
  const unsigned int kDataSize(FindImmediateNextMultiple(kDataTestSetSize,
                                                         kBlockSize));
  std::vector<float> data(kDataSize);
  Synthesizer synth;

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    synth.ProcessAudio(&data[data_idx], kBlockSize);
    data_idx += kBlockSize;
  }

  // Check for clicks - this value is a bit higher (although still quite low)
  // due to the filter ringing effect
  const float kEpsilon(10.0f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Asking the synthesizer for various block size over time
/// The generated sound should stay OK
TEST(Synthesizer, VaryingBlockSize) {
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    const unsigned int kBlockSize(std::uniform_int_distribution<unsigned int>(1,
                                    data.size() - data_idx)(kRandomGenerator));
    synth.ProcessAudio(&data[data_idx], kBlockSize);
    data_idx += kBlockSize;
  }

  // Check for clicks - this value is a bit higher (although still quite low)
  // due to the filter ringing effect
  const float kEpsilon(10.0f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Asking the synthesizer for the smallest possible block size
/// The generated sound should stay OK
TEST(Synthesizer, SmallestBlockSize) {
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;
  const unsigned int kMinBlockSize(1);

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    synth.ProcessAudio(&data[data_idx], kMinBlockSize);
    data_idx += kMinBlockSize;
  }

  // Check for clicks - this value is a bit higher (although still quite low)
  // due to the filter ringing effect
  const float kEpsilon(10.0f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Asking the synthesizer for various output sampling rates over time.
/// The generated sound should stay OK
TEST(Synthesizer, VaryingSamplingRate) {
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;
  const unsigned int kBlockSize(1024);
  const float kMinSamplingRate(10.0f);
  const float kMaxSamplingRate(SamplingRate::Instance().Get());

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    const float kSamplingRate(std::uniform_real_distribution<float>(
                                kMinSamplingRate, kMaxSamplingRate)
                              (kRandomGenerator));
    synth.SetOutputSamplingFrequency(kSamplingRate);
    synth.ProcessAudio(&data[data_idx], kBlockSize);
    data_idx += kBlockSize;
  }

  // Check for clicks - this value is a bit higher (although still quite low)
  // - due to the filter ringing effect
  // - due to the modulator amplitude changes
  const float kEpsilon(35.0f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Asking the synthesizer for various block size and various sampling
/// frequencies over time - the generated sound should stay OK
TEST(Synthesizer, VaryingOutputFormat) {
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;
  const float kMinSamplingRate(10.0f);
  const float kMaxSamplingRate(SamplingRate::Instance().Get());

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    const unsigned int kBlockSize(std::uniform_int_distribution<unsigned int>(1,
                                    data.size() - data_idx)(kRandomGenerator));
    const float kSamplingRate(std::uniform_real_distribution<float>(
                                kMinSamplingRate, kMaxSamplingRate)
                              (kRandomGenerator));
    synth.SetOutputSamplingFrequency(kSamplingRate);
    synth.ProcessAudio(&data[data_idx], kBlockSize);
    data_idx += kBlockSize;
  }

  // Check for clicks - this value is a bit higher (although still quite low)
  // due to the filter ringing effect
  const float kEpsilon(25.0f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Process a fixed amount of data without changing anything
/// to default parameters, only setting a note on - at 96kHz
TEST(Synthesizer, Perf96k) {
  const float kOutFrequency(96000.0f);
  std::vector<float> data(openmini::kBlockSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  synth.NoteOn(kMinKeyNote);
  synth.SetOutputSamplingFrequency(kOutFrequency);

  while (sample_idx < static_cast<unsigned int>(kSynthesizerPerfSetLength
                                                * kOutFrequency)) {
    // not storing everything, only creating an "history"
    synth.ProcessAudio(&data[0], openmini::kBlockSize);
    sample_idx += openmini::kBlockSize;
  }

  // No actual test!
  EXPECT_TRUE(true);
}

/// @brief Process a fixed amount of data without changing anything
/// to default parameters, only setting a note on - at 48kHz
TEST(Synthesizer, Perf48k) {
  const float kOutFrequency(48000.0f);
  std::vector<float> data(openmini::kBlockSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  synth.NoteOn(kMinKeyNote);
  synth.SetOutputSamplingFrequency(kOutFrequency);

  while (sample_idx < static_cast<unsigned int>(kSynthesizerPerfSetLength
                                                * kOutFrequency)) {
    // not storing everything, only creating an "history"
    synth.ProcessAudio(&data[0], openmini::kBlockSize);
    sample_idx += openmini::kBlockSize;
  }

  // No actual test!
  EXPECT_TRUE(true);
}

/// @brief Process a fixed amount of data without changing anything
/// to default parameters, only setting a note on - at 48kHz
TEST(Synthesizer, Quality44k1) {
  const float kOutFrequency(44100.0f);
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  synth.NoteOn(kMaxKeyNote);
  synth.SetOutputSamplingFrequency(kOutFrequency);

  while (sample_idx < kDataTestSetSize) {
    synth.ProcessAudio(&data[sample_idx], openmini::kBlockSize);
    sample_idx += openmini::kBlockSize;
  }
  const float kEpsilon(5.0f);
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

// TODO(gm): Activate this test
/// @brief With any parameters value the output must stay within [-1.0, 1.0]
///
/// Using random but fixed parameters for output stream
//TEST(Synthesizer, OutputRange) {
//  // Random block size
//  const unsigned int kBlockSize(
//    std::uniform_int_distribution<int>(32, 4096)(kRandomGenerator));
//  std::vector<float> block(kBlockSize);
//  // Random sampling frequency
//  const float kSamplingFrequency(
//    std::uniform_real_distribution<float>(8000, SamplingRate::Instance().Get())
//      (kRandomGenerator));
//
//  // High limiter threshold
//  const float kLimiterThreshold(2.0f);
//
//  Synthesizer synth(kLimiterThreshold);
//  synth.SetOutputSamplingFrequency(kSamplingFrequency);
//
//  unsigned int sample_idx(0);
//  synth.NoteOn(kMinKeyNote);
//  while (sample_idx < kDataTestSetSize) {
//    // Random parameters value
//    for (unsigned int param_id(0);
//         param_id < openmini::synthesizer::Parameters::kCount;
//         ++param_id) {
//      // For now oscillators waveform changing is not available while processing
//      // TODO(gm): debug it (See #65)
//      if ((param_id != openmini::synthesizer::Parameters::kOsc1Waveform)
//          && (param_id != openmini::synthesizer::Parameters::kOsc2Waveform)
//          && (param_id != openmini::synthesizer::Parameters::kOsc3Waveform)) {
//        synth.SetValue(param_id, kNormPosDistribution(kRandomGenerator));
//      }
//    }
//    synth.ProcessAudio(&block[0], block.size());
//    for (unsigned int i(0); i < block.size(); ++i) {
//      EXPECT_GE(1.0f, block[i]);
//      EXPECT_LE(-1.0f, block[i]);
//    }
//    sample_idx += kBlockSize;
//  }
//}
