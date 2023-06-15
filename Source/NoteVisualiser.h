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
    NoteVisual() : 
        m_noteNr(0),
        m_index(0),
        m_isActive(false),
        m_offsetTotal(0),
        m_colour(juce::Colours::black),
        m_rect() {}

    //==============================================================================
    int m_noteNr;
    int m_index;
    bool m_isActive;
    float m_offsetTotal;
    juce::Colour m_colour;
    juce::Rectangle<int> m_rect;
};


class NoteVisualiser : public juce::Component
{
public:
    NoteVisualiser(int nrOfNotes, const juce::ColourGradient& gradient, const juce::Colour& bgClr) :
        m_noteStartPos(0),
        m_nrOfDuplicates(8),
        m_notesVect(),
        m_border(),
        m_bounds(),
        m_margin(0.f),
        m_gradient(gradient), 
        m_backgroundClr(bgClr)
    {
        m_notesVect.resize(nrOfNotes * m_nrOfDuplicates);

        for (int i{}; i < m_notesVect.size(); ++i)
        {
            m_notesVect[i] = std::make_unique<NoteVisual>();
            m_notesVect[i]->m_noteNr = i / m_nrOfDuplicates;
            m_notesVect[i]->m_index = i % m_nrOfDuplicates;
            m_notesVect[i]->m_colour = m_gradient.getColourAtPosition(static_cast<float>(m_notesVect[i]->m_noteNr) / nrOfNotes);
        }
    }
    //==============================================================================
    void setBounds(const juce::Rectangle<int>& newBounds)
    {
        m_border = newBounds;

        m_bounds = newBounds;
        m_margin = juce::jmin(m_bounds.getWidth(), m_bounds.getHeight()) * .1f;
        m_bounds.reduce(m_margin, m_margin);

        juce::Rectangle<int> noteBounds = m_bounds;
        noteBounds.setWidth(m_bounds.getWidth() / 12);
        noteBounds.setHeight(m_bounds.getHeight() / 12);

        for (auto& note : m_notesVect)
        {
            noteBounds.setX(noteBounds.getWidth()  * note->m_noteNr + m_margin);
            m_noteStartPos = m_border.getHeight() - noteBounds.getHeight();
            noteBounds.setY(m_noteStartPos);
            note->m_rect = noteBounds;
            note->m_offsetTotal = m_noteStartPos;
        }
    }
    void paint(juce::Graphics& g) override
    {
        g.setColour(m_backgroundClr.darker(.2f));
        g.fillRect(m_bounds);

        for (const auto& note : m_notesVect)
        {
            g.setColour(note->m_colour);
            g.fillRect(note->m_rect);
        }

        g.setColour(juce::Colours::rebeccapurple.darker(.2f));
        g.drawRect(m_border, m_margin);
    }

    //==============================================================================
    void UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p)
    {
        int noteChanged{ p.GetCurrentMidiNote() };
        int noteIndex = noteChanged * m_nrOfDuplicates;
        m_notesVect[noteIndex]->m_colour = m_gradient.getColourAtPosition( static_cast<float>(noteChanged) / (m_notesVect.size() / m_nrOfDuplicates));
        if (!m_notesVect[noteIndex]->m_isActive)
        {
            m_notesVect[noteIndex]->m_isActive = true;
        }
        else
        {
            bool activatedDuplicate{ false };
            for (int i{1}; i < m_nrOfDuplicates; ++i)
            {
                if (!m_notesVect[noteIndex + i]->m_isActive)
                {
                    m_notesVect[noteIndex + i]->m_isActive = true;
                    activatedDuplicate = true;
                    break;
                }
            }
            if (!activatedDuplicate)
            {
                jassertfalse;
            }
        }
    }
    void UpdateNotePosition(int timerFreqHz)
    {
        float delta = 150.f / timerFreqHz;
        for (auto& note : m_notesVect)
        {
            if (note->m_isActive)
            {
                note->m_offsetTotal -= delta;
                note->m_rect.setY(note->m_offsetTotal);
                if (note->m_offsetTotal <= 0 - note->m_rect.getHeight())
                {
                    note->m_rect.setY(m_noteStartPos);
                    note->m_isActive = false;
                    note->m_offsetTotal = m_noteStartPos;
                }
            }
        }
    }

private:
    int m_noteStartPos;
    int m_nrOfDuplicates;
    std::vector<std::unique_ptr<NoteVisual>> m_notesVect;
    juce::Rectangle<int> m_border;
    juce::Rectangle<int> m_bounds;
    float m_margin;
    juce::ColourGradient m_gradient;
    juce::Colour m_backgroundClr;
};