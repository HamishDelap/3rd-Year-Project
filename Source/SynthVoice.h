/*
  ==============================================================================

    SynthVoice.h
    Created: 20 Oct 2020 4:23:03pm
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <../JuceLibraryCode/JuceHeader.h>
#include "SynthSound.h"
#include "FMOscillator.h"
#include "Oscillator.h"
#include "ModEnvelope.h"
#include "LFO.h"

using namespace juce;

class SynthVoice : public SynthesiserVoice
{
public:

    bool canPlaySound(SynthesiserSound* sound) {
        // If the sound can be cast to the class SynthSound it will return true, otherwise it will return false. (Taken from a tutorial)
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
	//===============================================//

    void setOP1MODINDEX(float* modindex) {
        fmTable[2][0] = *modindex;
    }

	void setOP2MODINDEX(float* modindex) {
        fmTable[2][1] = *modindex;
	}

	void setOP3MODINDEX(float* modindex) {
        fmTable[2][2] = *modindex;
	}

	void setOP4MODINDEX(float* modindex) {
        fmTable[2][3] = *modindex;
	}

	void setOP1LEVEL(float* level) {
		fmTable[1][0] = *level;
	}

	void setOP2LEVEL(float* level) {
		fmTable[1][1] = *level;
	}

	void setOP3LEVEL(float* level) {
		fmTable[1][2] = *level;
	}

	void setOP4LEVEL(float* level) {
		fmTable[1][3] = *level;
	}

    void setOp1Adsr(float* attack, float* decay, float* sustain, float* release) {
        op1AdsrParams.attack = *attack;
        op1AdsrParams.decay = *decay;
        op1AdsrParams.sustain = *sustain;
        op1AdsrParams.release = *release;
    }

    void setOp2Adsr(float* attack, float* decay, float* sustain, float* release) {
        op2AdsrParams.attack = *attack;
        op2AdsrParams.decay = *decay;
        op2AdsrParams.sustain = *sustain;
        op2AdsrParams.release = *release;
    }

    void setOp3Adsr(float* attack, float* decay, float* sustain, float* release) {
        op3AdsrParams.attack = *attack;
        op3AdsrParams.decay = *decay;
        op3AdsrParams.sustain = *sustain;
        op3AdsrParams.release = *release;
    }

    void setOp4Adsr(float* attack, float* decay, float* sustain, float* release) {
        op4AdsrParams.attack = *attack;
        op4AdsrParams.decay = *decay;
        op4AdsrParams.sustain = *sustain;
        op4AdsrParams.release = *release;
    }

    void setModAdsr(std::shared_ptr<ModEnvelope> env) {
        modAdsr = env;
    }

    void setModLfo(std::shared_ptr<Lfo> lfo) {
        modLfo = lfo;
    }

 //   void setModAdsrParams(float* attack, float* decay, float* sustain, float* release) {
 //       modAdsr->setAttack(*attack);
 //       modAdsr->setDecay(*decay);
 //       modAdsr->setSustain(*sustain);
 //       modAdsr->setRelease(*release);
 //   }

    void setADSRSampleRate (double sampleRate) {
        op1Adsr->setSampleRate(sampleRate);
        op2Adsr->setSampleRate(sampleRate);
        op3Adsr->setSampleRate(sampleRate);
        op4Adsr->setSampleRate(sampleRate);
        modAdsr->setSampleRate(sampleRate);
    }

    void setAlgo(float* algo) {
        algorithm = (int) *algo;
    }

    void setOP1WAVEFORM(float* waveform) {
        waveforms[0] = (int)*waveform;
    }

    void setOP2WAVEFORM(float* waveform) {
        waveforms[1] = (int)*waveform;
    }

    void setOP3WAVEFORM(float* waveform) {
        waveforms[2] = (int)*waveform;
    }

    void setOP4WAVEFORM(float* waveform) {
        waveforms[3] = (int)*waveform;
    }

    //===============================================//

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
        // Create FM Oscillator for the voice.
        fmosc.reset(new FMOscillator(algorithm, op1Adsr, op2Adsr, op3Adsr, op4Adsr, waveforms, getSampleRate()));
        fmosc->updateWaveforms(waveforms);

        // Calculate frequency of the note in Hz.
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity * 0.15;
        tailOff = 0.0;

        op1Adsr->noteOn();
        op2Adsr->noteOn();
        op3Adsr->noteOn();
        op4Adsr->noteOn();
        modAdsr->noteOn();
    }

    //===============================================//

    void stopNote(float velocity, bool allowTailOff) {
        if (allowTailOff) {
            if (tailOff == 0.0) {
                tailOff = 1.0;
            }
        }
        else {
            clearCurrentNote();
            angleDelta = 0.0;
        }
        op1Adsr->noteOff();
        op2Adsr->noteOff();
        op3Adsr->noteOff();
        op4Adsr->noteOff();
        modAdsr->noteOff();
    }

    void pitchWheelMoved(int newPitchWheelValue) {
        
    }

    void controllerMoved(int controllerNumber, int newControllerValue) {
        DBG(controllerNumber + ": " + newControllerValue);
    }

    void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
        op1Adsr->setParameters(op1AdsrParams);
        op2Adsr->setParameters(op2AdsrParams);
        op3Adsr->setParameters(op3AdsrParams);
        op4Adsr->setParameters(op4AdsrParams);
       // if (modAdsr->isOn()) {
       //     modAdsr->envelopeStep();
       // }
            // Written using JUCE midi synthesizer tutorial.
        if (fmosc != NULL) {
            // Check if note should have ended.
            if (tailOff > 0.0) {
                // Check theres samples left.
                while (--numSamples >= 0) {
                    // Calculate sample value.
                    modAdsr->envelopeStep();
                    fmosc->updateWaveforms(waveforms);
                    auto currentSample = fmosc->oscStep(fmTable, frequency, modAdsr, modLfo) * level;
                    // Add sample to outputBuffer
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    // Move wave along.
                    //fmTable[0][0] += angleDelta;
                    //fmTable[0][1] = fmTable[0][0] * modIndex;
                    // Increment start sample;
                    ++startSample;

                    tailOff *= 0.99999;

                    if (tailOff <= 0.005) {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else {
                while (--numSamples >= 0) {
                    modAdsr->envelopeStep();
                    fmosc->updateWaveforms(waveforms);
                    auto currentSample = fmosc->oscStep(fmTable, frequency, modAdsr, modLfo) * level;
                    //auto currentSample = (float)(std::sin(fmTable[0][0] + (float)std::sin(fmTable[0][1]) * fmTable[1][1]) * fmTable[1][0]);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                        outputBuffer.addSample(i, startSample, currentSample);
                    }

                    //fmTable[0][0] += angleDelta;
                    //fmTable[0][1] = fmTable[0][0] * modIndex;
                    ++startSample;
                }
            }
        }
    }
    
private:
    std::unique_ptr <FMOscillator> fmosc;

    int algorithm;

    int waveforms[4];

    double frequency;

    double modIndex = 3.2;

    double fmTable[3][4];

    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;

    std::shared_ptr<ADSR> op1Adsr = std::shared_ptr<ADSR>(new ADSR());
    ADSR::Parameters op1AdsrParams;

    std::shared_ptr<ADSR> op2Adsr = std::shared_ptr<ADSR>(new ADSR());
    ADSR::Parameters op2AdsrParams;

    std::shared_ptr<ADSR> op3Adsr = std::shared_ptr<ADSR>(new ADSR());
    ADSR::Parameters op3AdsrParams;

    std::shared_ptr<ADSR> op4Adsr = std::shared_ptr<ADSR>(new ADSR());
    ADSR::Parameters op4AdsrParams;

    std::shared_ptr<ModEnvelope> modAdsr = std::shared_ptr<ModEnvelope>(new ModEnvelope(10000, 0, 0, 0, 0));
    std::shared_ptr<Lfo> modLfo = std::shared_ptr<Lfo>(new Lfo(0, 1, 1, 1));
};