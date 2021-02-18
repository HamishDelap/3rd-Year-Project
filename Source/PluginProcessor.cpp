/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Oscillator.h"
#include "StateManager.h"

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
    /*apvt(*this, nullptr),*/ fft(fftOrder), windowFunction(fftSize, juce::dsp::WindowingFunction<float>::hann), stateManager(*this)
#endif
{
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

    modEnvelope = std::shared_ptr<ModEnvelope>(new ModEnvelope(0, 0, 0, 0, 0));
    modEnvelope->setSampleRate(lastSampleRate);
    modLfo = std::shared_ptr<Lfo>(new Lfo(lastSampleRate, 1, 1, 1));
    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i))) {
            myVoice->setModAdsr(modEnvelope);
            myVoice->setModLfo(modLfo);
        }
    }

}

void ThirdYearProjectAudioProcessor::updateFilter() {
    float cutoff = *stateManager.apvt.getRawParameterValue("CUTOFF");
    float resonance = *stateManager.apvt.getRawParameterValue("RESONANCE");
    float coeff = 0.8f;

    if (modEnvelope->isOn()) {
        cutoff = cutoff * modEnvelope->getOutput(2);
    }
    
    cutoff = cutoff - abs(modLfo->getOutput(2) * 10);
    currentCutoff = cutoff + coeff * (currentCutoff - cutoff);

    if (currentCutoff <= 0) {
        currentCutoff = 1;
    }

    if (currentCutoff > static_cast<float> (lastSampleRate * 0.5)) {
        currentCutoff = lastSampleRate * 0.5;
    }

    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, currentCutoff, resonance);
}

void ThirdYearProjectAudioProcessor::checkAlgoChanged() {
    if (*(float*)stateManager.apvt.getRawParameterValue("ALGO") != algo) {
        algoChanged = true;
    }
}

