/*
  ==============================================================================

    StateManager.cpp
    Created: 10 Feb 2021 3:12:44pm
    Author:  hamis

  ==============================================================================
*/

#include "StateManager.h"

StateManager::StateManager(AudioProcessor& audioProcessor) :
    apvt(audioProcessor, nullptr, "PARAMETERS",
        {
            // Initialise VST Parameters.
            std::make_unique<AudioParameterFloat>("OP1MODINDEX", "OP1MODINDEX", NormalisableRange<float>(0, 12, 0.5), 0.0f),
            std::make_unique<AudioParameterFloat>("OP2MODINDEX", "OP2MODINDEX", NormalisableRange<float>(0, 12, 0.5), 0.0f),
            std::make_unique<AudioParameterFloat>("OP3MODINDEX", "OP3MODINDEX", NormalisableRange<float>(0, 12, 0.5), 0.0f),
            std::make_unique<AudioParameterFloat>("OP4MODINDEX", "OP4MODINDEX", NormalisableRange<float>(0, 12, 0.5), 0.0f),

            std::make_unique<AudioParameterFloat>("OP1LEVEL", "OP1LEVEL", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP2LEVEL", "OP2LEVEL", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP3LEVEL", "OP3LEVEL", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP4LEVEL", "OP4LEVEL", NormalisableRange<float>(0, 1), 1.0f),

            std::make_unique<AudioParameterFloat>("OP1ATTACK", "OP1ATTACK", NormalisableRange<float>(0, 6), 0.2f),
            std::make_unique<AudioParameterFloat>("OP2ATTACK", "OP2ATTACK", NormalisableRange<float>(0, 6), 0.2f),
            std::make_unique<AudioParameterFloat>("OP3ATTACK", "OP3ATTACK", NormalisableRange<float>(0, 6), 0.2f),
            std::make_unique<AudioParameterFloat>("OP4ATTACK", "OP4ATTACK", NormalisableRange<float>(0, 6), 0.2f),

            std::make_unique<AudioParameterFloat>("OP1DECAY", "OP1DECAY", NormalisableRange<float>(0, 3), 0.0f),
            std::make_unique<AudioParameterFloat>("OP2DECAY", "OP2DECAY", NormalisableRange<float>(0, 3), 0.0f),
            std::make_unique<AudioParameterFloat>("OP3DECAY", "OP3DECAY", NormalisableRange<float>(0, 3), 0.0f),
            std::make_unique<AudioParameterFloat>("OP4DECAY", "OP4DECAY", NormalisableRange<float>(0, 3), 0.0f),

            std::make_unique<AudioParameterFloat>("OP1SUSTAIN", "OP1SUSTAIN", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP2SUSTAIN", "OP2SUSTAIN", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP3SUSTAIN", "OP3SUSTAIN", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("OP4SUSTAIN", "OP4SUSTAIN", NormalisableRange<float>(0, 1), 1.0f),

            std::make_unique<AudioParameterFloat>("OP1RELEASE", "OP1RELEASE", NormalisableRange<float>(0, 3), 0.5f),
            std::make_unique<AudioParameterFloat>("OP2RELEASE", "OP2RELEASE", NormalisableRange<float>(0, 3), 0.5f),
            std::make_unique<AudioParameterFloat>("OP3RELEASE", "OP3RELEASE", NormalisableRange<float>(0, 3), 0.5f),
            std::make_unique<AudioParameterFloat>("OP4RELEASE", "OP4RELEASE", NormalisableRange<float>(0, 3), 0.5f),

            std::make_unique<AudioParameterFloat>("MODENVATTACK", "MODENVATTACK", NormalisableRange<float>(0, 6), 0.2f),
            std::make_unique<AudioParameterFloat>("MODENVDECAY", "MODENVDECAY", NormalisableRange<float>(0, 3), 0.1f),
            std::make_unique<AudioParameterFloat>("MODENVSUSTAIN", "MODENVSUSTAIN", NormalisableRange<float>(0, 1), 1.0f),
            std::make_unique<AudioParameterFloat>("MODENVRELEASE", "MODENVRELEASE", NormalisableRange<float>(0, 3), 0.2f),
            std::make_unique<AudioParameterFloat>("MODENVAMOUNT", "MODENVAMOUNT", NormalisableRange<float>(0, 1), 1.0f),

            std::make_unique<AudioParameterFloat>("LFOAMOUNT", "LFOAMOUNT", NormalisableRange<float>(0, 100), 1.0f),
            std::make_unique<AudioParameterFloat>("LFOFREQ", "LFOFREQ", NormalisableRange<float>(0.1, 10), 1.0f),
            std::make_unique<AudioParameterFloat>("LFOWAVEFORM", "LFOWAVEFORM", NormalisableRange<float>(1, 3, 1), 1.0f),
            
            std::make_unique<AudioParameterFloat>("CUTOFF", "CUTOFF", NormalisableRange<float>(0.1, 20000), 20000.0f),
            std::make_unique<AudioParameterFloat>("RESONANCE", "RESONANCE", NormalisableRange<float>(0.1, 1), 1.0f),

            std::make_unique<AudioParameterFloat>("ALGO", "ALGO", NormalisableRange<float>(1, 4), 1.0f),

            std::make_unique<AudioParameterFloat>("OP1WAVEFORM", "OP1WAVEFORM", NormalisableRange<float>(1, 3), 1.0f),
            std::make_unique<AudioParameterFloat>("OP2WAVEFORM", "OP2WAVEFORM", NormalisableRange<float>(1, 3), 1.0f),
            std::make_unique<AudioParameterFloat>("OP3WAVEFORM", "OP3WAVEFORM", NormalisableRange<float>(1, 3), 1.0f),
            std::make_unique<AudioParameterFloat>("OP4WAVEFORM", "OP4WAVEFORM", NormalisableRange<float>(1, 3), 1.0f),

            std::make_unique<AudioParameterFloat>("LFOPITCH", "LFOPITCH", NormalisableRange<float>(0, 1), 0.0f),
            std::make_unique<AudioParameterFloat>("MODENVPITCH", "MODENVPITCH", NormalisableRange<float>(0, 1), 0.0f),
            std::make_unique<AudioParameterFloat>("LFOFILTER", "LFOFILTER", NormalisableRange<float>(0, 1), 0.0f),
            std::make_unique<AudioParameterFloat>("MODENVFILTER", "MODENVFILTER", NormalisableRange<float>(0, 1), 0.0f),
            std::make_unique<AudioParameterFloat>("LFOLEVEL", "LFOLEVEL", NormalisableRange<float>(0, 1), 0.0f),
            std::make_unique<AudioParameterFloat>("MODENVLEVEL", "MODENVLEVEL", NormalisableRange<float>(0, 1), 0.0f),
            
            std::make_unique<AudioParameterFloat>("MASTERLEVEL", "MASTERLEVEL", NormalisableRange<float>(0, 1.5), 1.0f)
        }
    )
{
    apvt.state = ValueTree("apvt");
}

void StateManager::readState(juce::MemoryBlock& destData) {
    auto state = apvt.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    AudioProcessor::copyXmlToBinary(*xml, destData);
}

void StateManager::writeState(const void* data, int sizeInBytes) {
    std::unique_ptr<XmlElement> xmlState(AudioProcessor::getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvt.state.getType()))
            apvt.state = ValueTree::fromXml(*xmlState);
}