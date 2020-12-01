/*
  ==============================================================================

    TinySP.h
    Created: 2 Nov 2020 3:01:42pm
    Author:  hamis

  ==============================================================================
*/

#include <../JuceLibraryCode/JuceHeader.h>
#include "SynthVoice.h"
#pragma once

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED


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


float fmOSC(int algo, double fmTable[][4], double angleDelta, ADSR* op1Env, ADSR* op2Env, ADSR* op3Env, ADSR* op4Env);

#endif // FUNCTIONS_H_INCLUDED 