/*
  ==============================================================================

    SynthVoice.cpp
    Created: 5 May 2023 5:29:34pm
    Author:  Phoenix

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    double midiFreq{ juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) };
    m_osc.setFrequency(midiFreq);

    m_adsr.noteOn();
}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    m_adsr.noteOff();

    if (!allowTailOff || !m_adsr.isActive())
    {
        clearCurrentNote();
    }
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

    if (!isVoiceActive())
    {
        return;
    }

    //int outBuffer{ outputBuffer.getNumSamples() };
    //jassert(outputBuffer.getNumSamples() == numSamples);

    m_buffer.setSize(outputBuffer.getNumChannels(),
        numSamples,
        false,
        false,
        true);

    m_buffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ m_buffer };

    m_osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    m_adsr.applyEnvelopeToBuffer(m_buffer, 0, m_buffer.getNumSamples());

    for (int channel{ 0 }; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, m_buffer, channel, 0, numSamples);

        if (!m_adsr.isActive())
        {
            clearCurrentNote();
        }
    }
}

void SynthVoice::prepareToPlay(int sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec{};
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    m_osc.prepare(spec);

    m_gain.prepare(spec);
    m_gain.setGainLinear(0.1f);

    m_adsr.setSampleRate(sampleRate);
    m_adsrParams.attack = 1.0f;
    m_adsrParams.decay = 1.0f;
    m_adsrParams.sustain = 1.0f;
    m_adsrParams.release = 1.0f;
    m_adsr.setParameters(m_adsrParams);

    m_isPrepared = true;
}