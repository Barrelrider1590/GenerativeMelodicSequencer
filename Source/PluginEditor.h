/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "NoteVisualiser.h"

struct RotaryKnob : public juce::Slider
{
    RotaryKnob() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::NoTextBox) {}
};
//==============================================================================
/**
*/
class GenerativeMelodicSequencerAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                                        public juce::ChangeListener, 
                                                        public juce::Timer
{
public:
    GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor&);
    ~GenerativeMelodicSequencerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;

private:

    std::vector<juce::Component*> GetComponents();

    juce::Colour RandomColour();

    //==============================================================================
    static int const m_maxNrOfNotes;

    GenerativeMelodicSequencerAudioProcessor& m_audioProcessor;

    int const m_timerFreq;

    juce::Atomic<bool> m_midiUpdated;

    CustomLookAndFeel m_lookAndFeel;

    NoteVisualiser m_noteVisualiser;

    juce::TextButton m_lockToggle;
    juce::TextButton m_randomiseBtn;

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
