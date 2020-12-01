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
	setSize (1600, 800);

	// Mod Index Sliders

	// Adding slider
	addAndMakeVisible(op1ModIndexSlider);
	op1ModIndexSlider.setRange(0, 12);
	op1ModIndexSlider.setTextValueSuffix("x");
	op1ModIndexSlider.addListener(this);
	//op1ModIndexSlider.setSliderStyle(juce::Slider::LinearBarVertical);
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
	keyboardComponent.setAvailableRange(0, 127);

	// op1 ADSR
	// Adding slider
	addAndMakeVisible(op1AttackSlider);
	op1AttackSlider.setRange(0, 12);
	op1AttackSlider.setTextValueSuffix("s");
	op1AttackSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op1AttackLabel);
	op1AttackLabel.setText("Op1 Attack", juce::dontSendNotification);
	op1AttackLabel.attachToComponent(&op1AttackSlider, true);
	op1AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1ATTACK", op1AttackSlider);
	// Adding slider
	addAndMakeVisible(op1DecaySlider);
	op1DecaySlider.setRange(0, 12);
	op1DecaySlider.setTextValueSuffix("s");
	op1DecaySlider.addListener(this);
	// Adding label
	addAndMakeVisible(op1DecayLabel);
	op1DecayLabel.setText("Op1 Decay", juce::dontSendNotification);
	op1DecayLabel.attachToComponent(&op1DecaySlider, true);
	op1DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1DECAY", op1DecaySlider);
	// Adding slider
	addAndMakeVisible(op1SustainSlider);
	op1SustainSlider.setRange(0, 12);
	op1SustainSlider.setTextValueSuffix("dB");
	op1SustainSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op1SustainLabel);
	op1SustainLabel.setText("Op1 Sustain", juce::dontSendNotification);
	op1SustainLabel.attachToComponent(&op1SustainSlider, true);
	op1SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1SUSTAIN", op1SustainSlider);
	// Adding slider
	addAndMakeVisible(op1ReleaseSlider);
	op1ReleaseSlider.setRange(0, 12);
	op1ReleaseSlider.setTextValueSuffix("s");
	op1ReleaseSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op1ReleaseLabel);
	op1ReleaseLabel.setText("Op1 Release", juce::dontSendNotification);
	op1ReleaseLabel.attachToComponent(&op1ReleaseSlider, true);
	op1ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP1RELEASE", op1ReleaseSlider);

	// op2 ADSR
	// Adding slider
	addAndMakeVisible(op2AttackSlider);
	op2AttackSlider.setRange(0, 12);
	op2AttackSlider.setTextValueSuffix("s");
	op2AttackSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2AttackLabel);
	op2AttackLabel.setText("Op2 Attack", juce::dontSendNotification);
	op2AttackLabel.attachToComponent(&op2AttackSlider, true);
	op2AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2ATTACK", op2AttackSlider);
	// Adding slider
	addAndMakeVisible(op2DecaySlider);
	op2DecaySlider.setRange(0, 12);
	op2DecaySlider.setTextValueSuffix("s");
	op2DecaySlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2DecayLabel);
	op2DecayLabel.setText("Op2 Decay", juce::dontSendNotification);
	op2DecayLabel.attachToComponent(&op2DecaySlider, true);
	op2DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2DECAY", op2DecaySlider);
	// Adding slider
	addAndMakeVisible(op2SustainSlider);
	op2SustainSlider.setRange(0, 12);
	op2SustainSlider.setTextValueSuffix("dB");
	op2SustainSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2SustainLabel);
	op2SustainLabel.setText("Op2 Sustain", juce::dontSendNotification);
	op2SustainLabel.attachToComponent(&op2SustainSlider, true);
	op2SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2SUSTAIN", op2SustainSlider);
	// Adding slider
	addAndMakeVisible(op2ReleaseSlider);
	op2ReleaseSlider.setRange(0, 12);
	op2ReleaseSlider.setTextValueSuffix("s");
	op2ReleaseSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op2ReleaseLabel);
	op2ReleaseLabel.setText("Op2 Release", juce::dontSendNotification);
	op2ReleaseLabel.attachToComponent(&op2ReleaseSlider, true);
	op2ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP2RELEASE", op2ReleaseSlider);

	// op3 ADSR
