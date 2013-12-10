/// @filename debug.h
/// @brief Tests common debug include file
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

#ifndef OPENMINI_TESTS_DEBUG_H_
#define OPENMINI_TESTS_DEBUG_H_

#include <fstream>
#include <string>

#include "openmini/tests/tests.h"

/// @brief Namespace holding debug stuff - nothing using it should be committed
namespace debug {

/// @brief Helper class for writing signal data into files
///
/// This is clearly not optimized at all nor really clean,
/// for debugging purpose only!
class FileWriter {
 public:
  explicit FileWriter(const std::string& filename)
    : out_stream_(filename) {
    // Nothing to do here for now
  }
  ~FileWriter() {
    out_stream_.close();
  }

  /// @brief Push one sample into file writer, will be unfolded if
  /// wrapping a vectorized type
  void Push(const Sample sample) {
    // Using the slower runtime version of GetByIndex() for genericity's sake
    for (unsigned int index(0); index < openmini::SampleSize; index += 1) {
      out_stream_ << GetByIndex(sample, index) << "\n";
    }
  }

  /// @brief Push version for one actual value
  void PushFloat(const float sample) {
    out_stream_ << sample << "\n";
  }

 private:
  std::ofstream out_stream_;
};

}  // namespace debug

#endif  // OPENMINI_TESTS_DEBUGS_H_
