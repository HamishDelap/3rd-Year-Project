/*
  ==============================================================================

    Oscillator.h
    Created: 4 Dec 2020 10:53:25am
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>

using namespace juce;

class Oscillator
{
public:
	Oscillator(double sampleRate, int wave) {
		localSampleRate = sampleRate;
		waveform = wave;
	}

	float oscCycle( double frequency, double level) {
		auto cyclesPerSecond = frequency;
		DBG("cycles per second: " + String(cyclesPerSecond));
		auto cyclesPerSample = cyclesPerSecond / localSampleRate;
		DBG("cycles per sample: " + String(localSampleRate));

		angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
		DBG("angle delta: " + String(angleDelta));

		currentAngle += angleDelta;

		switch (waveform) {
		// Sin
		case 1:
		{
			value = level * sin(currentAngle);
			break;
		}
		// Triangle
		case 2:
			break;
		// Square
		case 3:
			break;
		}

		return value;
	}
private:
	float currentAngle = 1;
	float angleDelta;
	double localSampleRate;
	int waveform;
	float value = 1;
};