#include "PluginProcessor.h"


//==============================================================================
EQPluginProcessor::EQPluginProcessor()
{
    mAudealizeAudioProcessor = new AudealizeeqAudioProcessor();
}

EQPluginProcessor::~EQPluginProcessor()
{
    mAudealizeAudioProcessor = nullptr;
}

//==============================================================================
const String EQPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQPluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQPluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double EQPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQPluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQPluginProcessor::getCurrentProgram()
{
    return 0;
}

void EQPluginProcessor::setCurrentProgram (int index)
{
}

const String EQPluginProcessor::getProgramName (int index)
{
    return String();
}

void EQPluginProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EQPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mAudealizeAudioProcessor->prepareToPlay(sampleRate, samplesPerBlock);
}

void EQPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mAudealizeAudioProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQPluginProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet) && mAudealizeAudioProcessor->setPreferredBusArrangement(isInput, bus, preferredSet);
}
#endif

void EQPluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    mAudealizeAudioProcessor->processBlock(buffer, midiMessages);
}

//==============================================================================
bool EQPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EQPluginProcessor::createEditor()
{
    return mAudealizeAudioProcessor->createEditor(false);
}

//==============================================================================
void EQPluginProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    mAudealizeAudioProcessor->getStateInformation(destData);
}

void EQPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    mAudealizeAudioProcessor->setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EQPluginProcessor();
}

int EQPluginProcessor::getNumParameters(){
    return mAudealizeAudioProcessor->getNumParameters();
}
