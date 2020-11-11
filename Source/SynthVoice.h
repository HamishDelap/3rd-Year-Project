/*
  ==============================================================================

    SynthVoice.h
    Created: 20 Oct 2020 4:23:03pm
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>
#include "SynthSound.h"
#include "TinySP.h"

using namespace juce;

class SynthVoice : public SynthesiserVoice
{
public:

    bool canPlaySound(SynthesiserSound* sound) {
        // If the sound can be cast to the class SynthSound it will return true, otherwise it will return false. (Taken from a tutorial)
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
	//===============================================//

    void setOP1MODINDEX(float* modindex) {
        fmTable[2][0] = *modindex;
    }

	void setOP2MODINDEX(float* modindex) {
        fmTable[2][1] = *modindex;
	}

	void setOP3MODINDEX(float* modindex) {
        fmTable[2][2] = *modindex;
	}

	void setOP4MODINDEX(float* modindex) {
        fmTable[2][3] = *modindex;
	}

	void setOP1LEVEL(float* level) {
		fmTable[1][0] = *level;
	}

	void setOP2LEVEL(float* level) {
		fmTable[1][1] = *level;
	}

	void setOP3LEVEL(float* level) {
		fmTable[1][2] = *level;
	}

	void setOP4LEVEL(float* level) {
		fmTable[1][3] = *level;
	}

    void setAmpADSR(float* attack, float* decay, float* sustain, float* release) {
        adsrParams.attack = *attack;
        adsrParams.decay = *decay;
        adsrParams.sustain = *sustain;
        adsrParams.release = *release;
    }

    void setADSRSampleRate (double sampleRate) {
        ampAdsr.setSampleRate(sampleRate);
    }

    //===============================================//

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
        // Calculate frequency of the note in Hz.
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        DBG("midiNoteNumber");
        level = velocity * 0.15;
        tailOff = 0.0;

        auto cyclesPerSecond = frequency;
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

        ampAdsr.noteOn();
    }

    //===============================================//

    void stopNote(float velocity, bool allowTailOff) {
        if (allowTailOff) {
            if (tailOff == 0.0) {
                tailOff = 1.0;
            }
        }
        else {
            clearCurrentNote();
            angleDelta = 0.0;
        }
        ampAdsr.noteOff();
    }

    void pitchWheelMoved(int newPitchWheelValue) {

    }

    void controllerMoved(int controllerNumber, int newControllerValue) {

    }

    void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
        ampAdsr.setParameters(adsrParams);
        // Written using JUCE midi synthesizer tutorial.
        if (angleDelta != 0.0) {
            // Check if note should have ended.
            if (tailOff > 0.0) {
                // Check theres samples left.
                while (--numSamples >= 0) {
                    // Calculate sample value.
                    auto currentSample = ampAdsr.getNextSample() * fmOSC(2, fmTable, angleDelta) * level * tailOff;
                    // Add sample to outputBuffer
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    // Move wave along.
                    //fmTable[0][0] += angleDelta;
                    //fmTable[0][1] = fmTable[0][0] * modIndex;
                    // Increment start sample;
                    ++startSample;

                    tailOff *= 0.9999;

                    if (tailOff <= 0.005) {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else {
                while (--numSamples >= 0) {

                    auto currentSample = ampAdsr.getNextSample() * fmOSC(2, fmTable, angleDelta) * level;
                    //auto currentSample = (float)(std::sin(fmTable[0][0] + (float)std::sin(fmTable[0][1]) * fmTable[1][1]) * fmTable[1][0]);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    //fmTable[0][0] += angleDelta;
                    //fmTable[0][1] = fmTable[0][0] * modIndex;
                    ++startSample;
                }
            }
        }
    }
    
private:
    double frequency;

    double modIndex = 3.2;

    double fmTable[3][4];

    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;

    ADSR ampAdsr;
    ADSR::Parameters adsrParams;
};