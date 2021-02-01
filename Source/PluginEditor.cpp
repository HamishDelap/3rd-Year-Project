/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "VerticalSlider.h"

//==============================================================================
ThirdYearProjectAudioProcessorEditor::ThirdYearProjectAudioProcessorEditor (ThirdYearProjectAudioProcessor& p)
	: AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{

	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize (1600, 800);

	myImage = ImageFileFormat::loadFrom(File("C:/Users/hamis/Documents/3rd Year Project/3rd-Year-Project/Assets/bg_1.png"));

	startTimerHz(5);

	// Keyboard
	addAndMakeVisible(keyboardComponent);
	audioProcessor.keyboardState.addListener(this);
	keyboardComponent.setAvailableRange(0, 127);

	specWidth = getLocalBounds().getWidth();
	specHeight = getLocalBounds().getHeight() / 4 - 65;

	addAndMakeVisible(algoLabel);
	algoLabel.setFont(algoFont);

	// add items to the combo-box
	addAndMakeVisible(algoMenu);
	algoMenu.addItem("op1(op2(op3(op4)))", 1);
	algoMenu.addItem("op1(op2(op3 + op4))", 2);
	algoMenu.addItem("op1(op2 + op3 + op4)", 3);
	algoMenu.addItem("op1 + op2 + op3 + op4", 4);

	algoLabel.attachToComponent(&algoMenu, false);
	algoMenu.setSelectedId(1);

	algoMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.apvt, "ALGO", algoMenu);


	// Op1 Waveform Dropdown
	// add items to the combo-box
	addAndMakeVisible(op1WaveformMenu);
	op1WaveformMenu.addItem("Sine", 1);
	op1WaveformMenu.addItem("Triangle", 2);
	op1WaveformMenu.addItem("Square", 3);
	op1WaveformMenu.setSelectedId(1);
	op1WaveformMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.apvt, "OP1WAVEFORM", op1WaveformMenu);

	// Op2 Waveform Dropdown
	// add items to the combo-box
	addAndMakeVisible(op2WaveformMenu);
	op2WaveformMenu.addItem("Sine", 1);
	op2WaveformMenu.addItem("Triangle", 2);
	op2WaveformMenu.addItem("Square", 3);
	op2WaveformMenu.setSelectedId(1);
	op2WaveformMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.apvt, "OP2WAVEFORM", op2WaveformMenu);

	// Op3 Waveform Dropdown
	// add items to the combo-box
	addAndMakeVisible(op3WaveformMenu);
	op3WaveformMenu.addItem("Triangle", 2);
	op3WaveformMenu.addItem("Square", 3);
	op3WaveformMenu.addItem("Sine", 1);
	op3WaveformMenu.setSelectedId(1);
	op3WaveformMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.apvt, "OP3WAVEFORM", op3WaveformMenu);

	// Op4 Waveform Dropdown
	// add items to the combo-box
	addAndMakeVisible(op4WaveformMenu);
	op4WaveformMenu.addItem("Sine", 1);
	op4WaveformMenu.addItem("Triangle", 2);
	op4WaveformMenu.addItem("Square", 3);
	op4WaveformMenu.setSelectedId(1);
	op4WaveformMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.apvt, "OP4WAVEFORM", op4WaveformMenu);


	juce::Slider::TextEntryBoxPosition noDisplay = juce::Slider::TextEntryBoxPosition::NoTextBox;

	//Mod Index Sliders
	//Adding slider
	addAndMakeVisible(op1ModIndexSlider);
	op1ModIndexSlider.setRange(0, 12);
	op1ModIndexSlider.setTextValueSuffix("x");
	op1ModIndexSlider.addListener(this);
	op1ModIndexSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1ModIndexSlider.setSliderStyle(juce::Slider::Rotary);
	op1ModIndexSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1MODINDEX", op1ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op2ModIndexSlider);
	op2ModIndexSlider.setRange(0, 12);
	op2ModIndexSlider.setTextValueSuffix("x");
	op2ModIndexSlider.addListener(this);
	op2ModIndexSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2ModIndexSlider.setSliderStyle(juce::Slider::Rotary);
	op2ModIndexSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2MODINDEX", op2ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op3ModIndexSlider);
	op3ModIndexSlider.setRange(0, 12);
	op3ModIndexSlider.setTextValueSuffix("x");
	op3ModIndexSlider.addListener(this);
	op3ModIndexSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3ModIndexSlider.setSliderStyle(juce::Slider::Rotary);
	op3ModIndexSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3MODINDEX", op3ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op4ModIndexSlider);
	op4ModIndexSlider.setRange(0, 12);
	op4ModIndexSlider.setTextValueSuffix("x");
	op4ModIndexSlider.addListener(this);
	op4ModIndexSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4ModIndexSlider.setSliderStyle(juce::Slider::Rotary);
	op4ModIndexSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4MODINDEX", op4ModIndexSlider);

	// Level Sliders

	// Adding slider
	addAndMakeVisible(op1LevelSlider);
	op1LevelSlider.setRange(0, 12);
	op1LevelSlider.setTextValueSuffix("x");
	op1LevelSlider.addListener(this);
	op1LevelSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1LevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op1LevelSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1LEVEL", op1LevelSlider);

	// Adding slider
	addAndMakeVisible(op2LevelSlider);
	op2LevelSlider.setRange(0, 12);
	op2LevelSlider.setTextValueSuffix("x");
	op2LevelSlider.addListener(this);
	op2LevelSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2LevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op2LevelSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2LEVEL", op2LevelSlider);

	// Adding slider
	addAndMakeVisible(op3LevelSlider);
	op3LevelSlider.setRange(0, 12);
	op3LevelSlider.setTextValueSuffix("x");
	op3LevelSlider.addListener(this);
	op3LevelSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3LevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op3LevelSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3LEVEL", op3LevelSlider);

	// Adding slider
	addAndMakeVisible(op4LevelSlider);
	op4LevelSlider.setRange(0, 12);
	op4LevelSlider.setTextValueSuffix("x");
	op4LevelSlider.addListener(this);
	op4LevelSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4LevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op4LevelSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4LEVEL", op4LevelSlider);

	// op1 ADSR
	// Adding slider
	addAndMakeVisible(op1AttackSlider);
	op1AttackSlider.setRange(0, 12);
	op1AttackSlider.setTextValueSuffix("s");
	op1AttackSlider.addListener(this);
	op1AttackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1AttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op1AttackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1ATTACK", op1AttackSlider);
	// Adding slider
	addAndMakeVisible(op1DecaySlider);
	op1DecaySlider.setRange(0, 12);
	op1DecaySlider.setTextValueSuffix("s");
	op1DecaySlider.addListener(this);
	op1DecaySlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1DecaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op1DecaySlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1DECAY", op1DecaySlider);
	// Adding slider
	addAndMakeVisible(op1SustainSlider);
	op1SustainSlider.setRange(0, 12);
	op1SustainSlider.setTextValueSuffix("dB");
	op1SustainSlider.addListener(this);
	op1SustainSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1SustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op1SustainSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1SUSTAIN", op1SustainSlider);
	// Adding slider
	addAndMakeVisible(op1ReleaseSlider);
	op1ReleaseSlider.setRange(0, 12);
	op1ReleaseSlider.setTextValueSuffix("s");
	op1ReleaseSlider.addListener(this);
	op1ReleaseSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op1ReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op1ReleaseSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op1ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1RELEASE", op1ReleaseSlider);

	// op2 ADSR
	// Adding slider
	addAndMakeVisible(op2AttackSlider);
	op2AttackSlider.setRange(0, 12);
	op2AttackSlider.setTextValueSuffix("s");
	op2AttackSlider.addListener(this);
	op2AttackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2AttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op2AttackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2ATTACK", op2AttackSlider);
	// Adding slider
	addAndMakeVisible(op2DecaySlider);
	op2DecaySlider.setRange(0, 12);
	op2DecaySlider.setTextValueSuffix("s");
	op2DecaySlider.addListener(this);
	op2DecaySlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2DecaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op2DecaySlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2DECAY", op2DecaySlider);
	// Adding slider
	addAndMakeVisible(op2SustainSlider);
	op2SustainSlider.setRange(0, 12);
	op2SustainSlider.setTextValueSuffix("dB");
	op2SustainSlider.addListener(this);
	op2SustainSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2SustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op2SustainSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2SUSTAIN", op2SustainSlider);
	// Adding slider
	addAndMakeVisible(op2ReleaseSlider);
	op2ReleaseSlider.setRange(0, 12);
	op2ReleaseSlider.setTextValueSuffix("s");
	op2ReleaseSlider.addListener(this);
	op2ReleaseSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op2ReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op2ReleaseSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op2ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2RELEASE", op2ReleaseSlider);

	// op3 ADSR
