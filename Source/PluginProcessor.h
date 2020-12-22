/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

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


    void pushNextSampleIntoFifo(float sample) noexcept;

    void drawNextFrameOfSpectrum();

    AudioProcessorValueTreeState apvt;

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];

    MidiKeyboardState keyboardState;

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

    Oscillator *lfo;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessor)

};
