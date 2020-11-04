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
    setSize (500, 300);

    // Adding slider
    addAndMakeVisible(op1ModIndexSlider);
    op1ModIndexSlider.setRange(0, 12);
    op1ModIndexSlider.setTextValueSuffix("x");
    op1ModIndexSlider.addListener(this);

    // Adding label
    addAndMakeVisible(op1ModIndexLabel);
    op1ModIndexLabel.setText("Modulation Index", juce::dontSendNotification);
    op1ModIndexLabel.attachToComponent(&op1ModIndexSlider, true);
    op1ModIndexSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "MODINDEX", op1ModIndexSlider);


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

	addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);

	keyboardComponent.setAvailableRange(21, 108);
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
    op1LevelSlider.setBounds(sliderLeft, 40, getWidth() - sliderLeft - 10, 20);
    op2LevelSlider.setBounds(sliderLeft, 60, getWidth() - sliderLeft - 10, 20);

	int w = (int)keyboardComponent.getKeyWidth() * (7 * 7 + 3), h = 50;
	keyboardComponent.setSize(w, h);
	keyboardComponent.setCentrePosition(getWidth() / 2, getHeight() / 2);
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