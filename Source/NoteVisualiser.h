/*
  ==============================================================================

    NoteVisualiser.h
    Created: 8 Jun 2023 1:26:18pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct NoteVisual
{
    NoteVisual() {}
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

    //==============================================================================
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

    //==============================================================================
    void UpdateNoteVisuals(GenerativeMelodicSequencerAudioProcessor& p, juce::Colour random)
    {
        int noteChanged{ p.GetCurrentMidiNote() - p.GetScale()[0] };
        m_notes[noteChanged].colour = random;
    }

private:
    juce::Rectangle<int> m_rect;
    std::vector<NoteVisual> m_notes;
};