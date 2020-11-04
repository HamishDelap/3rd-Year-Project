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
class ThirdYearProjectAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener, public MidiKeyboardState, juce::MidiKeyboardStateListener
{
public:
    ThirdYearProjectAudioProcessorEditor (ThirdYearProjectAudioProcessor&);
    ~ThirdYearProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ThirdYearProjectAudioProcessor& audioProcessor;

	MidiKeyboardState keyboardState;
	MidiKeyboardComponent keyboardComponent;

    bool isAddingFromMidiInput = false;

    // Mod Index Sliders
    juce::Slider op1ModIndexSlider;
    juce::Label op1ModIndexLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1ModIndexSliderAttachment;

	juce::Slider op2ModIndexSlider;
	juce::Label op2ModIndexLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2ModIndexSliderAttachment;

	juce::Slider op3ModIndexSlider;
	juce::Label op3ModIndexLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3ModIndexSliderAttachment;

	juce::Slider op4ModIndexSlider;
	juce::Label op4ModIndexLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4ModIndexSliderAttachment;

    // OP Level Sliders
	juce::Slider op1LevelSlider;
	juce::Label op1LevelLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1LevelSliderAttachment;

	juce::Slider op2LevelSlider;
	juce::Label op2LevelLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2LevelSliderAttachment;

	juce::Slider op3LevelSlider;
	juce::Label op3LevelLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3LevelSliderAttachment;

	juce::Slider op4LevelSlider;
	juce::Label op4LevelLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4LevelSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessorEditor)
};
