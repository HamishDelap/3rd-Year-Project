/*
  ==============================================================================

    Operator.h
    Created: 22 Dec 2020 2:54:42pm
    Author:  hamis

  ==============================================================================
*/
#include <../JuceLibraryCode/JuceHeader.h>
#include "Oscillator.h"
#pragma once

using namespace juce;


class Operator
{
public:
    Operator(double samplerate, int waveform, std::shared_ptr<ADSR> env) {
        oscillator.reset(new Oscillator(samplerate, waveform));
        envelope = std::shared_ptr<ADSR>(env);
    }

    float operatorStep(double angle, double level) {
        float oscOutput = oscillator->oscCycleWithAngle(angle, level);
        float output = envelope->getNextSample() * oscOutput;
        return output;
    }

    void updateWaveform(int waveform) {
        oscillator->setWaveform(waveform);
    }
private:
    std::shared_ptr<ADSR> envelope;
    std::unique_ptr<Oscillator> oscillator;
};
