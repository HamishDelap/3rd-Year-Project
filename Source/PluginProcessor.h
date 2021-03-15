/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "LFO.h"
#include "ModEnvelope.h"
#include "StateManager.h"
#include "SpectrumProcessor.h"
#include "ModWheelController.h"

class StateManager;

//==============================================================================
/**
*/
class ThirdYearProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ThirdYearProjectAudioProcessor();
    ~ThirdYearProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateFilter();

    void checkAlgoChanged();

    void checkPresetChanged();

    StateManager stateManager; // <-- Unique pointer??s

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };
    bool nextFFTBlockReady = false;
    bool algoChanged = true;
    float scopeData[scopeSize];

    MidiKeyboardState keyboardState;

    std::unique_ptr<SpectrumProcessor> spectrumProcessor;

    float preset = 1.0f;
    float lastPreset = 0.0f;
private:

    Synthesiser mySynth;
    SynthVoice* myVoice;

    MidiMessageCollector midiCollector;

    double lastSampleRate;

    dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter;

    // Spectrum Analyzer Vars
    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> windowFunction;

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;

    float currentLfoLevel = 0;
    float algo = 1.0;

    double currentCutoff = 1000;
    
    float masterLevel = 1;
    float currentLevel = 1;

    float smoothingCoeff = 0.8f;
    float previousLfoLevel = 0.0;

    std::shared_ptr<ModEnvelope> modEnvelope;
    std::shared_ptr<Lfo> modLfo;

    int controllerValue = 127;
    float controllerValMapped = 1.0f;

    std::shared_ptr<ModWheelController> modWheelController;

	// Parameter Pointers
    std::atomic<float>* lfoFreq;
    std::atomic<float>* lfoAmount;
    std::atomic<float>* lfoWaveform;
    std::atomic<float>* lfoPitch;
    std::atomic<float>* lfoFilter;
    std::atomic<float>* lfoLevel;

    std::atomic<float>* modEnvAttack;
    std::atomic<float>* modEnvDecay;
    std::atomic<float>* modEnvSustain;
    std::atomic<float>* modEnvRelease;
    std::atomic<float>* modEnvAmount;
    std::atomic<float>* modEnvPitch;
    std::atomic<float>* modEnvFilter;
    std::atomic<float>* modEnvLevel;

    std::atomic<float>* op1ModIndex;
    std::atomic<float>* op2ModIndex;
    std::atomic<float>* op3ModIndex;
    std::atomic<float>* op4ModIndex;

    std::atomic<float>* op1Level;
    std::atomic<float>* op2Level;
    std::atomic<float>* op3Level;
    std::atomic<float>* op4Level;

    std::atomic<float>* op1Attack;
    std::atomic<float>* op1Decay;
    std::atomic<float>* op1Sustain;
    std::atomic<float>* op1Release;

    std::atomic<float>* op2Attack;
    std::atomic<float>* op2Decay;
    std::atomic<float>* op2Sustain;
    std::atomic<float>* op2Release;

    std::atomic<float>* op3Attack;
    std::atomic<float>* op3Decay;
    std::atomic<float>* op3Sustain;
    std::atomic<float>* op3Release;

    std::atomic<float>* op4Attack;
    std::atomic<float>* op4Decay;
    std::atomic<float>* op4Sustain;
    std::atomic<float>* op4Release;
	
    std::atomic<float>* algoPointer;

    std::atomic<float>* op1Waveform;
    std::atomic<float>* op2Waveform;
    std::atomic<float>* op3Waveform;
    std::atomic<float>* op4Waveform;

    std::atomic<float>* modWheelLfoFreq;
    std::atomic<float>* modWheelLfoAmt;
    std::atomic<float>* modWheelCutoff;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessor)

};
