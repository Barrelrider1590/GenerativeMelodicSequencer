/*
  ==============================================================================

    SynthVoice.cpp
    Created: 5 May 2023 5:29:34pm
    Author:  Phoenix

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{

}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{

}
void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(m_isPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };

    m_osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void SynthVoice::prepareToPlay(int sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec{};
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    m_osc.prepare(spec);

    m_osc.setFrequency(220.f);

    m_gain.prepare(spec);
    m_gain.setGainLinear(0.1f);

    bool m_isPrepared = true;
}