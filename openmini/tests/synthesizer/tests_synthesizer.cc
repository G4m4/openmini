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

#include "openmini/src/synthesizer/synthesizer.h"

// Using declarations for tested class
using openmini::synthesizer::Synthesizer;

/// @brief Play sound, check that something is generated,
/// stop sound and check that we actually gets zeroes
///
/// Using default values everywhere (sampling rate, block size etc.)
TEST(Synthesizer, SynthesizerNoteOnNoteOff) {
  // Random, weird block size
  const unsigned int kBlockSize(GeneratorRangedInteger(32, 1024)());
  std::vector<float> block(kBlockSize);
  Synthesizer synth;

  int sample_idx(0);
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

