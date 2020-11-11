/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThirdYearProjectAudioProcessorEditor::ThirdYearProjectAudioProcessorEditor (ThirdYearProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

	// Mod Index Sliders

    // Adding slider
    addAndMakeVisible(op1ModIndexSlider);
    op1ModIndexSlider.setRange(0, 12);
    op1ModIndexSlider.setTextValueSuffix("x");
    op1ModIndexSlider.addListener(this);
    // Adding label
    addAndMakeVisible(op1ModIndexLabel);
    op1ModIndexLabel.setText("Op1 Mod Index", juce::dontSendNotification);
    op1ModIndexLabel.attachToComponent(&op1ModIndexSlider, true);
    op1ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1MODINDEX", op1ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op2ModIndexSlider);
	op2ModIndexSlider.setRange(0, 12);
	op2ModIndexSlider.setTextValueSuffix("x");
	op2ModIndexSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2ModIndexLabel);
	op2ModIndexLabel.setText("Op2 Mod Index", juce::dontSendNotification);
	op2ModIndexLabel.attachToComponent(&op2ModIndexSlider, true);
	op2ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2MODINDEX", op2ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op3ModIndexSlider);
	op3ModIndexSlider.setRange(0, 12);
	op3ModIndexSlider.setTextValueSuffix("x");
	op3ModIndexSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3ModIndexLabel);
	op3ModIndexLabel.setText("Op3 Mod Index", juce::dontSendNotification);
	op3ModIndexLabel.attachToComponent(&op3ModIndexSlider, true);
	op3ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3MODINDEX", op3ModIndexSlider);

	// Adding slider
	addAndMakeVisible(op4ModIndexSlider);
	op4ModIndexSlider.setRange(0, 12);
	op4ModIndexSlider.setTextValueSuffix("x");
	op4ModIndexSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4ModIndexLabel);
	op4ModIndexLabel.setText("Op4 Mod Index", juce::dontSendNotification);
	op4ModIndexLabel.attachToComponent(&op4ModIndexSlider, true);
	op4ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4MODINDEX", op4ModIndexSlider);

	// Level Sliders

	// Adding slider
	addAndMakeVisible(op1LevelSlider);
	op1LevelSlider.setRange(0, 12);
	op1LevelSlider.setTextValueSuffix("x");
	op1LevelSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op1LevelLabel);
	op1LevelLabel.setText("OP1 Level", juce::dontSendNotification);
	op1LevelLabel.attachToComponent(&op1LevelSlider, true);
	op1LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1LEVEL", op1LevelSlider);

	// Adding slider
	addAndMakeVisible(op2LevelSlider);
    op2LevelSlider.setRange(0, 12);
    op2LevelSlider.setTextValueSuffix("x");
    op2LevelSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2LevelLabel);
    op2LevelLabel.setText("OP2 Level", juce::dontSendNotification);
    op2LevelLabel.attachToComponent(&op2LevelSlider, true);
	op2LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2LEVEL", op2LevelSlider);

	// Adding slider
	addAndMakeVisible(op3LevelSlider);
	op3LevelSlider.setRange(0, 12);
	op3LevelSlider.setTextValueSuffix("x");
	op3LevelSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3LevelLabel);
	op3LevelLabel.setText("OP3 Level", juce::dontSendNotification);
	op3LevelLabel.attachToComponent(&op3LevelSlider, true);
	op3LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3LEVEL", op3LevelSlider);

	// Adding slider
	addAndMakeVisible(op4LevelSlider);
	op4LevelSlider.setRange(0, 12);
	op4LevelSlider.setTextValueSuffix("x");
	op4LevelSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4LevelLabel);
	op4LevelLabel.setText("OP4 Level", juce::dontSendNotification);
	op4LevelLabel.attachToComponent(&op4LevelSlider, true);
	op4LevelSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4LEVEL", op4LevelSlider);

	// Keyboard
	addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);
	keyboardComponent.setAvailableRange(21, 108);

	// Amp ADSR
	
	// Adding slider
	addAndMakeVisible(ampAttackSlider);
	ampAttackSlider.setRange(0, 12);
	ampAttackSlider.setTextValueSuffix("s");
	ampAttackSlider.addListener(this);
	// Adding label
	addAndMakeVisible(ampAttackLabel);
	ampAttackLabel.setText("Attack", juce::dontSendNotification);
	ampAttackLabel.attachToComponent(&ampAttackSlider, true);
	ampAttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "AMPATTACK", ampAttackSlider);

	// Adding slider
	addAndMakeVisible(ampDecaySlider);
	ampDecaySlider.setRange(0, 12);
	ampDecaySlider.setTextValueSuffix("s");
	ampDecaySlider.addListener(this);
	// Adding label
	addAndMakeVisible(ampDecayLabel);
	ampDecayLabel.setText("Decay", juce::dontSendNotification);
	ampDecayLabel.attachToComponent(&ampDecaySlider, true);
	ampDecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "AMPDECAY", ampDecaySlider);

	// Adding slider
	addAndMakeVisible(ampSustainSlider);
	ampSustainSlider.setRange(0, 12);
	ampSustainSlider.setTextValueSuffix("dB");
	ampSustainSlider.addListener(this);
	// Adding label
	addAndMakeVisible(ampSustainLabel);
	ampSustainLabel.setText("Sustain", juce::dontSendNotification);
	ampSustainLabel.attachToComponent(&ampSustainSlider, true);
	ampSustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "AMPSUSTAIN", ampSustainSlider);

	// Adding slider
	addAndMakeVisible(ampReleaseSlider);
	ampReleaseSlider.setRange(0, 12);
	ampReleaseSlider.setTextValueSuffix("s");
	ampReleaseSlider.addListener(this);
	// Adding label
	addAndMakeVisible(ampReleaseLabel);
	ampReleaseLabel.setText("Release", juce::dontSendNotification);
	ampReleaseLabel.attachToComponent(&ampReleaseSlider, true);
	ampReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "AMPRELEASE", ampReleaseSlider);

	// Filter
	// Adding slider
	addAndMakeVisible(cutoffSlider);
	cutoffSlider.setRange(0, 12);
	cutoffSlider.setTextValueSuffix("s");
	cutoffSlider.addListener(this);
	// Adding label
	addAndMakeVisible(cutoffLabel);
	cutoffLabel.setText("Cutoff", juce::dontSendNotification);
	cutoffLabel.attachToComponent(&cutoffSlider, true);
	cutoffSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "CUTOFF", cutoffSlider);

	// Adding slider
	addAndMakeVisible(resonanceSlider);
	resonanceSlider.setRange(0, 12);
	resonanceSlider.setTextValueSuffix("s");
	resonanceSlider.addListener(this);
	// Adding label
	addAndMakeVisible(resonanceLabel);
	resonanceLabel.setText("Resonance", juce::dontSendNotification);
	resonanceLabel.attachToComponent(&resonanceSlider, true);
	resonanceSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "RESONANCE", resonanceSlider);
}

