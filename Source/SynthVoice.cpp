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

    m_adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::prepareToPlay(int sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec{};
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    m_osc.prepare(spec);

    //m_osc.setFrequency(220.f);

    m_gain.prepare(spec);
    m_gain.setGainLinear(0.1f);

    m_adsr.setSampleRate(sampleRate);

    m_isPrepared = true;
}