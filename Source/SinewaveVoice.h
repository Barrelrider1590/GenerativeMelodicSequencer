/*
  ==============================================================================

    SynthVoice.h
    Created: 5 May 2023 5:29:34pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:

    bool canPlaySound(juce::SynthesiserSound* sound)                    override;
    void startNote(int midiNoteNumber,
        float velocity,
        juce::SynthesiserSound* sound,
        int currentPitchWheelPosition)                                  override;
    void stopNote(float velocity, bool allowTailOff)                    override;
    void pitchWheelMoved(int newPitchWheelValue)                        override;
    void controllerMoved(int controllerNumber, int newControllerValue)  override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
        int startSample,
        int numSamples)                                override;
    //===========================================================================
    void prepareToPlay(int sampleRate, int samplesPerBlock, int outputChannels);

private:
    juce::dsp::Oscillator<float> m_osc{ [](float x) { return std::sin(x); } };
    juce::dsp::Gain<float> m_gain;

    juce::ADSR m_adsr;
    juce::ADSR::Parameters m_adsrParams;

    bool m_isPrepared = false;

};

// wave shape formula
// sine: return std::sin(x);
// saw: return x / juce::MathConstants<float>::pi;
// square: return x < 0.0f ? -1.0f : 1.0f;