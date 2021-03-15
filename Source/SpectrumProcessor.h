/*
  ==============================================================================

    SpectrumProcessor.h
    Created: 12 Mar 2021 2:37:07pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SpectrumProcessor
{
public:
    SpectrumProcessor();
    void pushNextSampleIntoFifo(float sample) noexcept;
    void calcNextFrameOfSpectrum();

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };
	
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];
private:
    // Spectrum Analyzer Vars
    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> windowFunction;

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
};