/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
const int GenerativeMelodicSequencerAudioProcessorEditor::m_maxNrOfNotes{ 12 };

GenerativeMelodicSequencerAudioProcessorEditor::GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_timerFreq(60),
    m_midiUpdated(false),
    m_lookAndFeel(),
    m_noteVisualiser(m_maxNrOfNotes, m_lookAndFeel.GetGradient(), m_lookAndFeel.GetBackgroundColour()),
    m_bpmKnobAttachment(*p.GetAPVTS(), "bpm", m_bpmKnob),
    m_loopLengthKnobAttachment(*p.GetAPVTS(), "length", m_loopLengthKnob),
    m_gateKnobAttachment(*p.GetAPVTS(), "gate", m_gateKnob),
    m_densityKnobAttachment(*p.GetAPVTS(), "density", m_densityKnob),
    m_mutateKnobAttachment(*p.GetAPVTS(), "mutate", m_mutateKnob)    
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_audioProcessor.AddListenerToBroadcaster(this);
    
    startTimerHz(m_timerFreq);

    addAndMakeVisible(m_noteVisualiser);

    for (auto knob : GetComponents())
    {
        knob->setLookAndFeel(&m_lookAndFeel);
        addAndMakeVisible(knob);
    }

    setSize (360, 720);
}

GenerativeMelodicSequencerAudioProcessorEditor::~GenerativeMelodicSequencerAudioProcessorEditor()
{
    m_audioProcessor.RemoveListenerFromBroadcaster(this);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (m_lookAndFeel.GetBackgroundColour());

    m_noteVisualiser.paint(g);
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

    m_noteVisualiser.setBounds(midiEventBounds);

    m_bpmKnob.setBounds(loopParamBounds.removeFromLeft(bounds.getWidth() * .5));
    m_loopLengthKnob.setBounds(loopParamBounds.removeFromRight(bounds.getWidth() * .5));
    
    m_gateKnob.setBounds(noteParamBounds.removeFromLeft(bounds.getWidth() * .5));
    m_densityKnob.setBounds(noteParamBounds.removeFromRight(bounds.getWidth() * .5));

    m_mutateKnob.setBounds(melodyParamBounds);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    m_midiUpdated = true;
}

void GenerativeMelodicSequencerAudioProcessorEditor::timerCallback()
{
    if (m_midiUpdated.compareAndSetBool(false, true))
    {
        m_noteVisualiser.UpdateNoteVisibility(m_audioProcessor);
    }

    m_noteVisualiser.UpdateNotePosition(m_timerFreq);

    repaint(getLocalBounds().removeFromTop(getLocalBounds().getHeight() * .2));
    //repaint();
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