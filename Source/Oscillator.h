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

		float triangleDelta = 0.1;

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
			sineValue = sin(currentAngle);

			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
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
			sineValue = sin(currentAngle);

			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
			value = level * value;
			break;
		// Square
		case 3:
			sineValue = sin(currentAngle);

			value = (sineValue >= 0) ? 1 : -1;
			break;
		}

		return value;
	}

	void setWaveform(int waveshape) {
		waveform = waveshape;
	}
private:
	float currentAngle = 1;
	float angleDelta;
	double localSampleRate;
	int waveform;
	float value = 1;

	float sineValue = 0;
	float triangleDelta = 0.1;
};