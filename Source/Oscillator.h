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

	float oscCycleWithFreq( double frequency, double level) {
		auto cyclesPerSecond = frequency;
		auto cyclesPerSample = cyclesPerSecond / localSampleRate;

		angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

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

	float oscCycleWithAngle(double currentAngle, double level) {
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