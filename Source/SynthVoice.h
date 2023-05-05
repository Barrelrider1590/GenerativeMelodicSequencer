/*
  ==============================================================================

    SynthVoice.h
    Created: 5 May 2023 5:29:34pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
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


private:
};