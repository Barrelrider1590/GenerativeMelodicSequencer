/*
  ==============================================================================

    Utility.h
    Created: 16 Jun 2023 8:05:05am
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
        m_colourActive(juce::Colours::green),
        m_colourInactive(juce::Colours::red),
        m_rect() {}

    //==============================================================================
    int m_noteNr;
    int m_index;
    bool m_isActive;
    float m_offsetTotal;
    juce::Colour m_colourActive;
    juce::Colour m_colourInactive;
    juce::Rectangle<int> m_rect;
};

struct SequencerSettings
{
    int scale{ 0 }, bpm{ 0 }, loopLength{ 0 };
    float gate{ 0 }, density{ 0 }, mutate{ 0 };
};

struct Vector2f
{
    float x;
    float y;
};