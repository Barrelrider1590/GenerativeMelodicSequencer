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
        noteNr(0), 
        index(0),
        isActive(false), 
        offsetTotal(0), 
        colour(juce::Colours::black) {}

    //==============================================================================
    int noteNr;
    int index;
    bool isActive;
    float offsetTotal;
    juce::Colour colour;
    juce::Rectangle<int> rect;
};


class NoteVisualiser : public juce::Component
{
public:
    NoteVisualiser(int nrOfNotes, const juce::ColourGradient& gradient, const juce::Colour& bgClr) :
        m_noteStartPos(0),
        m_nrOfDuplicates(5),
        m_notesVect(),
        m_border(),
        m_bounds(),
        m_gradient(gradient), 
        m_backgroundClr(bgClr)
    {
        m_notesVect.resize(nrOfNotes * m_nrOfDuplicates);

        for (int i{}; i < m_notesVect.size(); ++i)
        {
            NoteVisual note{};
            note.noteNr = i / m_nrOfDuplicates;
            note.index = i % m_nrOfDuplicates;
            note.colour = m_gradient.getColourAtPosition(static_cast<float>(note.noteNr) / nrOfNotes);
            m_notesVect[i] = note;
        }
    }

    //==============================================================================
    void setBounds(const juce::Rectangle<int>& newBounds)
    {
        m_border = newBounds;

        int margin{ 12 };
        m_bounds = newBounds;
        m_bounds.removeFromLeft(margin);
        m_bounds.removeFromRight(margin);
        m_bounds.removeFromTop(margin);
        m_bounds.removeFromBottom(margin);

        juce::Rectangle<int> noteBounds = m_bounds;
        noteBounds.setWidth(m_bounds.getWidth() / 12);
        noteBounds.setHeight(m_bounds.getHeight() / 12);

        for (NoteVisual& note : m_notesVect)
        {
            noteBounds.setX(noteBounds.getWidth()  * note.noteNr + margin);
            m_noteStartPos = m_border.getHeight() - noteBounds.getHeight();
            noteBounds.setY(m_noteStartPos);
            note.rect = noteBounds;
            note.offsetTotal = m_noteStartPos;
        }
    }
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour(juce::Colours::black));
        g.fillRect(m_bounds);

        for (const NoteVisual& note : m_notesVect)
        {
            g.setColour(note.colour);
            g.fillRect(note.rect);
        }

        g.setColour(juce::Colours::rebeccapurple.darker(.5f));
        g.drawRect(m_border, 10.f);
    }

    //==============================================================================
    void UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p)
    {
        int noteChanged{ p.GetCurrentMidiNote() - p.GetScale()[0] };
        int noteIndex = noteChanged * m_nrOfDuplicates;
        m_notesVect[noteIndex].colour = m_gradient.getColourAtPosition( static_cast<float>(noteChanged) / (m_notesVect.size() / m_nrOfDuplicates));
        if (!m_notesVect[noteIndex].isActive)
        {
            m_notesVect[noteIndex].isActive = true;
        }
        else
        {
            bool activatedDuplicate{ false };
            for (int i{1}; i < m_nrOfDuplicates; ++i)
            {
                if (!m_notesVect[noteIndex + i].isActive)
                {
                    m_notesVect[noteIndex + i].isActive = true;
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
        for (NoteVisual& note : m_notesVect)
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
                }
            }
        }
    }

private:
    int m_noteStartPos;
    int m_nrOfDuplicates;
    std::vector<NoteVisual> m_notesVect;
    juce::Rectangle<int> m_border;
    juce::Rectangle<int> m_bounds;
    juce::ColourGradient m_gradient;
    juce::Colour m_backgroundClr;
};