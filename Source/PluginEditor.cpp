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
    m_mutateKnobAttachment(*p.GetAPVTS(), "mutate", m_mutateKnob)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_audioProcessor.AddListenerToBroadcaster(this);


    for (auto knob : GetComponents())
    {
        addAndMakeVisible(knob);
    }

    setSize (350, 600);
}

GenerativeMelodicSequencerAudioProcessorEditor::~GenerativeMelodicSequencerAudioProcessorEditor()
{
    m_audioProcessor.RemoveListenerFromBroadcaster(this);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour (20,20,20));

    g.setColour (juce::Colour(220, 220, 220));
    g.setFont (15.0f);
    
    auto bounds{ getLocalBounds() };
    auto midiEventBounds{ bounds.removeFromTop(bounds.getHeight() * .2) };

    g.setColour(m_colour);
    g.fillRect(midiEventBounds);
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
        RandomColour(m_colour);
    }
    else
    {
        m_colour = juce::Colours::black;
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

void GenerativeMelodicSequencerAudioProcessorEditor::RandomColour(juce::Colour& colour)
{
    juce::Random rand;
    juce::uint8 r{ static_cast<juce::uint8>(rand.nextInt()) };
    juce::uint8 g{ static_cast<juce::uint8>(rand.nextInt()) };
    juce::uint8 b{ static_cast<juce::uint8>(rand.nextInt()) };

    colour = juce::Colour(r, g, b);
}