/*
  ==============================================================================

    NoteVisualiser.cpp
    Created: 16 Jun 2023 9:02:11am
    Author:  Phoenix

  ==============================================================================
*/

#include "NoteVisualiser.h"

NoteVisualiser::NoteVisualiser(const juce::ColourGradient& gradient, const juce::Colour& bgClr) :
    m_noteStartPos(0),
    m_nrOfDuplicates(8),
    m_notesVect(),
    m_noteNamesVect({ "c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b" }),
    m_border(),
    m_bounds(),
    m_margin(0.f),
    m_gradient(gradient),
    m_backgroundClr(bgClr)
{
    int nrOfNotes(14);
    m_notesVect.resize(nrOfNotes * m_nrOfDuplicates);

    for (int i{}; i < m_notesVect.size(); ++i)
    {
        m_notesVect[i] = std::make_unique<NoteVisual>();
        m_notesVect[i]->m_noteNr = i / m_nrOfDuplicates;
        m_notesVect[i]->m_index = i % m_nrOfDuplicates;
        m_notesVect[i]->m_colourActive = m_gradient.getColourAtPosition(static_cast<float>(m_notesVect[i]->m_noteNr) / nrOfNotes);
    }
}
//==============================================================================
void NoteVisualiser::setBounds(const juce::Rectangle<int>& newBounds)
{
    m_border = newBounds;

    m_bounds = newBounds;
    m_margin = juce::jmin(m_bounds.getWidth(), m_bounds.getHeight()) * .1f;
    m_bounds.reduce(m_margin, m_margin);

    juce::Rectangle<int> noteBounds = m_bounds;
    noteBounds.setWidth(m_bounds.getWidth() / static_cast<float>(m_notesVect.size() / m_nrOfDuplicates));
    noteBounds.setHeight(m_bounds.getHeight() / static_cast<float>(m_nrOfDuplicates));

    for (auto& note : m_notesVect)
    {
        noteBounds.setX(noteBounds.getWidth() * note->m_noteNr + m_margin);
        m_noteStartPos = m_border.getHeight() - noteBounds.getHeight();
        noteBounds.setY(m_noteStartPos);
        note->m_rect = noteBounds;
        note->m_offsetTotal = m_noteStartPos;
    }
}

void NoteVisualiser::resized(){}

void NoteVisualiser::paint(juce::Graphics& g)
{
    g.setColour(m_backgroundClr.darker(.2f));
    g.fillRect(m_bounds);

    float maxNoteWidth{ m_bounds.getWidth() / static_cast<float>(m_notesVect.size() / m_nrOfDuplicates) };

    for (const auto& note : m_notesVect)
    {
        g.setColour((note->m_isActive ? note->m_colourActive : note->m_colourInactive));
        g.fillRect(note->m_rect);
        
        if (note->m_index == 0)
        {
            juce::Line<float> line{ maxNoteWidth * note->m_noteNr + m_margin, 0.f,
                                    maxNoteWidth * note->m_noteNr + m_margin, m_bounds.getHeight() + m_margin };
            g.setColour(note->m_colourActive.brighter(.2f));
            g.drawLine(line, 1.f);

            g.drawFittedText(m_noteNamesVect[note->m_noteNr % 12],
                             maxNoteWidth * note->m_noteNr + m_margin, m_margin,
                             maxNoteWidth, m_notesVect[0]->m_rect.getHeight(),
                             juce::Justification::centred, 1);
        }
    }

    g.setColour(juce::Colours::rebeccapurple.darker(.2f));
    g.drawRect(m_border, m_margin);
}

//==============================================================================
void NoteVisualiser::UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p)
{
    int noteChanged{ p.GetCurrentMidiNote() };
    int noteIndex{ noteChanged * m_nrOfDuplicates };
    float noteHeight{ m_bounds.getHeight() / static_cast<float>(m_nrOfDuplicates) * p.GetSequencerSettings(*p.GetAPVTS()).gate };
    float noteDensity{ p.GetSequencerSettings(*p.GetAPVTS()).density };
    juce::Colour baseColour{m_gradient.getColourAtPosition(noteChanged / static_cast<float>(m_notesVect.size() / m_nrOfDuplicates))};


    if (!m_notesVect[noteIndex]->m_isActive)
    {
        m_notesVect[noteIndex]->m_isActive = true;
        m_notesVect[noteIndex]->m_rect.setHeight(noteHeight);
        m_notesVect[noteIndex]->m_colourActive = baseColour.darker(.75f - (noteDensity * .75f));
    }
    else
    {
        bool activatedDuplicate{ false };
        for (int i{ 1 }; i < m_nrOfDuplicates; ++i)
        {
            int noteDuplicate{ noteIndex + i };
            if (!m_notesVect[noteDuplicate]->m_isActive)
            {
                m_notesVect[noteDuplicate]->m_isActive = true;
                m_notesVect[noteDuplicate]->m_rect.setHeight(noteHeight);
                m_notesVect[noteDuplicate]->m_colourActive = baseColour.darker(.75f - (noteDensity * .75f));
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
void NoteVisualiser::UpdateNotePosition(int timerFreqHz)
{
    float maxNoteWidth{ m_bounds.getWidth() / static_cast<float>(m_notesVect.size() / m_nrOfDuplicates) };
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
