/// @filename PluginProcessor.h
/// @brief OpenMini plugin processor (audio)
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

#ifndef OPENMINI_PLUGIN_COMMON_PLUGINPROCESSOR_H_
#define OPENMINI_PLUGIN_COMMON_PLUGINPROCESSOR_H_

#include "JuceHeader.h"
#include "openmini/src/synthesizer/synthesizer.h"

/// @brief Plugin "processor" class
///
/// Contains all audio, presets and Midi stuff
class OpenMiniAudioProcessor  : public juce::AudioProcessor,
                                public juce::ChangeBroadcaster {
 public:
  OpenMiniAudioProcessor();
  ~OpenMiniAudioProcessor();

  void prepareToPlay(double sampleRate, int samplesPerBlock);
  void releaseResources();

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

  AudioProcessorEditor* createEditor();
  bool hasEditor() const;

  const String getName() const;

  int getNumParameters();

  float getParameter(int index);
  void setParameter(int index, float newValue);

  const String getParameterName(int index);
  const String getParameterText(int index);

  const String getInputChannelName(int channelIndex) const;
  const String getOutputChannelName(int channelIndex) const;
  bool isInputChannelStereoPair(int index) const;
  bool isOutputChannelStereoPair(int index) const;

  bool acceptsMidi() const;
  bool producesMidi() const;
  bool silenceInProducesSilenceOut() const;
  double getTailLengthSeconds() const;

  int getNumPrograms();
  int getCurrentProgram();
  void setCurrentProgram(int index);
  const String getProgramName(int index);
  void changeProgramName(int index, const String& newName);

  void getStateInformation(MemoryBlock& destData);
  void setStateInformation(const void* data, int sizeInBytes);

  // Overrides from inherited classes
  void addChangeListener(juce::ChangeListener* listener);

  // OpenMini-specifics
  void triggerNoteOn(const int midi_note);
  void triggerNoteOff(const int midi_note);

  // DEBUG
  double GetLastProcessTime() const;
  //  /DEBUG

  juce::MidiKeyboardState keyboard_state_;

 private:
  openmini::synthesizer::Synthesizer synth_;
  double process_time_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenMiniAudioProcessor)
};

#endif  // OPENMINI_PLUGIN_COMMON_PLUGINPROCESSOR_H_
