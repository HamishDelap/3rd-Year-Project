/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ThirdYearProjectAudioProcessorEditor : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    ThirdYearProjectAudioProcessorEditor (ThirdYearProjectAudioProcessor&);
    ~ThirdYearProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ThirdYearProjectAudioProcessor& audioProcessor;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modIndexSliderAttachment;

    juce::Slider modIndexSlider;
    juce::Label modIndexLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessorEditor)
};
