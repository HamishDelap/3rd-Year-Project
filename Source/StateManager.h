/*
  ==============================================================================

    StateManager.h
    Created: 10 Feb 2021 3:12:44pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "dirent.h"

class ThirdYearProjectAudioProcessor;

class StateManager
{
    public:
        StateManager(AudioProcessor&);

        void writeState(juce::MemoryBlock&);
        void readState(const void*, int);

        void writePreset(String presetName);
        void readPreset(String presetName);
        StringArray getPresets();
        AudioProcessorValueTreeState apvt;
    private:
        StringArray filenames;
};


