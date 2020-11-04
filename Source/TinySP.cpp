/*
  ==============================================================================

    TinySP.cpp
    Created: 2 Nov 2020 3:01:42pm
    Author:  hamis

  ==============================================================================
*/

#include "TinySP.h"
#include <../JuceLibraryCode/JuceHeader.h>

/*
- op1(op2(op3(op4)))
- op1(op2(op3 + op4))
- op1(op2 + op3 + op4)
- op1 + op2 + op3 + op4
*/

float op(double angle, double level) {
	return (float)(std::sin(angle) * level);
}

float fmOSC(int algo, double fmTable[2][4]) {
	float output = 0;
	switch (algo) {
	case 1:
		output = op((fmTable[0][0] + op(fmTable[0][1], fmTable[1][1])), fmTable[1][0]);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	return output;
}
