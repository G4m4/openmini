/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OpenMiniAudioProcessor::OpenMiniAudioProcessor()
{
}

OpenMiniAudioProcessor::~OpenMiniAudioProcessor()
{
}

//==============================================================================
const String OpenMiniAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int OpenMiniAudioProcessor::getNumParameters()
{
    return 0;
}

float OpenMiniAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void OpenMiniAudioProcessor::setParameter (int index, float newValue)
{
}

const String OpenMiniAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String OpenMiniAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String OpenMiniAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String OpenMiniAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool OpenMiniAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool OpenMiniAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool OpenMiniAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OpenMiniAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OpenMiniAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double OpenMiniAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OpenMiniAudioProcessor::getNumPrograms()
{
    return 0;
}

int OpenMiniAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OpenMiniAudioProcessor::setCurrentProgram (int index)
{
}

const String OpenMiniAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void OpenMiniAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OpenMiniAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OpenMiniAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void OpenMiniAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  synth_.ProcessAudio(buffer.getArrayOfChannels()[0], buffer.getNumSamples());
}

//==============================================================================
bool OpenMiniAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OpenMiniAudioProcessor::createEditor()
{
    return new OpenMiniAudioProcessorEditor (this);
}

//==============================================================================
void OpenMiniAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OpenMiniAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void OpenMiniAudioProcessor::triggerNoteOn() {
  synth_.NoteOn(42);
}
void OpenMiniAudioProcessor::triggerNoteOff() {
  synth_.NoteOff(42);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OpenMiniAudioProcessor();
}
