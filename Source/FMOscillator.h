/*
  ==============================================================================

    TinySP.h
    Created: 2 Nov 2020 3:01:42pm
    Author:  hamis

  ==============================================================================
*/



#pragma once
#include <../JuceLibraryCode/JuceHeader.h>
#include "Operator.h"
#include "ModEnvelope.h"
#include "LFO.h"
#include <memory>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

using namespace juce;

/*
fmTable:
 row0: angle
 row1: level
 row2: lastLevel
 - columns represent operators
*/

class FMOscillator
{
public:
	FMOscillator(int algorithm, std::shared_ptr<ADSR> op1Adsr, std::shared_ptr<ADSR> op2Adsr, std::shared_ptr<ADSR> op3Adsr, std::shared_ptr<ADSR> op4Adsr,
				 int waveforms[],  double sampleRate) {
		// Set algorithm and sample rate.
		algo = algorithm;
		samplerate = sampleRate;
		// Assign shared pointers for envelopes.
		op1Env = std::shared_ptr<ADSR>(op1Adsr);
		op2Env = std::shared_ptr<ADSR>(op2Adsr);
		op3Env = std::shared_ptr<ADSR>(op3Adsr);
		op4Env = std::shared_ptr<ADSR>(op4Adsr);

		// Initialise the four operators.
		operator1.reset(new Operator(samplerate, waveforms[0], op1Env));
		operator2.reset(new Operator(samplerate, waveforms[1], op2Env));
		operator3.reset(new Operator(samplerate, waveforms[2], op3Env));
		operator4.reset(new Operator(samplerate, waveforms[3], op4Env));
	}

	// Update waveforms based on an array of the shapes.
	void updateWaveforms(int waveforms[]) {
		operator1->updateWaveform(waveforms[0]);
		operator2->updateWaveform(waveforms[1]);
		operator3->updateWaveform(waveforms[2]);
		operator4->updateWaveform(waveforms[3]);
	}

	// Processes each step of audio.
	double oscStep(double fmTable[][4], double frequency, std::shared_ptr<ModEnvelope> modAdsr, std::shared_ptr<Lfo> modLfo, float pitchWheel) {
		// Calculate the frequency in cycles per second accounting for pitch wheel and lfo. (Divide by 10 simply scales the lfo for a better sound)
		auto cyclesPerSecond = ((frequency * pitchWheel)+ modLfo->getOutput(1) / 10);
		
		// Calculate cycles per sample
		auto cyclesPerSample = cyclesPerSecond / samplerate;

		// Calculate the change in frequency for updating FM table.
		double angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
		float output = 0;

		// Swith statement for each algorithm
		switch (algo) {
		case 1:
			// Increment carrier frequency and then calculate modulator frequencies.
			fmTable[0][0] += angleDelta;
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][2] * fmTable[2][3];

			// Apply the fm algorithm.
			output = operator1->operatorStep((fmTable[0][0] + operator2->operatorStep((fmTable[0][1] + operator3->operatorStep((fmTable[0][2] + operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][2])), fmTable[1][1])), fmTable[1][0]);
			break;
		case 2:
			fmTable[0][0] += angleDelta;
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][1] * fmTable[2][3];

			output = operator1->operatorStep((fmTable[0][0] + 
						operator2->operatorStep((fmTable[0][1] + 
							operator3->operatorStep(fmTable[0][2], fmTable[1][2]) + 
								operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][1])), fmTable[1][0]);
			break;
		case 3:
			fmTable[0][0] += angleDelta;
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][0] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][0] * fmTable[2][3];

			output = operator1->operatorStep((fmTable[0][0] + 
						operator2->operatorStep(fmTable[0][1], fmTable[1][1]) + 
							operator3->operatorStep(fmTable[0][2] , fmTable[1][2]) + 
								operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][0]);
			break;
		case 4:
			fmTable[0][0] += angleDelta;
	
			fmTable[0][0] = fmTable[0][0] * fmTable[2][0];
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][0] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][0] * fmTable[2][3];


			output = operator1->operatorStep(fmTable[0][0], fmTable[1][0]) + 
						operator2->operatorStep(fmTable[0][1], fmTable[1][1]) +
						operator3->operatorStep(fmTable[0][2], fmTable[1][2]) +
						operator4->operatorStep(fmTable[0][3], fmTable[1][3]);
			break;
		}
		if (modAdsr->isOn()) {
			double adsrLevel = modAdsr->getOutput(3);
			output = output * adsrLevel;
		}
		return output;
	}
private:
	int algo;
	double samplerate;
	std::shared_ptr<ADSR> op2Env;
	std::shared_ptr<ADSR> op1Env;
	std::shared_ptr<ADSR> op3Env;
	std::shared_ptr<ADSR> op4Env;

	std::unique_ptr<Operator> operator1;
	std::unique_ptr<Operator> operator2;
	std::unique_ptr<Operator> operator3;
	std::unique_ptr<Operator> operator4;

};
#endif // FUNCTIONS_H_INCLUDED 