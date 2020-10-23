/*
  ==============================================================================

    SynthSound.h
    Created: 20 Oct 2020 4:22:52pm
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>
using namespace juce;

class SynthSound : public SynthesiserSound
{
    public:
        bool appliesToNote(int /*midiNoteNumber*/) {
            return true;
        }

        bool appliesToChannel(int /*midiChannel*/) {
            return true;
        }
};