/// @filename factory.cc
/// @brief Filters factory - implementation
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

#include "openmini/src/filters/factory.h"

#include "openmini/src/common.h"
#include "openmini/src/filters/filter_base.h"
#include "openmini/src/filters/secondorder_raw.h"

namespace openmini {
namespace filters {

Filter_Base* CreateFilter(void) {
  void* ptr(Allocate(sizeof(SecondOrderRaw)));
  ASSERT(ptr != nullptr);
  return new (ptr) SecondOrderRaw();
}

void DestroyFilter(Filter_Base* filter) {
  ASSERT(filter != nullptr);

  // Beware, this is not safe! (explicit call to base destructor
  // -> possible leaks in the child!)
  // We should use smart pointers anyway
  filter->~Filter_Base();
  Deallocate(filter);
}

}  // namespace filters
}  // namespace openmini
