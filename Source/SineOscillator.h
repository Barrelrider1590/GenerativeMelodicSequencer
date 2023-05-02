/*
  ==============================================================================

    SineOscillator.h
    Created: 27 Apr 2023 6:59:06pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SineOscillator
{
public:
    SineOscillator();

    void setFrequency(float frequency, float sampleRate);

private:
    float m_currentAngle, m_angleDelta;

    float getNextSample();
    void updateAngle();
};