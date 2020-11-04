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

    void setMODINDEX(float* modindex) {
        modIndex = *modindex;
    }

	void setOP1Level(float* level) {
		fmTable[1][0] = *level;
	}

	void setOP2Level(float* level) {
		fmTable[1][1] = *level;
	}


    //===============================================//

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
        // Calculate frequency of the note in Hz.
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        DBG(midiNoteNumber);

      //  fmTable[0][0] = 0.0;
      //  fmTable[0][1] = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

       // fmTable[1][0] = level;
       // fmTable[1][1] = level;

        auto cyclesPerSecond = frequency;
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
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
    }

    void pitchWheelMoved(int newPitchWheelValue) {

    }

    void controllerMoved(int controllerNumber, int newControllerValue) {

    }

    void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
        // Written using JUCE midi synthesizer tutorial.
        if (angleDelta != 0.0) {
            // Check if note should have ended.
            if (tailOff > 0.0) {
                // Check theres samples left.
                while (--numSamples >= 0) {
                    // Calculate sample value.
                    auto currentSample = fmOSC(1, fmTable) * level * tailOff;
                    // Add sample to outputBuffer
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    // Move wave along.
                    fmTable[0][0] += angleDelta;
                    fmTable[0][1] = fmTable[0][0] * modIndex;
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

                    auto currentSample = fmOSC(1,fmTable) * level; 
                    //auto currentSample = (float)(std::sin(fmTable[0][0] + (float)std::sin(fmTable[0][1]) * fmTable[1][1]) * fmTable[1][0]);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    fmTable[0][0] += angleDelta;
                    fmTable[0][1] = fmTable[0][0] * modIndex;
                    ++startSample;
                }
            }
        }
    }
    
private:
    double frequency;

    double modIndex = 3.2;

    double fmTable[2][4];

    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
};