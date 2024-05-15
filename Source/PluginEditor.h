/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SpectrumEditor.h"

//==============================================================================
/**
*/
class ThirdYearProjectAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener, public MidiKeyboardState, juce::MidiKeyboardStateListener, public Timer, public Button::Listener, public ComboBox::Listener
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

	void timerCallback() override;

	void buttonClicked(juce::Button* button) override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	void drawAlgoDiagram(juce::Graphics& g);

	void populatePresets();

	// Taken from https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
	static float round(const float var) {
		const float value = static_cast<int>(var * 100 + .5);
		return static_cast<float>(value / 100);
	}

private:
    ThirdYearProjectAudioProcessor& audioProcessor;

	MidiKeyboardState keyboardState;
	MidiKeyboardComponent keyboardComponent;

    bool isAddingFromMidiInput = false;

	int specWidth;
	int specHeight;
	std::unique_ptr<SpectrumEditor> spectrumEditor;

	juce::Image myImage;
	juce::Image algOneImage;
	juce::Image algTwoImage;
	juce::Image algThreeImage;
	juce::Image algFourImage;

	// Algorithm Dropdown
	juce::Label algoLabel{ {}, "Algorithm" };
	juce::Font algoFont{ 12.0f };
	juce::ComboBox algoMenu;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> algoMenuAttachment;

	std::vector<juce::Slider*> testSliders = { new juce::Slider };

	//Waveform Dropdowns
	juce::Font op1WaveformFont{ 12.0f };
	juce::ComboBox op1WaveformMenu;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> op1WaveformMenuAttachment;

	juce::Font op2WaveformFont{ 12.0f };
	juce::ComboBox op2WaveformMenu;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> op2WaveformMenuAttachment;

	juce::Font op3WaveformFont{ 12.0f };
	juce::ComboBox op3WaveformMenu;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> op3WaveformMenuAttachment;

	juce::Font op4WaveformFont{ 12.0f };
	juce::ComboBox op4WaveformMenu;
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> op4WaveformMenuAttachment;

	//juce::Font lfoWaveformFont{ 12.0f };
	//juce::ComboBox lfoWaveformMenu;
	//ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> lfoWaveformMenuAttachment;

	juce::Slider lfoWaveformSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lfoWaveformSliderAttachment;

    // Mod Index Sliders
    juce::Slider op1ModIndexSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1ModIndexSliderAttachment;

	juce::Slider op2ModIndexSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2ModIndexSliderAttachment;

	juce::Slider op3ModIndexSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3ModIndexSliderAttachment;

	juce::Slider op4ModIndexSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4ModIndexSliderAttachment;

    // OP Level Sliders
	juce::Slider op1LevelSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1LevelSliderAttachment;

	juce::Slider op2LevelSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2LevelSliderAttachment;

	juce::Slider op3LevelSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3LevelSliderAttachment;

	juce::Slider op4LevelSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4LevelSliderAttachment;

    // op1 ADSR
	juce::Slider op1AttackSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1AttackSliderAttachment;
	juce::Slider op1DecaySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1DecaySliderAttachment;
	juce::Slider op1SustainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1SustainSliderAttachment;
	juce::Slider op1ReleaseSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op1ReleaseSliderAttachment;

	// op2 ADSR
	juce::Slider op2AttackSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2AttackSliderAttachment;
	juce::Slider op2DecaySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2DecaySliderAttachment;
	juce::Slider op2SustainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2SustainSliderAttachment;
	juce::Slider op2ReleaseSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op2ReleaseSliderAttachment;

	// op3 ADSR
	juce::Slider op3AttackSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3AttackSliderAttachment;
	juce::Slider op3DecaySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3DecaySliderAttachment;
	juce::Slider op3SustainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3SustainSliderAttachment;
	juce::Slider op3ReleaseSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op3ReleaseSliderAttachment;

	// op4 ADSR
	juce::Slider op4AttackSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4AttackSliderAttachment;
	juce::Slider op4DecaySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4DecaySliderAttachment;
	juce::Slider op4SustainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4SustainSliderAttachment;
	juce::Slider op4ReleaseSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> op4ReleaseSliderAttachment;

	// mod ADSR
	juce::Slider modAttackSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modAttackSliderAttachment;
	juce::Slider modDecaySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modDecaySliderAttachment;
	juce::Slider modSustainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modSustainSliderAttachment;
	juce::Slider modReleaseSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modReleaseSliderAttachment;
	juce::Slider modAmountSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> modAmountSliderAttachment;

	// LFO
	juce::Slider lfoAmountSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lfoAmountSliderAttachment;

	juce::Slider lfoFreqSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lfoFreqSliderAttachment;

	// Mod Buttons
	juce::ToggleButton lfoPitchButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> lfoPitchButtonAttachment;

	juce::ToggleButton lfoCutoffButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> lfoCutoffButtonAttachment;

	juce::ToggleButton lfoLevelButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> lfoLevelButtonAttachment;

	juce::ToggleButton envPitchButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> envPitchButtonAttachment;

	juce::ToggleButton envCutoffButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> envCutoffButtonAttachment;

	juce::ToggleButton envLevelButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> envLevelButtonAttachment;

	// Filter
	juce::Slider cutoffSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttachment;

	juce::Slider resonanceSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resonanceSliderAttachment;

	// Master Level
	juce::Slider masterLevelSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> masterLevelSliderAttachment;

	juce::Label op1ModIndexLabel;
	juce::Label op2ModIndexLabel;
	juce::Label op3ModIndexLabel;
	juce::Label op4ModIndexLabel;

	// Preset Buttons
	juce::ArrowButton nextPresetButton;
	juce::ArrowButton prevPresetButton;
	juce::TextButton savePresetButton;
	juce::ComboBox presetDropdownMenu;

	// Modwheel
	juce::ToggleButton modWheelLfoFreqButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> modWheelLfoFreqButtonAttachment;

	juce::ToggleButton modWheelLfoAmtButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> modWheelLfoAmtButtonAttachment;

	juce::ToggleButton modWheelCutoffButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> modWheelCutoffButtonAttachment;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdYearProjectAudioProcessorEditor)
};
