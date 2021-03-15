/*
  ==============================================================================

    SpectrumProcessor.cpp
    Created: 12 Mar 2021 2:37:07pm
    Author:  hamis

  ==============================================================================
*/

#include "SpectrumProcessor.h"

SpectrumProcessor::SpectrumProcessor():fft(fftOrder), windowFunction(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    DBG("Creating Spectrum Processor...");
}

void SpectrumProcessor::pushNextSampleIntoFifo(float sample) noexcept
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
void SpectrumProcessor::calcNextFrameOfSpectrum()
{
    // Apply the windowing funtion
    windowFunction.multiplyWithWindowingTable(fftData, fftSize);

    // Apply the forwardFFT
    fft.performFrequencyOnlyForwardTransform(fftData);

    auto minLevel = -100.0f;
    auto maxLevel = 0.0f;

    for (int i = 0; i < scopeSize; i++)
    {
        auto skewedXAxis = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedXAxis * (float)fftSize * 0.5f));
        auto currentLevel = jmap(jlimit(minLevel, maxLevel, Decibels::gainToDecibels(fftData[fftDataIndex]) -
            Decibels::gainToDecibels((float)fftSize)), minLevel, maxLevel, 0.0f, 1.0f);
        scopeData[i] = currentLevel;
    }

}