// Adding slider
	addAndMakeVisible(op3AttackSlider);
	op3AttackSlider.setRange(0, 12);
	op3AttackSlider.setTextValueSuffix("s");
	op3AttackSlider.addListener(this);
	op3AttackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3AttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op3AttackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3ATTACK", op3AttackSlider);
	// Adding slider
	addAndMakeVisible(op3DecaySlider);
	op3DecaySlider.setRange(0, 12);
	op3DecaySlider.setTextValueSuffix("s");
	op3DecaySlider.addListener(this);
	op3DecaySlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3DecaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op3DecaySlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3DECAY", op3DecaySlider);
	// Adding slider
	addAndMakeVisible(op3SustainSlider);
	op3SustainSlider.setRange(0, 12);
	op3SustainSlider.setTextValueSuffix("dB");
	op3SustainSlider.addListener(this);
	op3SustainSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3SustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op3SustainSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3SUSTAIN", op3SustainSlider);
	// Adding slider
	addAndMakeVisible(op3ReleaseSlider);
	op3ReleaseSlider.setRange(0, 12);
	op3ReleaseSlider.setTextValueSuffix("s");
	op3ReleaseSlider.addListener(this);
	op3ReleaseSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op3ReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op3ReleaseSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op3ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3RELEASE", op3ReleaseSlider);

	// op4 ADSR