ThirdYearProjectAudioProcessorEditor::~ThirdYearProjectAudioProcessorEditor()
{
}

//==============================================================================
void ThirdYearProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ThirdYearProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	auto sliderLeft = 120;
	op1ModIndexSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
	op2ModIndexSlider.setBounds(sliderLeft, 40, getWidth() - sliderLeft - 10, 20);
	op3ModIndexSlider.setBounds(sliderLeft, 60, getWidth() - sliderLeft - 10, 20);
	op4ModIndexSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);

    op1LevelSlider.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 10, 20);
    op2LevelSlider.setBounds(sliderLeft, 120, getWidth() - sliderLeft - 10, 20);
	op3LevelSlider.setBounds(sliderLeft, 140, getWidth() - sliderLeft - 10, 20);
	op4LevelSlider.setBounds(sliderLeft, 160, getWidth() - sliderLeft - 10, 20);

	ampAttackSlider.setBounds(sliderLeft, 180, getWidth() - sliderLeft - 10, 20);
	ampDecaySlider.setBounds(sliderLeft, 200, getWidth() - sliderLeft - 10, 20);
	ampSustainSlider.setBounds(sliderLeft, 220, getWidth() - sliderLeft - 10, 20);
	ampReleaseSlider.setBounds(sliderLeft, 240, getWidth() - sliderLeft - 10, 20);

	cutoffSlider.setBounds(sliderLeft, 260, getWidth() - sliderLeft - 10, 20);
	resonanceSlider.setBounds(sliderLeft, 280, getWidth() - sliderLeft - 10, 20);

	int w = (int)keyboardComponent.getKeyWidth() * (7 * 7 + 3), h = 50;
	keyboardComponent.setSize(w, h);
	keyboardComponent.setCentrePosition(getWidth() / 2, 3 * getHeight() / 4);
}

void ThirdYearProjectAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    //if (slider == &modIndexSlider) {
   //     audioProcessor.modIndex = modIndexSlider.getValue();
 //   }
//	if (slider == &op1LevelSlider) {
//		audioProcessor.op1Level = op1LevelSlider.getValue();
//	}
//	if (slider == &op2LevelSlider) {
//		audioProcessor.op2Level = op2LevelSlider.getValue();
//	}

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