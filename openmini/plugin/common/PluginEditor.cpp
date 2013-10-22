/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

OpenMiniAudioProcessorEditor::OpenMiniAudioProcessorEditor (OpenMiniAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      owner_(ownerFilter) {
  addAndMakeVisible (keyboard_ = new juce::TextButton("Trigger Note On"));
  keyboard_->addListener(this);
  // This is where our plugin's editor size is set.
  setSize (kMainWindowSizeX, kMainWindowSizeY);
}

OpenMiniAudioProcessorEditor::~OpenMiniAudioProcessorEditor() {
}

void OpenMiniAudioProcessorEditor::paint (Graphics& g) {
  g.fillAll (Colours::white);
  keyboard_->setBounds(0, 0, 100, 100);
}

void OpenMiniAudioProcessorEditor::buttonClicked (Button*) {
  return;
}

void OpenMiniAudioProcessorEditor::buttonStateChanged (Button* button) {
  if (button->isDown()) {
    owner_->triggerNoteOn();
  } else {
    owner_->triggerNoteOff();
  }
}
