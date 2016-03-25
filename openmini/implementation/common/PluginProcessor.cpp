/// @filename PluginProcessor.cpp
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


#include "openmini/implementation/common/PluginProcessor.h"
#include "openmini/implementation/common/PluginEditor.h"
#include "openmini/src/synthesizer/parameter_meta.h"

OpenMiniAudioProcessor::OpenMiniAudioProcessor()
  : keyboard_state_(),
    lastUIWidth(kMaxWindowWidth / 2),
    lastUIHeight(kMaxWindowHeight / 2),
    synth_(),
    process_time_(0.0) {
  // Manually create one output bus
  busArrangement.outputBuses.clear();
  busArrangement.outputBuses.add(AudioProcessorBus(String("Output #") += String(1), AudioChannelSet::disabled()));
}

OpenMiniAudioProcessor::~OpenMiniAudioProcessor() {
}

const juce::String OpenMiniAudioProcessor::getName() const {
  return JucePlugin_Name;
}

int OpenMiniAudioProcessor::getNumParameters() {
  return synth_.ParametersCount();
}

float OpenMiniAudioProcessor::getParameter(int index) {
  return synth_.GetValue(index);
}

void OpenMiniAudioProcessor::setParameter(int index, float newValue) {
  synth_.SetValue(index, newValue);
  // Inform UI of any change
  sendChangeMessage();
}

const juce::String OpenMiniAudioProcessor::getParameterName(int index) {
  return juce::String(synth_.GetMetadata(index).name());
}

const juce::String OpenMiniAudioProcessor::getParameterText(int index) {
  return juce::String(synth_.GetMetadata(index).description());
}

bool OpenMiniAudioProcessor::acceptsMidi() const {
  #if JucePlugin_WantsMidiInput
  return true;
  #else
  return false;
  #endif
}

bool OpenMiniAudioProcessor::producesMidi() const {
  #if JucePlugin_ProducesMidiOutput
  return true;
  #else
  return false;
  #endif
}

double OpenMiniAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int OpenMiniAudioProcessor::getNumPrograms() {
  return 0;
}

int OpenMiniAudioProcessor::getCurrentProgram() {
  return 0;
}

void OpenMiniAudioProcessor::setCurrentProgram(int index) {
}

const String OpenMiniAudioProcessor::getProgramName(int index) {
  return String::empty;
}

void OpenMiniAudioProcessor::changeProgramName(int index,
                                               const juce::String& newName) {
}

void OpenMiniAudioProcessor::prepareToPlay(double sampleRate,
                                           int samplesPerBlock) {
  // Notify UI of the last changes
  sendChangeMessage();
  synth_.SetOutputSamplingFrequency(static_cast<float>(sampleRate));
  keyboard_state_.reset();
}

void OpenMiniAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void OpenMiniAudioProcessor::processBlock(juce::AudioSampleBuffer& buffer,
                                          juce::MidiBuffer& midiMessages) {
  // This scans the keyboard state and inject into our current midi buffer
  // any pending events
  keyboard_state_.processNextMidiBuffer(midiMessages,
                                        0,
                                        buffer.getNumSamples(),
                                        true);

  // Iterating on midi messages...
  juce::MidiBuffer::Iterator midi_iterator(midiMessages);
  juce::MidiMessage midi_message;
  int midi_event_position(0);
  bool event_found = midi_iterator.getNextEvent(midi_message,
                                                midi_event_position);
  while (event_found) {
    if (midi_message.isNoteOn()) {
      triggerNoteOn(midi_message.getNoteNumber());
    } else if (midi_message.isNoteOff()) {
      triggerNoteOff(midi_message.getNoteNumber());
    }
    event_found = midi_iterator.getNextEvent(midi_message,
                                             midi_event_position);
  }  // Iterating on midi messages...
  midiMessages.clear();

  const double counter_start(juce::Time::getMillisecondCounterHiRes());

  synth_.ProcessAudio(buffer.getArrayOfWritePointers()[0], buffer.getNumSamples());

  process_time_ = juce::Time::getMillisecondCounterHiRes() - counter_start;
}

bool OpenMiniAudioProcessor::hasEditor() const {
  return true;
}

AudioProcessorEditor* OpenMiniAudioProcessor::createEditor() {
  return new OpenMiniAudioProcessorEditor(this);
}

void OpenMiniAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void OpenMiniAudioProcessor::setStateInformation(const void* data,
                                                 int sizeInBytes) {
}

void OpenMiniAudioProcessor::addChangeListener(
    juce::ChangeListener* listener) {
  // Call parent method
  ChangeBroadcaster::addChangeListener(listener);
  // Update newly added listener
  sendChangeMessage();
}

void OpenMiniAudioProcessor::triggerNoteOn(const int midi_note) {
  synth_.NoteOn(midi_note);
}
void OpenMiniAudioProcessor::triggerNoteOff(const int midi_note) {
  synth_.NoteOff(midi_note);
}

// DEBUG
double OpenMiniAudioProcessor::GetLastProcessTime() const {
  return process_time_;
}
//  /DEBUG

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new OpenMiniAudioProcessor();
}
