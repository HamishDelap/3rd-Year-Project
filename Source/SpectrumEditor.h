/*
  ==============================================================================

    SpectrumEditor.h
    Created: 12 Mar 2021 2:37:19pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SpectrumEditor
{
public:
	SpectrumEditor(int height, int width);
    void drawSpecFrame(juce::Graphics& g, ThirdYearProjectAudioProcessor& audioProcessor);
private:
    int specHeight;
    int specWidth;
    AffineTransform transform;
};

