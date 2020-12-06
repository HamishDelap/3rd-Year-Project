/*
  ==============================================================================

    Slider.h
    Created: 4 Dec 2020 10:04:32am
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>

using namespace juce;

class VerticalSlider : public Component, public Slider::Listener {
public:
    VerticalSlider(String paramID, String labelText, String suffix, AudioProcessorValueTreeState apvt) {
		addAndMakeVisible(slider);
		slider.setRange(0, 12);
		slider.setTextValueSuffix(suffix);
		slider.addListener(this);
		slider.setSliderStyle(juce::Slider::Rotary);
		juce::Slider::TextEntryBoxPosition noDisplay = juce::Slider::TextEntryBoxPosition::NoTextBox;
		slider.setTextBoxStyle(noDisplay, false, 1, 1);
		addAndMakeVisible(label);
		label.setText(labelText, juce::dontSendNotification);
		sliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(apvt, paramID, slider);
    }

	void sliderValueChanged(Slider* slider) {};
private:
    juce::Slider slider;
    juce::Label label;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
};