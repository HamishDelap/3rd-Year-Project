/*
  ==============================================================================

    ModEnvelope.h
    Created: 29 Dec 2020 5:32:06pm
    Author:  hamis

  ==============================================================================
*/

#include <../JuceLibraryCode/JuceHeader.h>
#include "Oscillator.h"
#pragma once

using namespace juce;


class ModEnvelope
{
public:
    ModEnvelope(double attack, double decay, double sustain, double release, double level) {
        envelope.reset(new ADSR());
        amount = level;
        envelope->setParameters(envelopeParams);
        deselectDest(1);
        deselectDest(2);
        deselectDest(3);
        deselectDest(4);

        outputs[1] = 1;
        outputs[3] = 1;

    }

    void envelopeStep() {
        envelope->setParameters(envelopeParams);
        value = envelope->getNextSample() / amount;
        for (int i = 1; i < 5; i++) {
            if (active[i] == 1) {
                outputs[i] = value;
            }
            else {
                outputs[i] = 1;
            }
        }
    }

    void noteOn() {
        envelope->noteOn();
    }

    void noteOff() {
        envelope->noteOff();
    }

    void setSampleRate(float samplerate) {
        envelope->setSampleRate(samplerate);
    }

    void setAttack(float* attack) {
        envelopeParams.attack = *attack;
    }

    void setDecay(float* decay) {
        envelopeParams.decay = *decay;
    }

    void setSustain(float* sustain) {
        envelopeParams.sustain = *sustain;
    }

    void setRelease(float* release) {
        envelopeParams.release = *release;
    }
    
    void setAmount(float* level) {
        amount = *level;
    }

    float getOutput(int destination) {
        return outputs[destination];
    }

    void toggleDest(float* toggle, int destination) {
        if (*toggle == 1.0f) {
            selectDest(destination);
        }
        else {
            deselectDest(destination);
        }
    }

    void selectDest(int destination) {
        active[destination] = 1;
    }

    void deselectDest(int destination) {
        active[destination] = 0;
        outputs[destination] = 1;
    }

    bool isActive(int destination) {
        if (active[destination] == 1)
            return true;
        else
            return false;
    }

    bool isOn() {
        return envelope->isActive();
    }

private:
    float outputs[4];
    int active[4];
    float value;
    float amount;

    std::unique_ptr<ADSR> envelope;
    ADSR::Parameters envelopeParams;
};
