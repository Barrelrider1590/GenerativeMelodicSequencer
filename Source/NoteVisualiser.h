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
    NoteVisual() : isActive(false), offsetTotal(0), colour(juce::Colours::black) {}
    //==============================================================================
    bool isActive;
    float offsetTotal;
    juce::Colour colour;
    juce::Rectangle<int> rect;
};


class NoteVisualiser : public juce::Component
{
public:
    NoteVisualiser(juce::Colour bgClr) : m_backgroundClr(bgClr)
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
        m_bounds = newBounds;
        m_bounds.removeFromBottom(10);
        m_bounds.removeFromLeft(10);
        m_bounds.removeFromRight(10);
        m_bounds.removeFromTop(10);
        auto noteBounds = m_bounds;
        noteBounds.setWidth(m_bounds.getWidth() / 12);
        noteBounds.setHeight(m_bounds.getHeight() / 12);
        int counter{};
        for (auto& note : m_notes)
        {
            noteBounds.setX(noteBounds.getWidth() * counter + 10);
            noteBounds.setY(m_bounds.getHeight() - noteBounds.getHeight());
            note.rect = noteBounds;
            note.offsetTotal = m_bounds.getHeight();
            ++counter;
        }
    }
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour(juce::Colours::black));
        g.fillRect(m_bounds);

        for (const auto& note : m_notes)
        {
            g.setColour(note.colour);
            g.fillRect(note.rect);
        }
    }

    //==============================================================================
    void UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p, juce::Colour random)
    {
        int noteChanged{ p.GetCurrentMidiNote() - p.GetScale()[0] };
        m_notes[noteChanged].colour = random;
        m_notes[noteChanged].isActive = true;
    }
    void UpdateNotePosition(int timerFreqHz)
    {
        float delta = 150.f / timerFreqHz;
        for (auto& note : m_notes)
        {
            if (note.isActive)
            {
                note.offsetTotal -= delta;
                note.rect.setY(note.offsetTotal);
                if (note.offsetTotal <= 0 - note.rect.getHeight())
                {
                    note.rect.setY(m_bounds.getHeight());
                    note.isActive = false;
                    note.offsetTotal = m_bounds.getHeight();
                    note.colour = juce::Colour(.0f, .0f, .0f, .0f);
                }
            }
        }
    }

private:
    juce::Rectangle<int> m_bounds;
    std::vector<NoteVisual> m_notes;
    juce::Colour m_backgroundClr;
};