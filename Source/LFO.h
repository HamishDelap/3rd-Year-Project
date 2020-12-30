/*
  ==============================================================================

    LFO.h
    Created: 29 Dec 2020 4:56:48pm
    Author:  hamis

  ==============================================================================
*/

#include <../JuceLibraryCode/JuceHeader.h>
#include "Oscillator.h"
#pragma once

using namespace juce;


class Lfo
{
public:
    Lfo(double samplerate, int waveform, double freq, double level) {
        oscillator.reset(new Oscillator(samplerate, waveform));
        frequency = freq;
        level = level;
        deselectDest(1);
        deselectDest(2);
        deselectDest(3);
        deselectDest(4);
    }

    void lfoStep() {
        value = amount * oscillator->oscCycleWithFreq(frequency, 10);
        for (int i = 1; i < 5; i++) {
            if (active[i] == 1)
                outputs[i] = value;
            else
                outputs[i] = 0;
        }
    }

    void setWaveform(int waveform) {
        oscillator->setWaveform(waveform);
    }

    void setFrequency(float* freq) {
        frequency = *freq;
    }

    void setLevel(float* level) {
        amount = *level;
    }

    float getOutput(int destination) {
        return outputs[destination];
    }

    void toggleDest(float* toggle, int destination) {
        DBG(*toggle);
        if (*toggle == 1.0f) {
            selectDest(destination);
        }
        else {
            deselectDest(destination);
        }
    }

    void selectDest(int destination) {
        active[destination] = 1;
        outputs[destination] = value;
    }

    void deselectDest(int destination) {
        active[destination] = 0;
        outputs[destination] = 0;
    }

    bool isActive(int destination) {
        if (active[destination] == 1)
            return true;
        else
            return false;
    }

private:
    float outputs[4];
    int active[4];
    double frequency;
    double amount;
    float value;
    
    std::unique_ptr<Oscillator> oscillator;
};
