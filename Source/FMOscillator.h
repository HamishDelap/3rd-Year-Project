/*
  ==============================================================================

    TinySP.h
    Created: 2 Nov 2020 3:01:42pm
    Author:  hamis

  ==============================================================================
*/

#include <../JuceLibraryCode/JuceHeader.h>
#include "SynthVoice.h"
#include "Oscillator.h"
#include "Operator.h"
#include "ModEnvelope.h"
#include "LFO.h"
#include <memory>

#pragma once

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

using namespace juce;


/*
-Oscillator Class
    -Subclass for each waveform

-ADSR Class

-LFO Class

-Future:
    -JUCE Wrappers
        -Tree wrapper functions
        -Knobs
        -Text box
        -Wave display
        -Effects
*/

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

		algo = algorithm;
		samplerate = sampleRate;
		op1Env = std::shared_ptr<ADSR>(op1Adsr);
		op2Env = std::shared_ptr<ADSR>(op2Adsr);
		op3Env = std::shared_ptr<ADSR>(op3Adsr);
		op4Env = std::shared_ptr<ADSR>(op4Adsr);

		operator1.reset(new Operator(samplerate, waveforms[0], op1Env));
		operator2.reset(new Operator(samplerate, waveforms[1], op2Env));
		operator3.reset(new Operator(samplerate, waveforms[2], op3Env));
		operator4.reset(new Operator(samplerate, waveforms[3], op4Env));
	}

	float op(double angle, double level, ADSR* env) {
		float nextEnvSample = env->getNextSample();
		return (float)(env->getNextSample() * std::sin(angle) * level);
	}

	void updateWaveforms(int waveforms[]) {
		operator1->updateWaveform(waveforms[0]);
		operator2->updateWaveform(waveforms[1]);
		operator3->updateWaveform(waveforms[2]);
		operator4->updateWaveform(waveforms[3]);
	}

	float oscStep(double fmTable[][4], double frequency, std::shared_ptr<ModEnvelope> modAdsr, std::shared_ptr<Lfo> modLfo) {

		float adsrLevel;

		auto cyclesPerSecond = (frequency + modLfo->getOutput(1) / 10);
		if (modAdsr->isOn()) {
			cyclesPerSecond *= modAdsr->getOutput(1);
		}
		auto cyclesPerSample = cyclesPerSecond / samplerate;

		float angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;

		float output = 0;
		switch (algo) {
		case 1:
			fmTable[0][0] += angleDelta;
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][2] * fmTable[2][3];
			                    
			output = operator1->operatorStep((fmTable[0][0] + 
						operator2->operatorStep((fmTable[0][1] + 
							operator3->operatorStep((fmTable[0][2] + 
								operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][2])), fmTable[1][1])), fmTable[1][0]);
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
						operator2->operatorStep((fmTable[0][1] + 
							operator3->operatorStep((fmTable[0][2] + 
								operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][2])), fmTable[1][1])), fmTable[1][0]);
			break;
		case 4:
			fmTable[0][0] += angleDelta;
			fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
			fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
			fmTable[0][3] = fmTable[0][2] * fmTable[2][3];

			output = operator1->operatorStep((fmTable[0][0] + 
						operator2->operatorStep((fmTable[0][1] + 
							operator3->operatorStep((fmTable[0][2] + 
								operator4->operatorStep(fmTable[0][3], fmTable[1][3])), fmTable[1][2])), fmTable[1][1])), fmTable[1][0]);
			break;
		}
		if (modAdsr->isOn()) {
			adsrLevel = modAdsr->getOutput(3);
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