// Adding slider
	addAndMakeVisible(op4AttackSlider);
	op4AttackSlider.setRange(0, 12);
	op4AttackSlider.setTextValueSuffix("s");
	op4AttackSlider.addListener(this);
	op4AttackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4AttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op4AttackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4ATTACK", op4AttackSlider);
	// Adding slider
	addAndMakeVisible(op4DecaySlider);
	op4DecaySlider.setRange(0, 12);
	op4DecaySlider.setTextValueSuffix("s");
	op4DecaySlider.addListener(this);
	op4DecaySlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4DecaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op4DecaySlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4DECAY", op4DecaySlider);
	// Adding slider
	addAndMakeVisible(op4SustainSlider);
	op4SustainSlider.setRange(0, 12);
	op4SustainSlider.setTextValueSuffix("dB");
	op4SustainSlider.addListener(this);
	op4SustainSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4SustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op4SustainSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4SUSTAIN", op4SustainSlider);
	// Adding slider
	addAndMakeVisible(op4ReleaseSlider);
	op4ReleaseSlider.setRange(0, 12);
	op4ReleaseSlider.setTextValueSuffix("s");
	op4ReleaseSlider.addListener(this);
	op4ReleaseSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	op4ReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	op4ReleaseSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	op4ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4RELEASE", op4ReleaseSlider);

	// mod ADSR
