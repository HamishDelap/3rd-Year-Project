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

    // op1 ADSR
	juce::Slider op1AttackSlider;
	juce::Label op1AttackLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1AttackSliderAttachment;
	juce::Slider op1DecaySlider;
	juce::Label op1DecayLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1DecaySliderAttachment;
	juce::Slider op1SustainSlider;
	juce::Label op1SustainLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1SustainSliderAttachment;
	juce::Slider op1ReleaseSlider;
	juce::Label op1ReleaseLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1ReleaseSliderAttachment;

	// op2 ADSR
	juce::Slider op2AttackSlider;
	juce::Label op2AttackLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2AttackSliderAttachment;
	juce::Slider op2DecaySlider;
	juce::Label op2DecayLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2DecaySliderAttachment;
	juce::Slider op2SustainSlider;
	juce::Label op2SustainLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2SustainSliderAttachment;
	juce::Slider op2ReleaseSlider;
	juce::Label op2ReleaseLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2ReleaseSliderAttachment;

	// op1 ADSR
	juce::Slider op3AttackSlider;
	juce::Label op3AttackLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3AttackSliderAttachment;
	juce::Slider op3DecaySlider;
	juce::Label op3DecayLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3DecaySliderAttachment;
	juce::Slider op3SustainSlider;
	juce::Label op3SustainLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3SustainSliderAttachment;
	juce::Slider op3ReleaseSlider;
	juce::Label op3ReleaseLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3ReleaseSliderAttachment;

	// op1 ADSR
	juce::Slider op4AttackSlider;
	juce::Label op4AttackLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4AttackSliderAttachment;
	juce::Slider op4DecaySlider;
	juce::Label op4DecayLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4DecaySliderAttachment;
	juce::Slider op4SustainSlider;
	juce::Label op4SustainLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4SustainSliderAttachment;
	juce::Slider op4ReleaseSlider;
	juce::Label op4ReleaseLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4ReleaseSliderAttachment;

	// Filter
	juce::Slider cutoffSlider;
	juce::Label cutoffLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttachment;

	juce::Slider resonanceSlider;
	juce::Label resonanceLabel;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resonanceSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessorEditor)
};
