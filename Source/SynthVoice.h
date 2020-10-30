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
using namespace juce;

class SynthVoice : public SynthesiserVoice
{
public:

    bool canPlaySound(SynthesiserSound* sound) {
        // If the sound can be cast to the class SynthSound it will return true, otherwise it will return false.
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
	//===============================================//

    void getParam(float* modindex) {
        modIndex = *modindex;
    }

    //===============================================//

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
        // Calculate frequency of the note in Hz.
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        DBG(midiNoteNumber);

        osc1Angle = 0.0;
        osc2Angle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

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
                    auto currentSample = (float)(std::sin(osc1Angle + (float)std::sin(osc2Angle) * level) * level * tailOff);

                    // Add sample to outputBuffer
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    // Move wave along.
                    osc1Angle += angleDelta;
                    osc2Angle += angleDelta;
                    // Increment start sample;
                    ++startSample;

                    tailOff *= 0.99;

                    if (tailOff <= 0.005) {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else {
                while (--numSamples >= 0) {
                    auto currentSample = (float)(std::sin(osc1Angle + (float)std::sin(osc2Angle) * level) * level);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    osc1Angle += angleDelta;
                    osc2Angle = osc1Angle * modIndex;
                    ++startSample;
                }
            }
        }
    }
    
private:
    double frequency;

    double modIndex = 0.0;

    double osc1Angle = 0.0;
    double osc2Angle = 0.0;

    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
};