// Adding slider
	addAndMakeVisible(modAttackSlider);
	modAttackSlider.setRange(0, 12);
	modAttackSlider.setTextValueSuffix("s");
	modAttackSlider.addListener(this);
	modAttackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	modAttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	modAttackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	modAttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODENVATTACK", modAttackSlider);
	// Adding slider
	addAndMakeVisible(modDecaySlider);
	modDecaySlider.setRange(0, 12);
	modDecaySlider.setTextValueSuffix("s");
	modDecaySlider.addListener(this);
	modDecaySlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	modDecaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	modDecaySlider.setTextBoxStyle(noDisplay, false, 1, 1);
	modDecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODENVDECAY", modDecaySlider);
	// Adding slider
	addAndMakeVisible(modSustainSlider);
	modSustainSlider.setRange(0, 12);
	modSustainSlider.setTextValueSuffix("dB");
	modSustainSlider.addListener(this);
	modSustainSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	modSustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	modSustainSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	modSustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODENVSUSTAIN", modSustainSlider);
	// Adding slider
	addAndMakeVisible(modReleaseSlider);
	modReleaseSlider.setRange(0, 12);
	modReleaseSlider.setTextValueSuffix("s");
	modReleaseSlider.addListener(this);
	modReleaseSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 74, 97));
	modReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	modReleaseSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	modReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODENVRELEASE", modReleaseSlider);
	// Adding slider
	addAndMakeVisible(modAmountSlider);
	modAmountSlider.setRange(0, 12);
	modAmountSlider.setTextValueSuffix("x");
	modAmountSlider.addListener(this);
	modAmountSlider.setSliderStyle(juce::Slider::Rotary);
	modAmountSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	modAmountSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODENVAMOUNT", modAmountSlider);

	// LFO
	// Adding slider
	addAndMakeVisible(lfoAmountSlider);
	lfoAmountSlider.setRange(0, 12);
	lfoAmountSlider.setTextValueSuffix("x");
	lfoAmountSlider.addListener(this);
	lfoAmountSlider.setSliderStyle(juce::Slider::Rotary);
	lfoAmountSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	lfoAmountSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "LFOAMOUNT", lfoAmountSlider);

	// Adding slider
	addAndMakeVisible(lfoFreqSlider);
	lfoFreqSlider.setRange(0, 12);
	lfoFreqSlider.setTextValueSuffix("Hz");
	lfoFreqSlider.addListener(this);
	lfoFreqSlider.setSliderStyle(juce::Slider::Rotary);
	lfoFreqSlider.hideTextBox(true);
	lfoFreqSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	lfoFreqSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "LFOFREQ", lfoFreqSlider);


	// Filter
	// Adding slider
	addAndMakeVisible(cutoffSlider);
	cutoffSlider.setRange(0, 12);
	cutoffSlider.setTextValueSuffix("s");
	cutoffSlider.addListener(this);
	cutoffSlider.setSliderStyle(juce::Slider::Rotary);
	cutoffSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	cutoffSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "CUTOFF", cutoffSlider);

	// Adding slider
	addAndMakeVisible(resonanceSlider);
	resonanceSlider.setRange(0, 12);
	resonanceSlider.setTextValueSuffix("s");
	resonanceSlider.addListener(this);
	resonanceSlider.setSliderStyle(juce::Slider::Rotary);
	resonanceSlider.hideTextBox(true);
	resonanceSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	resonanceSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "RESONANCE", resonanceSlider);

	// Adding slider
	addAndMakeVisible(masterLevelSlider);
	masterLevelSlider.setRange(0, 12);
	masterLevelSlider.setTextValueSuffix("s");
	masterLevelSlider.addListener(this);
	masterLevelSlider.setSliderStyle(juce::Slider::Rotary);
	masterLevelSlider.hideTextBox(true);
	masterLevelSlider.setTextBoxStyle(noDisplay, false, 1, 1);
	masterLevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MASTERLEVEL", masterLevelSlider);

	addAndMakeVisible(lfoPitchButton);
	lfoPitchButtonAttachment = new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.apvt, "LFOPITCH", lfoPitchButton);

	addAndMakeVisible(envPitchButton);
	envPitchButtonAttachment = new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.apvt, "MODENVPITCH", envPitchButton);

	addAndMakeVisible(lfoCutoffButton);
	lfoCutoffButtonAttachment = new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.apvt, "LFOFILTER", lfoCutoffButton);

	addAndMakeVisible(envCutoffButton);
	envCutoffButtonAttachment = new AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.apvt, "MODENVFILTER", envCutoffButton);

	addAndMakeVisible(op1ModIndexLabel);
	op1ModIndexLabel.setFont(juce::Font(24.0f, juce::Font::bold));
	op1ModIndexLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	addAndMakeVisible(op2ModIndexLabel);
	op2ModIndexLabel.setFont(juce::Font(24.0f, juce::Font::bold));
	op2ModIndexLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	addAndMakeVisible(op3ModIndexLabel);
	op3ModIndexLabel.setFont(juce::Font(24.0f, juce::Font::bold));
	op3ModIndexLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
	addAndMakeVisible(op4ModIndexLabel);
	op4ModIndexLabel.setFont(juce::Font(24.0f, juce::Font::bold));
	op4ModIndexLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
}

