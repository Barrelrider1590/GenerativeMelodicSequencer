/*
  ==============================================================================

    SynthSound.h
    Created: 5 May 2023 5:29:51pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};