// Adding slider
	addAndMakeVisible(op3AttackSlider);
	op3AttackSlider.setRange(0, 12);
	op3AttackSlider.setTextValueSuffix("s");
	op3AttackSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3AttackLabel);
	op3AttackLabel.setText("Op3 Attack", juce::dontSendNotification);
	op3AttackLabel.attachToComponent(&op3AttackSlider, true);
	op3AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3ATTACK", op3AttackSlider);
	// Adding slider
	addAndMakeVisible(op3DecaySlider);
	op3DecaySlider.setRange(0, 12);
	op3DecaySlider.setTextValueSuffix("s");
	op3DecaySlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3DecayLabel);
	op3DecayLabel.setText("Op3 Decay", juce::dontSendNotification);
	op3DecayLabel.attachToComponent(&op3DecaySlider, true);
	op3DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3DECAY", op3DecaySlider);
	// Adding slider
	addAndMakeVisible(op3SustainSlider);
	op3SustainSlider.setRange(0, 12);
	op3SustainSlider.setTextValueSuffix("dB");
	op3SustainSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3SustainLabel);
	op3SustainLabel.setText("Op3 Sustain", juce::dontSendNotification);
	op3SustainLabel.attachToComponent(&op3SustainSlider, true);
	op3SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3SUSTAIN", op3SustainSlider);
	// Adding slider
	addAndMakeVisible(op3ReleaseSlider);
	op3ReleaseSlider.setRange(0, 12);
	op3ReleaseSlider.setTextValueSuffix("s");
	op3ReleaseSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op3ReleaseLabel);
	op3ReleaseLabel.setText("Op3 Release", juce::dontSendNotification);
	op3ReleaseLabel.attachToComponent(&op3ReleaseSlider, true);
	op3ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP3RELEASE", op3ReleaseSlider);

	// op4 ADSR
// Adding slider
	addAndMakeVisible(op4AttackSlider);
	op4AttackSlider.setRange(0, 12);
	op4AttackSlider.setTextValueSuffix("s");
	op4AttackSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4AttackLabel);
	op4AttackLabel.setText("Op4 Attack", juce::dontSendNotification);
	op4AttackLabel.attachToComponent(&op4AttackSlider, true);
	op4AttackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4ATTACK", op4AttackSlider);
	// Adding slider
	addAndMakeVisible(op4DecaySlider);
	op4DecaySlider.setRange(0, 12);
	op4DecaySlider.setTextValueSuffix("s");
	op4DecaySlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4DecayLabel);
	op4DecayLabel.setText("Op4 Decay", juce::dontSendNotification);
	op4DecayLabel.attachToComponent(&op4DecaySlider, true);
	op4DecaySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4DECAY", op4DecaySlider);
	// Adding slider
	addAndMakeVisible(op4SustainSlider);
	op4SustainSlider.setRange(0, 12);
	op4SustainSlider.setTextValueSuffix("dB");
	op4SustainSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4SustainLabel);
	op4SustainLabel.setText("Op4 Sustain", juce::dontSendNotification);
	op4SustainLabel.attachToComponent(&op4SustainSlider, true);
	op4SustainSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4SUSTAIN", op4SustainSlider);
	// Adding slider
	addAndMakeVisible(op4ReleaseSlider);
	op4ReleaseSlider.setRange(0, 12);
	op4ReleaseSlider.setTextValueSuffix("s");
	op4ReleaseSlider.addListener(this);
	// Adding label
	addAndMakeVisible(op4ReleaseLabel);
	op4ReleaseLabel.setText("Op4 Release", juce::dontSendNotification);
	op4ReleaseLabel.attachToComponent(&op4ReleaseSlider, true);
	op4ReleaseSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvt, "OP4RELEASE", op4ReleaseSlider);





	// Filter
	// Adding slider
	addAndMakeVisible(cutoffSlider);
	cutoffSlider.setRange(0, 12);
	cutoffSlider.setTextValueSuffix("s");
	cutoffSlider.addListener(this);
	cutoffSlider.setSliderStyle(juce::Slider::Rotary);
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
	resonanceSlider.setSliderStyle(juce::Slider::Rotary);
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

	drawSpecFrame(g);
}

void ThirdYearProjectAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	auto sliderLeft = 120;
	op1LevelSlider.setBounds(sliderLeft, 20, getWidth() / 4, 20);
	op1ModIndexSlider.setBounds(sliderLeft, 40, getWidth() / 4, 20);

	op1AttackSlider.setBounds(sliderLeft, 60, getWidth() / 4, 20);
	op1DecaySlider.setBounds(sliderLeft, 80, getWidth() / 4, 20);
	op1SustainSlider.setBounds(sliderLeft, 100, getWidth() / 4, 20);
	op1ReleaseSlider.setBounds(sliderLeft, 120, getWidth() / 4, 20);


	op2LevelSlider.setBounds(getWidth() / 2 + sliderLeft, 20, getWidth() / 4, 20);
	op2ModIndexSlider.setBounds(getWidth() / 2 + sliderLeft, 40, getWidth() / 4, 20);

	op2AttackSlider.setBounds(getWidth() / 2 + sliderLeft, 60, getWidth() / 4, 20);
	op2DecaySlider.setBounds(getWidth() / 2 + sliderLeft, 80, getWidth() / 4, 20);
	op2SustainSlider.setBounds(getWidth() / 2 + sliderLeft, 100, getWidth() / 4, 20);
	op2ReleaseSlider.setBounds(getWidth() / 2 + sliderLeft, 120, getWidth() / 4, 20);


	op3LevelSlider.setBounds(sliderLeft, 180, getWidth() / 4, 20);
	op3ModIndexSlider.setBounds(sliderLeft, 200, getWidth() / 4, 20);

	op3AttackSlider.setBounds(sliderLeft, 220, getWidth() / 4, 20);
	op3DecaySlider.setBounds(sliderLeft, 240, getWidth() / 4, 20);
	op3SustainSlider.setBounds(sliderLeft, 260, getWidth() / 4, 20);
	op3ReleaseSlider.setBounds(sliderLeft, 280, getWidth() / 4, 20);

	
	op4LevelSlider.setBounds(getWidth() / 2 + sliderLeft, 180, getWidth() / 4, 20);
	op4ModIndexSlider.setBounds(getWidth() / 2 + sliderLeft, 200, getWidth() / 4, 20);

	op4AttackSlider.setBounds(getWidth() / 2 + sliderLeft, 220, getWidth() / 4, 20);
	op4DecaySlider.setBounds(getWidth() / 2 + sliderLeft, 240, getWidth() / 4, 20);
	op4SustainSlider.setBounds(getWidth() / 2 + sliderLeft, 260, getWidth() / 4, 20);
	op4ReleaseSlider.setBounds(getWidth() / 2 + sliderLeft, 280, getWidth() / 4, 20);



	cutoffSlider.setBounds(sliderLeft, 510, 130, 130);
//	cutoffSlider.setBounds(sliderLeft, 260, 20, 20);
	resonanceSlider.setBounds(sliderLeft, 550, 130, 130);

	int w = (int)keyboardComponent.getKeyWidth() * (7 * 10 + 5), h = 80;
	keyboardComponent.setSize(w, h);
	keyboardComponent.setCentrePosition(getWidth() / 2, getHeight() - 40);
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

void ThirdYearProjectAudioProcessorEditor::drawSpecFrame(Graphics& g)
{
	for (int i = 1; i < audioProcessor.scopeSize; ++i)
	{
		auto width = getLocalBounds().getWidth();
		auto height = getLocalBounds().getHeight() / 2;

		g.drawLine({ (float) jmap(i - 1, 0, audioProcessor.scopeSize - 1, 0, width),
					  jmap(audioProcessor.scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
			  (float) jmap(i,     0, audioProcessor.scopeSize - 1, 0, width),
					  jmap(audioProcessor.scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
	}
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
	if (audioProcessor.nextFFTBlockReady) {
		audioProcessor.drawNextFrameOfSpectrum();
		audioProcessor.nextFFTBlockReady = false;
		repaint();
	}
}