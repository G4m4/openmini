/// @filename WidgetsManager.h
/// @brief OpenMini widgets manager
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


// This file is NOT generated by Juce (at least not after the first time),
// That's why we apply our coding style here


#ifndef OPENMINI_PLUGIN_COMMON_WIDGETSMANAGER_H_
#define OPENMINI_PLUGIN_COMMON_WIDGETSMANAGER_H_

#include <array>

#include "JuceHeader.h"

#include "openmini/src/synthesizer/parameter_meta.h"
#include "openmini/src/synthesizer/parameters.h"

#include "openmini/implementation/common/Widget.h"

// Using declarations for OpenMini parameters
using openmini::synthesizer::ParameterMeta;
// Using directives for OpenMini parameters
using namespace openmini::synthesizer;

class OpenMiniAudioProcessorEditor;

/// @brief Widgets manager
///
/// This component manages all widgets based on a plugin actual parameters.
/// Hence it bridges the gap between OpenMini specific parameters management,
/// and implementation details such as links with UI etc.
class WidgetsManager : public juce::Component,
                       public juce::ChangeListener,
                       public juce::Slider::Listener {
 public:
  explicit WidgetsManager(
    const std::array<ParameterMeta, Parameters::kCount>& params);
  ~WidgetsManager();

  void paint(juce::Graphics& g);

  // Overrides from inherited classes
  void sliderValueChanged(juce::Slider* slider);
  void sliderDragStarted(juce::Slider* slider);
  void sliderDragEnded(juce::Slider* slider);
  void changeListenerCallback(juce::ChangeBroadcaster *source);

 private:
  /// @brief Helper for retrieving a paramter index from a UI widget
  int getParamId(const juce::Slider* slider) const;
  /// @brief Helper for retrieving plugin editor
  OpenMiniAudioProcessorEditor& getEditor(void) const;

  juce::OwnedArray<Widget> widgets_;

  /// @brief Common widget width
  static const int kWidgetWidth = 60;
  /// @brief Common widget height
  static const int kWidgetHeight = 60;
  /// @brief Margin (vertical & horizontal) between widgets
  static const int kWidgetMargin = 10;
  /// @brief Height of each widgets column
  static const int kColumnHeight = 250;
  /// @brief Width of each widgets column
  static const int kColumnWidth = 200;
};

#endif  // OPENMINI_PLUGIN_COMMON_WIDGETSMANAGER_H_