ThirdYearProjectAudioProcessorEditor::~ThirdYearProjectAudioProcessorEditor()
{
}

//==============================================================================
void ThirdYearProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll ();

	g.drawImageAt(myImage, 0, 0);

	g.setColour (juce::Colours::white);
	g.setFont (15.0f);

	drawSpecFrame(g);
}

void ThirdYearProjectAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	auto sliderLeft = 120;

	algoMenu.setBounds(sliderLeft + 773, 140, getWidth() / 8, 20);


	op1WaveformMenu.setBounds(65, 50, getWidth() / 16, 20);
	op1LevelSlider.setBounds(217, 100, 20, 95);
	op1ModIndexSlider.setBounds(sliderLeft + 145, 100, 60, 60);
	op1ModIndexLabel.setBounds(sliderLeft + 201, 113, 90, 30);

	op1AttackSlider.setBounds(70, 100, 20, 95);
	op1DecaySlider.setBounds(101, 100, 20, 95);
	op1SustainSlider.setBounds(134, 100, 20, 95);
	op1ReleaseSlider.setBounds(166, 100, 20, 95);

	auto secondColumn = 386;

	op2WaveformMenu.setBounds(secondColumn + 67, 50, getWidth() / 10, 20);
	op2LevelSlider.setBounds(secondColumn + 217, 100, 20, 95);
	op2ModIndexSlider.setBounds(secondColumn + sliderLeft + 145, 100, 60, 60);
	op2ModIndexLabel.setBounds(secondColumn + sliderLeft + 201, 113, 90, 30);

	op2AttackSlider.setBounds(secondColumn + 70, 100, 20, 95);
	op2DecaySlider.setBounds(secondColumn + 101, 100, 20, 95);
	op2SustainSlider.setBounds(secondColumn + 134, 100, 20, 95);
	op2ReleaseSlider.setBounds(secondColumn + 166, 100, 20, 95);

	auto secondRow = 213;

	op3WaveformMenu.setBounds(65, secondRow + 50, getWidth() / 10, 20);
	op3LevelSlider.setBounds(217, secondRow + 100, 20, 95);
	op3ModIndexSlider.setBounds(sliderLeft + 145, secondRow + 100, 60, 60);
	op3ModIndexLabel.setBounds(sliderLeft + 201, secondRow + 113, 90, 30);

	op3AttackSlider.setBounds(70, secondRow + 100, 20, 95);
	op3DecaySlider.setBounds(101, secondRow + 100, 20, 95);
	op3SustainSlider.setBounds(134, secondRow + 100, 20, 95);
	op3ReleaseSlider.setBounds(166, secondRow + 100, 20, 95);

	op4WaveformMenu.setBounds(secondColumn + 66, secondRow + 50, getWidth() / 10, 20);
	op4LevelSlider.setBounds(secondColumn + 217, secondRow + 100, 20, 95);
	op4ModIndexSlider.setBounds(secondColumn + sliderLeft + 145, secondRow + 100, 60, 60);
	op4ModIndexLabel.setBounds(secondColumn + sliderLeft + 201, secondRow + 113, 90, 30);

	op4AttackSlider.setBounds(secondColumn + 70, secondRow + 100, 20, 95);
	op4DecaySlider.setBounds(secondColumn + 101, secondRow + 100, 20, 95);
	op4SustainSlider.setBounds(secondColumn + 134, secondRow + 100, 20, 95);
	op4ReleaseSlider.setBounds(secondColumn + 166, secondRow + 100, 20, 95);


	modAttackSlider.setBounds((getWidth() / 4) * 3 + 76, 296, 20, 95);
	modDecaySlider.setBounds((getWidth() / 4) * 3 + 101, 296, 20, 95);
	modSustainSlider.setBounds((getWidth() / 4) * 3 + 126, 296, 20, 95);
	modReleaseSlider.setBounds((getWidth() / 4) * 3 + 151, 296, 20, 95);

	modAmountSlider.setBounds((getWidth() / 4) * 3 + 176, 310, 60, 60);

	lfoAmountSlider.setBounds((getWidth() / 4) * 3 + 173, 229, 60, 60);
	lfoFreqSlider.setBounds((getWidth() / 4) * 3 + 90, 229, 60, 60);


	cutoffSlider.setBounds((getWidth() / 4) * 3 + 75, 130, 60, 60);
	resonanceSlider.setBounds((getWidth() / 4) * 3 + 155, 130, 60, 60);
	masterLevelSlider.setBounds((getWidth() / 4) * 3 + 235, 130, 60, 60);

	lfoPitchButton.setBounds((getWidth() / 4) * 3 + 290, 240, 22, 22);
	envPitchButton.setBounds((getWidth() / 4) * 3 + 325, 240, 22, 22);

	lfoCutoffButton.setBounds((getWidth() / 4) * 3 + 290, 260, 22, 22);
	envCutoffButton.setBounds((getWidth() / 4) * 3 + 325, 260, 22, 22);

	int w = (int)keyboardComponent.getKeyWidth() * (7 * 10 + 5), h = 100;
	keyboardComponent.setSize(w, h);
	keyboardComponent.setCentrePosition(getWidth() / 2, getHeight() - 50);
}

