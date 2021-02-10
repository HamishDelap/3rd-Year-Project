/*
  ==============================================================================

    StateManager.h
    Created: 10 Feb 2021 3:12:44pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ThirdYearProjectAudioProcessor;

class StateManager
{
    public:
        StateManager(AudioProcessor&);

        void readState(juce::MemoryBlock&);
        void writeState(const void*, int);
        AudioProcessorValueTreeState apvt;
    private:

};


