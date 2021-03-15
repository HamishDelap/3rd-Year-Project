/*
  ==============================================================================

    ModWheelController.h
    Created: 15 Mar 2021 6:43:07pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ModWheelController
{
public:
    ModWheelController();
    void toggleDest(float toggle, int dest);
    void selectDest(int destination);
    void deselectDest(int destination);
    bool destActive(int destination);
private:
	// 0 - LFO Freq, 1 - LFO Amt, 2 - Cutoff
    int destinationArray[4];
};