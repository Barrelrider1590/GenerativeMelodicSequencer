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
    NoteVisualiser(juce::ColourGradient gradient, juce::Colour bgClr) 
        : m_gradient(gradient), m_backgroundClr(bgClr), m_noteStartPos(0)
    {
        int nrOfNotes{ 12 };
        for (int i{}; i < nrOfNotes; ++i)
        {
            NoteVisual note{};
            note.colour = m_gradient.getColourAtPosition(static_cast<float>(i) / nrOfNotes);
            m_notes.push_back(note);
        }
    }
    //==============================================================================
    void setBounds(juce::Rectangle<int> newBounds)
    {
        int margin{ 12 };

        m_border = newBounds;

        m_bounds = newBounds;
        m_bounds.removeFromLeft(margin);
        m_bounds.removeFromRight(margin);
        m_bounds.removeFromTop(margin);
        m_bounds.removeFromBottom(margin);

        auto noteBounds = m_bounds;
        noteBounds.setWidth(m_bounds.getWidth() / 12);
        noteBounds.setHeight(m_bounds.getHeight() / 12);

        int counter{};
        for (auto& note : m_notes)
        {
            noteBounds.setX(noteBounds.getWidth()  * counter + margin);
            m_noteStartPos = m_border.getHeight() - noteBounds.getHeight();
            noteBounds.setY(m_border.getHeight() - noteBounds.getHeight());
            note.rect = noteBounds;
            note.offsetTotal = m_noteStartPos;
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

        g.setColour(juce::Colours::rebeccapurple.darker(.5f));
        g.drawRect(m_border, 10.f);
    }

    //==============================================================================
    void UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p, juce::Colour random)
    {
        int noteChanged{ p.GetCurrentMidiNote() - p.GetScale()[0] };
        m_notes[noteChanged].colour = m_gradient.getColourAtPosition( static_cast<float>(noteChanged) / m_notes.size());
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
                    note.rect.setY(m_noteStartPos);
                    note.isActive = false;
                    note.offsetTotal = m_noteStartPos;
                    note.colour = juce::Colour(.0f, .0f, .0f, .0f);
                }
            }
        }
    }

private:
    int m_noteStartPos;
    juce::Rectangle<int> m_bounds;
    juce::Rectangle<int> m_border;
    std::vector<NoteVisual> m_notes;
    juce::ColourGradient m_gradient;
    juce::Colour m_backgroundClr;
    
};