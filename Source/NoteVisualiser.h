/*
  ==============================================================================

    NoteVisualiser.h
    Created: 8 Jun 2023 1:26:18pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Utility.h"

class NoteVisualiser : public juce::Component
{
public:
    NoteVisualiser(int nrOfNotes, const juce::ColourGradient& gradient, const juce::Colour& bgClr);
    //==============================================================================
    void setBounds(const juce::Rectangle<int>& newBounds);
    void resized();
    void paint(juce::Graphics& g) override;
    //==============================================================================
    void UpdateNoteVisibility(GenerativeMelodicSequencerAudioProcessor& p);
    void UpdateNotePosition(int timerFreqHz);

private:
    int m_noteStartPos;
    int m_nrOfDuplicates;
    std::vector<std::unique_ptr<NoteVisual>> m_notesVect;
    std::vector<juce::String> m_noteNamesVect;
    juce::Rectangle<int> m_border;
    juce::Rectangle<int> m_bounds;
    float m_margin;
    juce::ColourGradient m_gradient;
    juce::Colour m_backgroundClr;
};