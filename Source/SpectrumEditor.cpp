/*
  ==============================================================================

    SpectrumEditor.cpp
    Created: 12 Mar 2021 2:37:19pm
    Author:  hamis

  ==============================================================================
*/

#include "SpectrumEditor.h"

SpectrumEditor::SpectrumEditor(int height, int width)
{
	specHeight = height;
	specWidth = width;
}

void SpectrumEditor::drawSpecFrame(juce::Graphics& g, ThirdYearProjectAudioProcessor& audioProcessor)
{
	transform = AffineTransform::translation((float)0, (float)specHeight + 325);

	for (int i = 1; i < audioProcessor.spectrumProcessor->scopeSize; ++i)
	{
		Line<float> line((float)jmap(i - 1, 0, audioProcessor.spectrumProcessor->scopeSize - 1, 0, specWidth),
			jmap(audioProcessor.spectrumProcessor->scopeData[i - 1], 0.0f, 1.0f, (float)(specHeight * 1.5), 0.0f),
			(float)jmap(i, 0, audioProcessor.spectrumProcessor->scopeSize - 1, 0, specWidth),
			jmap(audioProcessor.spectrumProcessor->scopeData[i], 0.0f, 1.0f, (float)(specHeight * 1.5), 0.0f));

		line.applyTransform(transform);
		g.drawLine(line);
	}
}