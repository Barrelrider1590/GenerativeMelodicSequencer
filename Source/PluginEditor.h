/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

struct NoteVisual 
{
    NoteVisual(int idx, juce::Colour clr, juce::Rectangle<int> bounds)
        : index(idx), colour(clr), rect(bounds) {}
    
    int index;
    juce::Colour colour;
    juce::Rectangle<int> rect;
};

struct RotaryKnob : public juce::Slider
{
    RotaryKnob() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::NoTextBox) {}
};
//==============================================================================
/**
*/
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

    std::vector<juce::Component*> GetComponents();

    juce::Colour RandomColour();

    //==============================================================================
    static int const m_maxNrOfNotes;
    static juce::Colour const m_backgroundClr;
    static CustomLookAndFeel m_lookAndFeel;
    
    GenerativeMelodicSequencerAudioProcessor& m_audioProcessor;

    std::vector<NoteVisual> m_notes;

    RotaryKnob m_bpmKnob;
    RotaryKnob m_loopLengthKnob;
    RotaryKnob m_gateKnob;
    RotaryKnob m_densityKnob;
    RotaryKnob m_mutateKnob;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment m_bpmKnobAttachment;
    Attachment m_loopLengthKnobAttachment;
    Attachment m_gateKnobAttachment;
    Attachment m_densityKnobAttachment;
    Attachment m_mutateKnobAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessorEditor)
};
