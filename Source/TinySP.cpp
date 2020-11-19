/*
  ==============================================================================

    TinySP.cpp
    Created: 2 Nov 2020 3:01:42pm
    Author:  hamis

  ==============================================================================
*/

#include "TinySP.h"
#include "SynthVoice.h"
#include <../JuceLibraryCode/JuceHeader.h>

/*
- op1(op2(op3(op4)))
- op1(op2(op3 + op4))
- op1(op2 + op3 + op4)
- op1 + op2 + op3 + op4
*/

float wave(int wave, double angle, double level) {
	switch (wave) {
		// Sin
	case 1:
		return (float)(std::sin(angle) * level);
		break;
		// Triangle
	case 2:
		break;
		// Square
	case 3:
		break;
	}
}

float op(double angle, double level, ADSR* env) {
	return (float) (env->getNextSample() * std::sin(angle) * level);
}

float fmOSC(int algo, double fmTable[][4], double angleDelta, ADSR* op1Env, ADSR* op2Env, ADSR* op3Env, ADSR* op4Env) {
	float output = 0;
	switch (algo) {
	case 1:
		fmTable[0][0] += angleDelta;
		fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
		fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
		fmTable[0][3] = fmTable[0][2] * fmTable[2][3];

		output = op((fmTable[0][0] + op((fmTable[0][1] + op((fmTable[0][2] + op(fmTable[0][3], fmTable[1][3], op4Env)), fmTable[1][2], op3Env)), fmTable[1][1], op2Env)), fmTable[1][0], op1Env);
		break;
	case 2:
		fmTable[0][0] += angleDelta;
		fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
		fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
		fmTable[0][3] = fmTable[0][1] * fmTable[2][3];

		output = op((fmTable[0][0] + op((fmTable[0][1] + op(fmTable[0][2], fmTable[1][2], op3Env) + op(fmTable[0][3], fmTable[1][3], op4Env)), fmTable[1][1], op2Env)), fmTable[1][0], op1Env);
		break;
	case 3:
		fmTable[0][0] += angleDelta;
		fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
		fmTable[0][2] = fmTable[0][0] * fmTable[2][2];
		fmTable[0][3] = fmTable[0][0] * fmTable[2][3];

		output = op((fmTable[0][0] + op((fmTable[0][1] + op((fmTable[0][2] + op(fmTable[0][3], fmTable[1][3], op4Env)), fmTable[1][2], op3Env)), fmTable[1][1], op2Env)), fmTable[1][0], op1Env);
		break;
	case 4:
		fmTable[0][0] += angleDelta;
		fmTable[0][1] = fmTable[0][0] * fmTable[2][1];
		fmTable[0][2] = fmTable[0][1] * fmTable[2][2];
		fmTable[0][3] = fmTable[0][2] * fmTable[2][3];

		output = op((fmTable[0][0] + op((fmTable[0][1] + op((fmTable[0][2] + op(fmTable[0][3], fmTable[1][3], op4Env)), fmTable[1][2], op3Env)), fmTable[1][1], op2Env)), fmTable[1][0], op1Env);
		break;
	}
	return output;
}