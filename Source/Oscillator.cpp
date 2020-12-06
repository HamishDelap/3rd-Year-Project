/*
  ==============================================================================

    Oscillator.cpp
    Created: 4 Dec 2020 10:53:25am
    Author:  hamis

  ==============================================================================
*/

#include "Oscillator.h"

Oscillator(double sampleRate, int wave) {
	localSampleRate = sampleRate;
	waveform = wave;
}

float oscCycle(int wave, double frequency, double level, double sampleRate) {
	auto cyclesPerSecond = frequency;
	auto cyclesPerSample = cyclesPerSecond / sampleRate;

	angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

	switch (waveform) {
		// Sin
	case 1:
		float value = level * sin(angleDelta);
		break;
		// Triangle
	case 2:
		break;
		// Square
	case 3:
		break;
	}
}