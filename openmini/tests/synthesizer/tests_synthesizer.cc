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
TEST(Synthesizer, SynthesizerNoteOnNoteOff) {
  // Random, weird block size
  const unsigned int kBlockSize(FindImmediateNextMultiple(
    GeneratorRangedInteger(32, 1024)(),
    4));
  std::vector<float> block(kBlockSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  float mean_square(0.0f);
  // Generating a signal during half the complete time
  synth.NoteOn(kMinKeyNote);
  while (sample_idx < kDataTestSetSize / 2) {
    synth.ProcessAudio(&block[0], block.size());
    sample_idx += kBlockSize;
    for (unsigned int i(0); i < kBlockSize; ++i) {
      mean_square += block[i] * block[i];
    }
  }
  // The generated signal mean square is supposed not to be null
  ASSERT_LT(0.0f, mean_square * 2.0f / kDataTestSetSize);

  mean_square = 0.0f;
  // Generating a silence during all the remaining time
  synth.NoteOff(kMinKeyNote);
  while (sample_idx < kDataTestSetSize) {
    synth.ProcessAudio(&block[0], block.size());
    sample_idx += kBlockSize;
    for (unsigned int i(0); i < kBlockSize; ++i) {
      mean_square += block[i] * block[i];
    }
  }
  // The generated signal mean square is supposed to be null
  ASSERT_FLOAT_EQ(0.0f, mean_square * 2.0f / kDataTestSetSize);
}

/// @brief Update the synthesizer twice in a row
/// The generated sound should stay unchanged
TEST(Synthesizer, SynthesizerReupdate) {
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

  // Check for clicks
  const float kEpsilon(1.06f);
  EXPECT_FALSE(ClickWasFound(&data[0], data.size(), kEpsilon));
}

/// @brief Asking the synthesizer for small block size
/// (not being a multiple of a sample size)
/// The generated sound should stay OK
TEST(Synthesizer, SynthesizerSmallBlockSize) {
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

  // Check for clicks
  const float kEpsilon(1.06f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Asking the synthesizer for various block size over time
/// The generated sound should stay OK
TEST(Synthesizer, SynthesizerVaryingBlockSize) {
  std::vector<float> data(kDataTestSetSize);
  Synthesizer synth;

  synth.NoteOn(kMinKeyNote);

  unsigned int data_idx(0);
  while (data_idx < data.size()) {
    const unsigned int kBlockSize(GeneratorRangedInteger(1,
                                    data.size() - data_idx)());
    synth.ProcessAudio(&data[data_idx], kBlockSize);
    data_idx += kBlockSize;
  }

  // Check for clicks
  const float kEpsilon(1.06f);
  // Not testing the first sample!
  EXPECT_FALSE(ClickWasFound(&data[1], data.size() - 1, kEpsilon));
}

/// @brief Process a fixed amount of data without changing anything
/// to default parameters, only setting a note on
TEST(Synthesizer, SynthesizerPerf) {
  std::vector<float> data(openmini::kBlockSize);
  Synthesizer synth;

  unsigned int sample_idx(0);
  synth.NoteOn(kMinKeyNote);

  while (sample_idx < kSynthesizerPerfSetSize) {
    // not storing everything, only creating an "history"
    synth.ProcessAudio(&data[0], openmini::kBlockSize);
    sample_idx += openmini::kBlockSize;
  }

  // No actual test!
  EXPECT_TRUE(true);
}