void ThirdYearProjectAudioProcessor::checkPresetChanged() {
    if (preset != lastPreset) {
        StringArray presets = stateManager.getPresets();
        stateManager.readPreset(presets[preset-1]);
        lastPreset = preset;
    }
    
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
    checkPresetChanged();
    checkAlgoChanged();
    algo = *(float*)stateManager.apvt.getRawParameterValue("ALGO");

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    modLfo->setFrequency((float*)stateManager.apvt.getRawParameterValue("LFOFREQ"));
    modLfo->setLevel((float*)stateManager.apvt.getRawParameterValue("LFOAMOUNT"));
    modLfo->setWaveform((float*)stateManager.apvt.getRawParameterValue("LFOWAVEFORM"));
    modLfo->toggleDest((float*)stateManager.apvt.getRawParameterValue("LFOPITCH"), 1);
    modLfo->toggleDest((float*)stateManager.apvt.getRawParameterValue("LFOFILTER"), 2);
    modLfo->toggleDest((float*)stateManager.apvt.getRawParameterValue("LFOLEVEL"), 3);

    modEnvelope->setAttack((float*)stateManager.apvt.getRawParameterValue("MODENVATTACK"));
    modEnvelope->setDecay((float*)stateManager.apvt.getRawParameterValue("MODENVDECAY"));
    modEnvelope->setSustain((float*)stateManager.apvt.getRawParameterValue("MODENVSUSTAIN"));
    modEnvelope->setRelease((float*)stateManager.apvt.getRawParameterValue("MODENVRELEASE"));
    modEnvelope->setAmount((float*)stateManager.apvt.getRawParameterValue("MODENVAMOUNT"));
    modEnvelope->toggleDest((float*)stateManager.apvt.getRawParameterValue("MODENVPITCH"), 1);
    modEnvelope->toggleDest((float*)stateManager.apvt.getRawParameterValue("MODENVFILTER"), 2);
    modEnvelope->toggleDest((float*)stateManager.apvt.getRawParameterValue("MODENVLEVEL"), 3);

    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i))) {
            myVoice->setOP1MODINDEX((float*)stateManager.apvt.getRawParameterValue("OP1MODINDEX"));
            myVoice->setOP2MODINDEX((float*)stateManager.apvt.getRawParameterValue("OP2MODINDEX"));
            myVoice->setOP3MODINDEX((float*)stateManager.apvt.getRawParameterValue("OP3MODINDEX"));
            myVoice->setOP4MODINDEX((float*)stateManager.apvt.getRawParameterValue("OP4MODINDEX"));

            myVoice->setOP1LEVEL((float*)stateManager.apvt.getRawParameterValue("OP1LEVEL"));
            myVoice->setOP2LEVEL((float*)stateManager.apvt.getRawParameterValue("OP2LEVEL"));
            myVoice->setOP3LEVEL((float*)stateManager.apvt.getRawParameterValue("OP3LEVEL"));
            myVoice->setOP4LEVEL((float*)stateManager.apvt.getRawParameterValue("OP4LEVEL"));

            myVoice->setADSRSampleRate(lastSampleRate);
			myVoice->setOp1Adsr((float*)stateManager.apvt.getRawParameterValue("OP1ATTACK"), (float*)stateManager.apvt.getRawParameterValue("OP1DECAY"), (float*)stateManager.apvt.getRawParameterValue("OP1SUSTAIN"), (float*)stateManager.apvt.getRawParameterValue("OP1RELEASE"));
            myVoice->setOp2Adsr((float*)stateManager.apvt.getRawParameterValue("OP2ATTACK"), (float*)stateManager.apvt.getRawParameterValue("OP2DECAY"), (float*)stateManager.apvt.getRawParameterValue("OP2SUSTAIN"), (float*)stateManager.apvt.getRawParameterValue("OP2RELEASE"));
            myVoice->setOp3Adsr((float*)stateManager.apvt.getRawParameterValue("OP3ATTACK"), (float*)stateManager.apvt.getRawParameterValue("OP3DECAY"), (float*)stateManager.apvt.getRawParameterValue("OP3SUSTAIN"), (float*)stateManager.apvt.getRawParameterValue("OP3RELEASE"));
            myVoice->setOp4Adsr((float*)stateManager.apvt.getRawParameterValue("OP4ATTACK"), (float*)stateManager.apvt.getRawParameterValue("OP4DECAY"), (float*)stateManager.apvt.getRawParameterValue("OP4SUSTAIN"), (float*)stateManager.apvt.getRawParameterValue("OP4RELEASE"));
            
            myVoice->setAlgo((float*)stateManager.apvt.getRawParameterValue("ALGO"));

            myVoice->setOP1WAVEFORM((float*)stateManager.apvt.getRawParameterValue("OP1WAVEFORM"));
            myVoice->setOP2WAVEFORM((float*)stateManager.apvt.getRawParameterValue("OP2WAVEFORM"));
            myVoice->setOP3WAVEFORM((float*)stateManager.apvt.getRawParameterValue("OP3WAVEFORM"));
            myVoice->setOP4WAVEFORM((float*)stateManager.apvt.getRawParameterValue("OP4WAVEFORM"));

           // myVoice->setModAdsrParams((float*)apvt.getRawParameterValue("MODATTACK"), (float*)apvt.getRawParameterValue("MODDECAY"), (float*)apvt.getRawParameterValue("MODSUSTAIN"), (float*)apvt.getRawParameterValue("MODRELEASE"));
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


    for (auto i = 0; i < buffer.getNumSamples(); ++i) {
        modLfo->lfoStep();
    }

    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer);
    updateFilter();
    lowPassFilter.process(dsp::ProcessContextReplacing<float>(block));

    currentLevel = *(float*)stateManager.apvt.getRawParameterValue("MASTERLEVEL");
    float lfoLevel = modLfo->getOutput(3) * 0.01;
    previousLfoLevel = lfoLevel + 0.7f * (previousLfoLevel - lfoLevel);
    currentLevel += previousLfoLevel;
    buffer.applyGainRamp(0, buffer.getNumSamples(), masterLevel, currentLevel);
    masterLevel = currentLevel;
    
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

    //auto state = stateManager.apvt.copyState();
    //std::unique_ptr<XmlElement> xml(state.createXml());
    //copyXmlToBinary(*xml, destData);

    stateManager.writeState(destData);
}

void ThirdYearProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    //if (xmlState != nullptr) 
       // if (xmlState->hasTagName(stateManager.apvt.state.getType()))
         //   stateManager.apvt.state = ValueTree::fromXml(*xmlState);
    stateManager.readState(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThirdYearProjectAudioProcessor();
}
