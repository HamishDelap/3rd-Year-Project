/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Oscillator.h"

//==============================================================================
ThirdYearProjectAudioProcessor::ThirdYearProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvt(*this, nullptr), fft(fftOrder), windowFunction(fftSize, juce::dsp::WindowingFunction<float>::hann)
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
    apvt.createAndAddParameter("OP1LEVEL", "OP1LEVEL", "OP1LEVEL", op1LevelRange, 1.0f, nullptr, nullptr);
    NormalisableRange<float> op2LevelRange(0, 1);
    apvt.createAndAddParameter("OP2LEVEL", "OP2LEVEL", "OP2LEVEL", op2LevelRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op3LevelRange(0, 1);
	apvt.createAndAddParameter("OP3LEVEL", "OP3LEVEL", "OP3LEVEL", op3LevelRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op4LevelRange(0, 1);
	apvt.createAndAddParameter("OP4LEVEL", "OP4LEVEL", "OP4LEVEL", op4LevelRange, 0.0f, nullptr, nullptr);

    // OP1 Envelope
	NormalisableRange<float> op1AttackRange(0, 6);
	apvt.createAndAddParameter("OP1ATTACK", "OP1ATTACK", "OP1ATTACK", op1AttackRange, 0.2f, nullptr, nullptr);
	NormalisableRange<float> op1DecayRange(0, 3);
	apvt.createAndAddParameter("OP1DECAY", "OP1DECAY", "OP1DECAY", op1DecayRange, 0.0f, nullptr, nullptr);
	NormalisableRange<float> op1SustainRange(0, 1);
	apvt.createAndAddParameter("OP1SUSTAIN", "OP1SUSTAIN", "OP1SUSTAIN", op1SustainRange, 1.0f, nullptr, nullptr);
	NormalisableRange<float> op1ReleaseRange(0, 3);
	apvt.createAndAddParameter("OP1RELEASE", "OP1RELEASE", "OP1RELEASE", op1ReleaseRange, 0.5f, nullptr, nullptr);
   
    // OP2 Envelope
    NormalisableRange<float> op2AttackRange(0, 6);
    apvt.createAndAddParameter("OP2ATTACK", "OP2ATTACK", "OP2ATTACK", op2AttackRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op2DecayRange(0, 3);
    apvt.createAndAddParameter("OP2DECAY", "OP2DECAY", "OP2DECAY", op2DecayRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op2SustainRange(0, 1);
    apvt.createAndAddParameter("OP2SUSTAIN", "OP2SUSTAIN", "OP2SUSTAIN", op2SustainRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op2ReleaseRange(0, 3);
    apvt.createAndAddParameter("OP2RELEASE", "OP2RELEASE", "OP2RELEASE", op2ReleaseRange, 0.0f, nullptr, nullptr);

    // OP3 Envelope
    NormalisableRange<float> op3AttackRange(0, 6);
    apvt.createAndAddParameter("OP3ATTACK", "OP3ATTACK", "OP3ATTACK", op3AttackRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op3DecayRange(0, 3);
    apvt.createAndAddParameter("OP3DECAY", "OP3DECAY", "OP3DECAY", op3DecayRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op3SustainRange(0, 1);
    apvt.createAndAddParameter("OP3SUSTAIN", "OP3SUSTAIN", "OP3SUSTAIN", op3SustainRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op3ReleaseRange(0, 3);
    apvt.createAndAddParameter("OP3RELEASE", "OP3RELEASE", "OP3RELEASE", op3ReleaseRange, 0.0f, nullptr, nullptr);

    // OP4 Envelope
    NormalisableRange<float> op4AttackRange(0, 6);
    apvt.createAndAddParameter("OP4ATTACK", "OP4ATTACK", "OP4ATTACK", op4AttackRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op4DecayRange(0, 3);
    apvt.createAndAddParameter("OP4DECAY", "OP4DECAY", "OP4DECAY", op4DecayRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op4SustainRange(0, 1);
    apvt.createAndAddParameter("OP4SUSTAIN", "OP4SUSTAIN", "OP4SUSTAIN", op4SustainRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> op4ReleaseRange(0, 3);
    apvt.createAndAddParameter("OP4RELEASE", "OP4RELEASE", "OP4RELEASE", op4ReleaseRange, 0.0f, nullptr, nullptr);
    
    // MOD Envelope
    NormalisableRange<float> modAttackRange(0, 3);
    apvt.createAndAddParameter("MODATTACK", "MODATTACK", "MODATTACK", modAttackRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> modDecayRange(0, 3);
    apvt.createAndAddParameter("MODDECAY", "MODDECAY", "MODDECAY", modDecayRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> modSustainRange(0, 1);
    apvt.createAndAddParameter("MODSUSTAIN", "MODSUSTAIN", "MODSUSTAIN", modSustainRange, 0.0f, nullptr, nullptr);
    NormalisableRange<float> modReleaseRange(0, 3);
    apvt.createAndAddParameter("MODRELEASE", "MODRELEASE", "MODRELEASE", modReleaseRange, 0.0f, nullptr, nullptr);
    
    // Filter
    NormalisableRange<float> cutoffRange(0.1, 20000);
	apvt.createAndAddParameter("CUTOFF", "CUTOFF", "CUTOFF", cutoffRange, 20000.0f, nullptr, nullptr);
	NormalisableRange<float> resonanceRange(0.1,1);
	apvt.createAndAddParameter("RESONANCE", "RESONANCE", "RESONANCE", resonanceRange, 1.0f, nullptr, nullptr);

    NormalisableRange<float> algoRange(1, 4);
    apvt.createAndAddParameter("ALGO", "ALGO", "ALGO", algoRange, 1.0f, nullptr, nullptr);

    NormalisableRange<float> op1WaveformRange(1, 3);
    apvt.createAndAddParameter("OP1WAVEFORM", "OP1WAVEFORM", "OP1WAVEFORM", op1WaveformRange, 1.0f, nullptr, nullptr);
    NormalisableRange<float> op2WaveformRange(1, 3);
    apvt.createAndAddParameter("OP2WAVEFORM", "OP2WAVEFORM", "OP2WAVEFORM", op2WaveformRange, 1.0f, nullptr, nullptr);
    NormalisableRange<float> op3WaveformRange(1, 3);
    apvt.createAndAddParameter("OP3WAVEFORM", "OP3WAVEFORM", "OP3WAVEFORM", op3WaveformRange, 1.0f, nullptr, nullptr);
    NormalisableRange<float> op4WaveformRange(1, 3);
    apvt.createAndAddParameter("OP4WAVEFORM", "OP4WAVEFORM", "OP4WAVEFORM", op4WaveformRange, 1.0f, nullptr, nullptr);

    apvt.state = ValueTree("apvt");

    mySynth.clearVoices();
    // Create 5 voices.
    for (int i = 0; i < 10; i++) {
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

void ThirdYearProjectAudioProcessor::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}

// Taken from the JUCE Spectrum Analyser tutorial
// Function to calculate next frame of spectrum (will be called by the editor)
void ThirdYearProjectAudioProcessor::drawNextFrameOfSpectrum()
{
    // Apply the windowing funtion
    windowFunction.multiplyWithWindowingTable(fftData, fftSize);

    // Apply the forwardFFT
    fft.performFrequencyOnlyForwardTransform(fftData);

    auto minLevel = -100.0f;
    auto maxLevel = 0.0f;

    for (int i = 0; i < scopeSize; i++)
    {
        auto skewedXAxis = 1.0f - std::exp(std::log(1.0f - (float) i / (float) scopeSize) * 0.2f);
        auto fftDataIndex = jlimit(0, fftSize / 2, (int) (skewedXAxis * (float) fftSize * 0.5f));
        auto currentLevel = jmap(jlimit(minLevel, maxLevel, Decibels::gainToDecibels(fftData[fftDataIndex]) - 
                                                            Decibels::gainToDecibels((float) fftSize)), minLevel, maxLevel, 0.0f, 1.0f);
        scopeData[i] = currentLevel;
    }

}

void ThirdYearProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

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
			myVoice->setOp1Adsr((float*)apvt.getRawParameterValue("OP1ATTACK"), (float*)apvt.getRawParameterValue("OP1DECAY"), (float*)apvt.getRawParameterValue("OP1SUSTAIN"), (float*)apvt.getRawParameterValue("OP1RELEASE"));
            myVoice->setOp2Adsr((float*)apvt.getRawParameterValue("OP2ATTACK"), (float*)apvt.getRawParameterValue("OP2DECAY"), (float*)apvt.getRawParameterValue("OP2SUSTAIN"), (float*)apvt.getRawParameterValue("OP2RELEASE"));
            myVoice->setOp3Adsr((float*)apvt.getRawParameterValue("OP3ATTACK"), (float*)apvt.getRawParameterValue("OP3DECAY"), (float*)apvt.getRawParameterValue("OP3SUSTAIN"), (float*)apvt.getRawParameterValue("OP3RELEASE"));
            myVoice->setOp4Adsr((float*)apvt.getRawParameterValue("OP4ATTACK"), (float*)apvt.getRawParameterValue("OP4DECAY"), (float*)apvt.getRawParameterValue("OP4SUSTAIN"), (float*)apvt.getRawParameterValue("OP4RELEASE"));
            
            myVoice->setAlgo((float*)apvt.getRawParameterValue("ALGO"));

            myVoice->setOP1WAVEFORM((float*)apvt.getRawParameterValue("OP1WAVEFORM"));
            myVoice->setOP2WAVEFORM((float*)apvt.getRawParameterValue("OP2WAVEFORM"));
            myVoice->setOP3WAVEFORM((float*)apvt.getRawParameterValue("OP3WAVEFORM"));
            myVoice->setOP4WAVEFORM((float*)apvt.getRawParameterValue("OP4WAVEFORM"));

            myVoice->setModAdsr((float*)apvt.getRawParameterValue("MODATTACK"), (float*)apvt.getRawParameterValue("MODDECAY"), (float*)apvt.getRawParameterValue("MODSUSTAIN"), (float*)apvt.getRawParameterValue("MODRELEASE"));
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
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer (channel);

        for (auto i = 0; i < buffer.getNumSamples(); ++i) {

            pushNextSampleIntoFifo(channelData[i]);
        }
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
