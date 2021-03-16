/*
  ==============================================================================

    ModWheelController.cpp
    Created: 15 Mar 2021 6:43:07pm
    Author:  hamis

  ==============================================================================
*/

#include "ModWheelController.h"

ModWheelController::ModWheelController()
{
    destinationArray[0] = 0;
    destinationArray[1] = 0;
    destinationArray[2] = 0;
}

void ModWheelController::toggleDest(float toggle, int destination) {
	if (toggle == 1.0f) {
        selectDest(destination);
    }
    else {
        deselectDest(destination);
    }
}

void ModWheelController::selectDest(int destination) {
    destinationArray[destination] = 1;
}

void ModWheelController::deselectDest(int destination) {
    destinationArray[destination] = 0;
}

bool ModWheelController::destActive(int destination) {
    if(destinationArray[destination] == 1) {
        return true;
    } else {
        return false;
    }
}