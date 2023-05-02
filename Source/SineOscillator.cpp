/*
  ==============================================================================

    SineOscillator.cpp
    Created: 27 Apr 2023 6:59:06pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include "SineOscillator.h"
SineOscillator::SineOscillator() : m_currentAngle(0.f), m_angleDelta(0.f) {}

void SineOscillator::setFrequency(float frequency, float sampleRate)
{
    float cyclesPerSample{ frequency / sampleRate };
    m_angleDelta = cyclesPerSample * 2.f * juce::MathConstants<float>::pi;
}

float SineOscillator::getNextSample()
{
    float currentSample = std::sin(m_currentAngle);
    updateAngle();
    return currentSample;
}

void SineOscillator::updateAngle()
{
    m_currentAngle += m_angleDelta;
    if (m_currentAngle >= juce::MathConstants<float>::twoPi)
    {
        m_currentAngle -= juce::MathConstants<float>::twoPi;
    }
}