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

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
        // Calculate frequency of the note in Hz.
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        DBG(midiNoteNumber);
    }

    //===============================================//

    void stopNote(float velocity, bool allowTailOff) {
        clearCurrentNote();
    }

    void pitchWheelMoved(int newPitchWheelValue) {

    }

    void controllerMoved(int controllerNumber, int newControllerValue) {

    }

    void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {

    }
private:
    double level;
    double frequency;
};