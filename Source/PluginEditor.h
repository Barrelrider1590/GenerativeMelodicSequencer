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
    NoteVisual(){}
    NoteVisual(juce::Colour clr, juce::Rectangle<int> bounds)
        : colour(clr), rect(bounds) {}

    //==============================================================================
    juce::Colour colour{ juce::Colours::red };
    juce::Rectangle<int> rect;
};

class NoteVisualiser : public juce::Component
{
public:
    NoteVisualiser()
    {
        for (int i{}; i < 12; ++i)
        {
            NoteVisual note{};
            m_notes.push_back(note);
        }
    }
    void setBounds(juce::Rectangle<int> newBounds)
    {
        m_rect = newBounds;
        auto noteBounds = m_rect;
        noteBounds.setWidth(m_rect.getWidth() / 12);
        noteBounds.setHeight(m_rect.getHeight() / 12);
        int counter{};
        for (auto& note : m_notes)
        {
            noteBounds.setX(noteBounds.getWidth() * counter);
            noteBounds.setY((m_rect.getHeight() - noteBounds.getHeight()) - (noteBounds.getHeight() * counter));
            note.rect = noteBounds;
            ++counter;
        }
    }
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour(juce::Colours::black));
        g.fillRect(m_rect);
        int counter{};
        for (const auto& note : m_notes)
        {
            g.setColour(note.colour);
            g.fillRect(note.rect);
            ++counter;
        }
    }

    void UpdateNoteVisuals(GenerativeMelodicSequencerAudioProcessor& p, juce::Colour random)
    {
        int noteChanged{ p.GetCurrentMidiNote() - p.GetScale()[0]};
        m_notes[noteChanged].colour = random;
    }

private:
    juce::Rectangle<int> m_rect;
    std::vector<NoteVisual> m_notes;
};

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
    static juce::Colour const m_backgroundClr;
    static CustomLookAndFeel m_lookAndFeel;
    
    GenerativeMelodicSequencerAudioProcessor& m_audioProcessor;

    juce::Atomic<bool> m_midiEventThrown;

    NoteVisualiser m_noteVisualiser;

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
