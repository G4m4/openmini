/// @filename parameters_manager.cc
/// @brief Parameters Manager implementation
/// interface between them and the user
///
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

// std::min, std::fill
#include <algorithm>

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/parameter_manager.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

float StoredToNormalized(const float stored_value,
                         const ParameterMeta& metadata) {
  const float ratio(metadata.max() - metadata.min());
  const float normalized((stored_value - metadata.min()) / ratio);
  ASSERT(normalized >= 0.0f);
  ASSERT(normalized <= 1.0f);

  return normalized;
}

float NormalizedToStored(const float normalized,
                         const ParameterMeta& metadata) {
  ASSERT(normalized >= 0.0f);
  ASSERT(normalized <= 1.0f);

  const float ratio(metadata.max() - metadata.min());
  return normalized * ratio + metadata.min();
}

int NormalizedToInt(const float normalized,
                    const ParameterMeta& metadata) {
  ASSERT(normalized >= 0.0f);
  ASSERT(normalized <= 1.0f);

  return std::min(metadata.cardinality() - 1,
                  FloorAndConvert<int>(normalized * metadata.cardinality()));
}

int UnnormalizedToInt(const float unnormalized) {
  return FloorAndConvert<int>(unnormalized);
}

ParametersManager::ParametersManager()
    : updated_parameters_(),
      metadatas_(Parameters::kParametersMeta) {
  AssignDefault();
}

ParametersManager::~ParametersManager() {
  // Nothing to do here for now
}

void ParametersManager::SetValue(const int parameter_id, const float value) {
  ASSERT(parameter_id >= 0);
  ASSERT(parameter_id < static_cast<int>(values_.size()));

  const ParameterMeta& metadata(GetMetadata(parameter_id));
  // If the parameter is normalized, then we have to pass through normalization
  if (metadata.is_normalized()) {
    values_[parameter_id] = NormalizedToStored(value, metadata);
  } else {
    values_[parameter_id] = value;
  }
  updated_parameters_.insert(parameter_id);
}

float ParametersManager::GetValue(const int parameter_id) const {
  ASSERT(parameter_id >= 0);
  ASSERT(parameter_id < static_cast<int>(values_.size()));

  const float value(values_[parameter_id]);
  const ParameterMeta& metadata(GetMetadata(parameter_id));
  // If the parameter is normalized, then we have to pass through normalization
  if (metadata.is_normalized()) {
    return StoredToNormalized(value, metadata);
  } else {
    return value;
  }
}

const ParameterMeta& ParametersManager::GetMetadata(
    const int parameter_id) const {
  ASSERT(parameter_id < static_cast<int>(values_.size()));
  return metadatas_[parameter_id];
}

unsigned int ParametersManager::ParametersCount(void) const {
  return static_cast<unsigned int>(values_.size());
}

bool ParametersManager::ParametersChanged(void) {
  return !(updated_parameters_.empty());
}

void ParametersManager::ParametersProcessed(void) {
  updated_parameters_.clear();
}

void ParametersManager::AssignDefault(void) {
  for (unsigned int i(0); i < values_.size(); ++i) {
    SetValue(i, GetMetadata(i).default_value());
  }
}

ParametersManager::UpdatedParametersIterator::UpdatedParametersIterator(
  const ParametersManager& manager)
    : manager_(manager),
      iterator_(manager.updated_parameters_.begin()) {
}

bool ParametersManager::UpdatedParametersIterator::Next() {
  ++iterator_;
  if (iterator_ != manager_.updated_parameters_.end()) {
    return true;
  } else {
    return false;
  }
}

int ParametersManager::UpdatedParametersIterator::GetID(void) const {
  return *iterator_;
}

}  // namespace synthesizer
}  // namespace openmini
