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
    spectrumProcessor.reset(new SpectrumProcessor());
    mySynth.clearVoices();
    // Create 5 voices.
    for (int i = 0; i < 4; i++) {
        mySynth.addVoice(new SynthVoice());
    }
    // Tidy up unwanted sounds.
    mySynth.clearSounds();
    // Add a new sound.
    mySynth.addSound(new SynthSound());

    // Parameter Pointers
    lfoFreq = stateManager.apvt.getRawParameterValue("LFOFREQ");
    lfoAmount = stateManager.apvt.getRawParameterValue("LFOAMOUNT");
    lfoWaveform = stateManager.apvt.getRawParameterValue("LFOWAVEFORM");
    lfoPitch = stateManager.apvt.getRawParameterValue("LFOPITCH");
    lfoFilter = stateManager.apvt.getRawParameterValue("LFOFILTER");
    lfoLevel = stateManager.apvt.getRawParameterValue("LFOLEVEL");

    modEnvAttack = stateManager.apvt.getRawParameterValue("MODENVATTACK");
    modEnvDecay = stateManager.apvt.getRawParameterValue("MODENVDECAY");
    modEnvSustain = stateManager.apvt.getRawParameterValue("MODENVSUSTAIN");
    modEnvRelease = stateManager.apvt.getRawParameterValue("MODENVRELEASE");
    modEnvAmount = stateManager.apvt.getRawParameterValue("MODENVAMOUNT");
    modEnvPitch = stateManager.apvt.getRawParameterValue("MODENVPITCH");
    modEnvFilter = stateManager.apvt.getRawParameterValue("MODENVFILTER");
    modEnvLevel = stateManager.apvt.getRawParameterValue("MODENVLEVEL");

    op1ModIndex = stateManager.apvt.getRawParameterValue("OP1MODINDEX");
    op2ModIndex = stateManager.apvt.getRawParameterValue("OP2MODINDEX");
    op3ModIndex = stateManager.apvt.getRawParameterValue("OP3MODINDEX");
    op4ModIndex = stateManager.apvt.getRawParameterValue("OP4MODINDEX");

    op1Level = stateManager.apvt.getRawParameterValue("OP1LEVEL");
    op2Level = stateManager.apvt.getRawParameterValue("OP2LEVEL");
    op3Level = stateManager.apvt.getRawParameterValue("OP3LEVEL");
    op4Level = stateManager.apvt.getRawParameterValue("OP4LEVEL");

    op1Attack = stateManager.apvt.getRawParameterValue("OP1ATTACK");
    op1Decay = stateManager.apvt.getRawParameterValue("OP1DECAY");
    op1Sustain = stateManager.apvt.getRawParameterValue("OP1SUSTAIN");
    op1Release = stateManager.apvt.getRawParameterValue("OP1RELEASE");

    op2Attack = stateManager.apvt.getRawParameterValue("OP2ATTACK");
    op2Decay = stateManager.apvt.getRawParameterValue("OP2DECAY");
    op2Sustain = stateManager.apvt.getRawParameterValue("OP2SUSTAIN");
    op2Release = stateManager.apvt.getRawParameterValue("OP2RELEASE");

    op3Attack = stateManager.apvt.getRawParameterValue("OP3ATTACK");
    op3Decay = stateManager.apvt.getRawParameterValue("OP3DECAY");
    op3Sustain = stateManager.apvt.getRawParameterValue("OP3SUSTAIN");
    op3Release = stateManager.apvt.getRawParameterValue("OP3RELEASE");

    op4Attack = stateManager.apvt.getRawParameterValue("OP4ATTACK");
    op4Decay = stateManager.apvt.getRawParameterValue("OP4DECAY");
    op4Sustain = stateManager.apvt.getRawParameterValue("OP4SUSTAIN");
    op4Release = stateManager.apvt.getRawParameterValue("OP4RELEASE");

    algoPointer = stateManager.apvt.getRawParameterValue("ALGO");

    op1Waveform = stateManager.apvt.getRawParameterValue("OP1WAVEFORM");
    op2Waveform = stateManager.apvt.getRawParameterValue("OP2WAVEFORM");
    op3Waveform = stateManager.apvt.getRawParameterValue("OP3WAVEFORM");
    op4Waveform = stateManager.apvt.getRawParameterValue("OP4WAVEFORM");

    modWheelController.reset(new ModWheelController());

	modWheelLfoFreq = stateManager.apvt.getRawParameterValue("MODWHEELLFOFREQ");
	modWheelLfoAmt = stateManager.apvt.getRawParameterValue("MODWHEELLFOAMT");
	modWheelCutoff = stateManager.apvt.getRawParameterValue("MODWHEELCUTOFF");

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
	
	if (modWheelController->destActive(2))
	{
        cutoff = cutoff * (controllerValMapped + 0.01f);
	}

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
    if (algoPointer->load() != algo) {
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

void ThirdYearProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    checkPresetChanged();
    checkAlgoChanged();
    algo = algoPointer->load();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    modWheelController->toggleDest(modWheelLfoFreq->load(), 0);
    modWheelController->toggleDest(modWheelLfoAmt->load(), 1);
    modWheelController->toggleDest(modWheelCutoff->load(), 2);
	
    if (modWheelController->destActive(0)) {
        modLfo->setFrequency(lfoFreq->load() * controllerValMapped);
    } else {
        modLfo->setFrequency(lfoFreq->load());
    }
    if (modWheelController->destActive(1)) {
        modLfo->setLevel(lfoAmount->load() * controllerValMapped);
    }
    else {
        modLfo->setLevel(lfoAmount->load());
    }
    
    modLfo->setWaveform(lfoWaveform->load());
    modLfo->toggleDest(lfoPitch->load(), 1);
    modLfo->toggleDest(lfoFilter->load(), 2);
    modLfo->toggleDest(lfoLevel->load(), 3);

    modEnvelope->setAttack(modEnvAttack->load());
    modEnvelope->setDecay(modEnvDecay->load());
    modEnvelope->setSustain(modEnvSustain->load());
    modEnvelope->setRelease(modEnvRelease->load());
    modEnvelope->setAmount(modEnvAmount->load());
    modEnvelope->toggleDest(modEnvPitch->load(), 1);
    modEnvelope->toggleDest(modEnvFilter->load(), 2);
    modEnvelope->toggleDest(modEnvLevel->load(), 3);

    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i))) {
            controllerValue = myVoice->getControllerValue();
            myVoice->setOP1MODINDEX(op1ModIndex->load());
            myVoice->setOP2MODINDEX(op2ModIndex->load());
            myVoice->setOP3MODINDEX(op3ModIndex->load());
            myVoice->setOP4MODINDEX(op4ModIndex->load());

            myVoice->setOP1LEVEL(op1Level->load());
            myVoice->setOP2LEVEL(op2Level->load());
            myVoice->setOP3LEVEL(op3Level->load());
            myVoice->setOP4LEVEL(op4Level->load());

            myVoice->setADSRSampleRate(lastSampleRate);
			myVoice->setOp1Adsr(op1Attack->load(), op1Decay->load(), op1Sustain->load(), op1Release->load());
            myVoice->setOp2Adsr(op2Attack->load(), op2Decay->load(), op2Sustain->load(), op2Release->load());
            myVoice->setOp3Adsr(op3Attack->load(), op3Decay->load(), op3Sustain->load(), op3Release->load());
            myVoice->setOp4Adsr(op4Attack->load(), op4Decay->load(), op4Sustain->load(), op4Release->load());
            
            myVoice->setAlgo(algoPointer->load());

            myVoice->setOP1WAVEFORM(op1Waveform->load());
            myVoice->setOP2WAVEFORM(op2Waveform->load());
            myVoice->setOP3WAVEFORM(op3Waveform->load());
            myVoice->setOP4WAVEFORM(op4Waveform->load());
        }
    }
    controllerValMapped = jmap<float>(static_cast<float>(controllerValue), 0.0f, 127.0f, 0.0f, 1.0f);
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

    currentLevel = *(float*)stateManager.apvt.getRawParameterValue("MASTERLEVEL");
    currentLfoLevel = modLfo->getOutput(3) * 0.01;
    previousLfoLevel = currentLfoLevel + 0.7f * (previousLfoLevel - currentLfoLevel);
    currentLevel += previousLfoLevel;
    buffer.applyGainRamp(0, buffer.getNumSamples(), masterLevel, currentLevel);
    masterLevel = currentLevel;
    
    // A simple switch to only step the LFO on one channels buffer to remove the need for second for loop.
    int lfoOnceSwitch = 1;

    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        const auto* channelData = buffer.getReadPointer (channel);

        for (auto i = 0; i < buffer.getNumSamples(); ++i) {
            spectrumProcessor->pushNextSampleIntoFifo(channelData[i]);
            
            if (lfoOnceSwitch == 1) { modLfo->lfoStep(); }
        }
        lfoOnceSwitch = 0;
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
