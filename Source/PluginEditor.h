/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

struct RotaryKnob : public juce::Slider
{
    RotaryKnob() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                juce::Slider::NoTextBox)
    {

    }
};

class GenerativeMelodicSequencerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener
{
public:
    GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor&);
    ~GenerativeMelodicSequencerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GenerativeMelodicSequencerAudioProcessor& audioProcessor;

    std::vector<juce::Component*> getComponents();
    void RandomColour(juce::Colour& colour);

    RotaryKnob m_bpmKnob, m_loopLengthKnob;
    RotaryKnob m_gateKnob, m_densityKnob, m_mutateKnob;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment m_bpmKnobAttachment, m_loopLengthKnobAttachment;
    Attachment m_gateKnobAttachment, m_densityKnobAttachment, m_mutateKnobAttachment;

    juce::Colour m_colour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessorEditor)
};
