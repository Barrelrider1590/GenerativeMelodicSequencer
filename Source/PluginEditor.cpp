/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GenerativeMelodicSequencerAudioProcessorEditor::GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor& p)
    : AudioProcessorEditor (&p), m_audioProcessor (p),
    m_bpmKnobAttachment(*p.GetAPVTS(), "bpm", m_bpmKnob),
    m_loopLengthKnobAttachment(*p.GetAPVTS(), "length", m_loopLengthKnob),
    m_gateKnobAttachment(*p.GetAPVTS(), "gate", m_gateKnob),
    m_densityKnobAttachment(*p.GetAPVTS(), "density", m_densityKnob),
    m_mutateKnobAttachment(*p.GetAPVTS(), "mutate", m_mutateKnob),
    m_backgroundClr(juce::Colour(20, 20, 20))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_audioProcessor.AddListenerToBroadcaster(this);


    for (auto knob : GetComponents())
    {
        addAndMakeVisible(knob);
    }

    setSize (350, 600);

    auto bounds = getLocalBounds();
    auto midiEventArea = bounds.removeFromTop(bounds.getHeight() * .2f);
    midiEventArea.setWidth(midiEventArea.getWidth() / 7);

    for (int i{}; i < 7; ++i)
    {
        midiEventArea.setX(midiEventArea.getWidth() * i);
        NoteVisual note{ i, m_backgroundClr, midiEventArea };
        m_notes.push_back(note);
    }
}

GenerativeMelodicSequencerAudioProcessorEditor::~GenerativeMelodicSequencerAudioProcessorEditor()
{
    m_audioProcessor.RemoveListenerFromBroadcaster(this);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (m_backgroundClr);

    //g.setColour (juce::Colour(220, 220, 220));
    //g.setFont (15.0f);

    for (const NoteVisual& note : m_notes)
    {
        g.setColour(note.colour);
        g.fillRect(note.rect);
    }
}

void GenerativeMelodicSequencerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds{ getLocalBounds() };

    auto midiEventBounds{ bounds.removeFromTop( bounds.getHeight() * .2) };
    auto loopParamBounds{ bounds.removeFromTop( bounds.getHeight() * .33) };
    auto noteParamBounds{ bounds.removeFromTop( bounds.getHeight() * .5) };
    auto melodyParamBounds{ bounds };

    m_bpmKnob.setBounds(loopParamBounds.removeFromLeft(bounds.getWidth() * .5));
    m_loopLengthKnob.setBounds(loopParamBounds.removeFromRight(bounds.getWidth() * .5));
    
    m_gateKnob.setBounds(noteParamBounds.removeFromLeft(bounds.getWidth() * .5));
    m_densityKnob.setBounds(noteParamBounds.removeFromRight(bounds.getWidth() * .5));

    m_mutateKnob.setBounds(melodyParamBounds);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (m_audioProcessor.GetIsNoteOn())
    {
        m_notes[m_audioProcessor.GetCurrentNote() % 7].colour = RandomColour();
    }
    else
    {
        for (auto& note : m_notes)
        {
            note.colour = m_backgroundClr;
        }
    }
    repaint();
}

//==============================================================================
std::vector<juce::Component*> GenerativeMelodicSequencerAudioProcessorEditor::GetComponents()
{
    return
    {
        &m_bpmKnob,
        &m_loopLengthKnob,
        &m_gateKnob,
        &m_densityKnob,
        &m_mutateKnob
    };
}

juce::Colour GenerativeMelodicSequencerAudioProcessorEditor::RandomColour()
{
    juce::Random rand;
    juce::uint8 r{ static_cast<juce::uint8>(rand.nextInt()) };
    juce::uint8 g{ static_cast<juce::uint8>(rand.nextInt()) };
    juce::uint8 b{ static_cast<juce::uint8>(rand.nextInt()) };

    return juce::Colour(r, g, b);
}