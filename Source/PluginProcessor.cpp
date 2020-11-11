/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThirdYearProjectAudioProcessor::ThirdYearProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvt(*this, nullptr)
#endif
{
    // OP Mod Indexes
    // Preparing the value tree state
    NormalisableRange<float> op1ModIndexRange(0, 12);
    apvt.createAndAddParameter("OP1MODINDEX", "OP1MODINDEX", "OP1MODINDEX", op1ModIndexRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op2ModIndexRange(0, 12);
	apvt.createAndAddParameter("OP2MODINDEX", "OP2MODINDEX", "OP2MODINDEX", op2ModIndexRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op3ModIndexRange(0, 12);
	apvt.createAndAddParameter("OP3MODINDEX", "OP3MODINDEX", "OP3MODINDEX", op3ModIndexRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op4ModIndexRange(0, 12);
	apvt.createAndAddParameter("OP4MODINDEX", "OP4MODINDEX", "OP4MODINDEX", op4ModIndexRange, 0.0f, nullptr, nullptr);

    // OP Levels
    NormalisableRange<float> op1LevelRange(0, 1);
    apvt.createAndAddParameter("OP1LEVEL", "OP1LEVEL", "OP1LEVEL", op1LevelRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op2LevelRange(0, 1);
    apvt.createAndAddParameter("OP2LEVEL", "OP2LEVEL", "OP2LEVEL", op2LevelRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op3LevelRange(0, 1);
	apvt.createAndAddParameter("OP3LEVEL", "OP3LEVEL", "OP3LEVEL", op3LevelRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op4LevelRange(0, 1);
	apvt.createAndAddParameter("OP4LEVEL", "OP4LEVEL", "OP4LEVEL", op4LevelRange, 0.0f, nullptr, nullptr);

    //Envelope
	NormalisableRange<float> ampAttackRange(0, 3);
	apvt.createAndAddParameter("AMPATTACK", "AMPATTACK", "AMPATTACK", ampAttackRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> ampDecayRange(0, 3);
	apvt.createAndAddParameter("AMPDECAY", "AMPDECAY", "AMPDECAY", ampDecayRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> ampSustainRange(0, 1);
	apvt.createAndAddParameter("AMPSUSTAIN", "AMPSUSTAIN", "AMPSUSTAIN", ampSustainRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> ampReleaseRange(0, 3);
	apvt.createAndAddParameter("AMPRELEASE", "AMPRELEASE", "AMPRELEASE", ampReleaseRange, 0.0f, nullptr, nullptr);
   
    NormalisableRange<float> cutoffRange(0, 20000);
	apvt.createAndAddParameter("CUTOFF", "CUTOFF", "CUTOFF", cutoffRange, 1.0f, nullptr, nullptr);
	NormalisableRange<float> resonanceRange(0,100);
	apvt.createAndAddParameter("RESONANCE", "RESONANCE", "RESONANCE", resonanceRange, 1.0f, nullptr, nullptr);

    apvt.state = ValueTree("apvt");

    mySynth.clearVoices();
    // Create 5 voices.
    for (int i = 0; i < 5; i++) {
        mySynth.addVoice(new SynthVoice());
    }
    // Tidy up unwanted sounds.
    mySynth.clearSounds();
    // Add a new sound.
    mySynth.addSound(new SynthSound());

}

ThirdYearProjectAudioProcessor::~ThirdYearProjectAudioProcessor()
{
}

//==============================================================================
const juce::String ThirdYearProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThirdYearProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThirdYearProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThirdYearProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThirdYearProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThirdYearProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThirdYearProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThirdYearProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ThirdYearProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void ThirdYearProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ThirdYearProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..


    ignoreUnused(samplesPerBlock);

    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);

    midiCollector.reset(sampleRate);

	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	lowPassFilter.prepare(spec);
	lowPassFilter.reset();
}

void ThirdYearProjectAudioProcessor::updateFilter() {
    float cutoff = *apvt.getRawParameterValue("CUTOFF");
    float resonance = *apvt.getRawParameterValue("RESONANCE");

    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, cutoff, resonance);
}

void ThirdYearProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThirdYearProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ThirdYearProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i))) {
            myVoice->setOP1MODINDEX((float*)apvt.getRawParameterValue("OP1MODINDEX"));
            myVoice->setOP2MODINDEX((float*)apvt.getRawParameterValue("OP2MODINDEX"));
            myVoice->setOP3MODINDEX((float*)apvt.getRawParameterValue("OP3MODINDEX"));
            myVoice->setOP4MODINDEX((float*)apvt.getRawParameterValue("OP4MODINDEX"));

            myVoice->setOP1LEVEL((float*)apvt.getRawParameterValue("OP1LEVEL"));
            myVoice->setOP2LEVEL((float*)apvt.getRawParameterValue("OP2LEVEL"));
            myVoice->setOP3LEVEL((float*)apvt.getRawParameterValue("OP3LEVEL"));
            myVoice->setOP4LEVEL((float*)apvt.getRawParameterValue("OP4LEVEL"));

            myVoice->setADSRSampleRate(lastSampleRate);
			myVoice->setAmpADSR((float*) apvt.getRawParameterValue("AMPATTACK"), (float*) apvt.getRawParameterValue("AMPDECAY"), (float*)apvt.getRawParameterValue("AMPSUSTAIN"), (float*) apvt.getRawParameterValue("AMPRELEASE"));
        }
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer);
    updateFilter();
    lowPassFilter.process(dsp::ProcessContextReplacing<float>(block));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool ThirdYearProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ThirdYearProjectAudioProcessor::createEditor()
{
    return new ThirdYearProjectAudioProcessorEditor(*this);
}

//==============================================================================
void ThirdYearProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThirdYearProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThirdYearProjectAudioProcessor();
}