void ThirdYearProjectAudioProcessorEditor::drawSpecFrame(Graphics& g)
{

	AffineTransform transform = AffineTransform::translation((float)0, (float)specHeight + 315);

	for (int i = 1; i < audioProcessor.scopeSize; ++i)
	{
		Line<float> line ((float)jmap(i - 1, 0, audioProcessor.scopeSize - 1, 0, specWidth),
												  jmap(audioProcessor.scopeData[i - 1], 0.0f, 1.0f, (float)(specHeight * 1.5), 0.0f),
										   (float)jmap(i, 0, audioProcessor.scopeSize - 1, 0, specWidth),
												  jmap(audioProcessor.scopeData[i], 0.0f, 1.0f, (float)(specHeight * 1.5), 0.0f));

		line.applyTransform(transform);
		g.drawLine(line);
	}
}

void ThirdYearProjectAudioProcessorEditor::sliderValueChanged(Slider* slider) {

}

void ThirdYearProjectAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
	midiChannel = 1;
	juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);

}

void ThirdYearProjectAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
	juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
}

void ThirdYearProjectAudioProcessorEditor::timerCallback()
{
	std::string op1ModIndexLabelText = std::to_string(round(*audioProcessor.apvt.getRawParameterValue("OP1MODINDEX")));
	std::string op2ModIndexLabelText = std::to_string(round(*audioProcessor.apvt.getRawParameterValue("OP2MODINDEX")));
	std::string op3ModIndexLabelText = std::to_string(round(*audioProcessor.apvt.getRawParameterValue("OP3MODINDEX")));
	std::string op4ModIndexLabelText = std::to_string(round(*audioProcessor.apvt.getRawParameterValue("OP4MODINDEX")));
	// Set Mod Index Values
	op1ModIndexLabel.setText(op1ModIndexLabelText.substr(0, op1ModIndexLabelText.size() - 3), juce::dontSendNotification);
	op2ModIndexLabel.setText(op2ModIndexLabelText.substr(0, op2ModIndexLabelText.size() - 3), juce::dontSendNotification);
	op3ModIndexLabel.setText(op3ModIndexLabelText.substr(0, op3ModIndexLabelText.size() - 3), juce::dontSendNotification);
	op4ModIndexLabel.setText(op4ModIndexLabelText.substr(0, op4ModIndexLabelText.size() - 3), juce::dontSendNotification);

	if (audioProcessor.nextFFTBlockReady) {
		audioProcessor.drawNextFrameOfSpectrum();
		audioProcessor.nextFFTBlockReady = false;
		repaint();
	}
}