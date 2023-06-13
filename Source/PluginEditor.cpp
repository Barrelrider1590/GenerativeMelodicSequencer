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
    m_timerFreq(30),
    m_midiUpdated(false),
    m_lookAndFeel(),
    m_noteVisualiser(m_maxNrOfNotes, m_lookAndFeel.GetGradient(), m_lookAndFeel.GetBackgroundColour()),
    m_lockToggle(),
    m_randomiseBtn("Randomise"),
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

    for (juce::Component* component : GetComponents())
    {
        component->setLookAndFeel(&m_lookAndFeel);
        addAndMakeVisible(component);
    }

    auto generateMelody{ [this]() { m_audioProcessor.ResetMelody();  } };
    
    m_randomiseBtn.onClick = generateMelody;

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

    auto midiEventBounds{ bounds.removeFromTop( bounds.getHeight() * .2f) };
    auto buttonBounds{ bounds.removeFromTop(bounds.getHeight() * .2f) };
    auto loopParamBounds{ bounds.removeFromTop( bounds.getHeight() * .33f) };
    auto noteParamBounds{ bounds.removeFromTop( bounds.getHeight() * .5f) };
    auto melodyParamBounds{ bounds };

    m_noteVisualiser.setBounds(midiEventBounds);

    //buttonBounds = buttonBounds.withSizeKeepingCentre(buttonBounds.getHeight() * .5f, buttonBounds.getHeight());
    auto lockBnds{ buttonBounds.removeFromTop(buttonBounds.getHeight() * .33f) };
    auto randBnds{ buttonBounds };

    lockBnds = lockBnds.reduced(5);
    m_lockToggle.setBounds(lockBnds.withSizeKeepingCentre(lockBnds.getHeight(), lockBnds.getHeight()));

    m_randomiseBtn.setBounds(randBnds.removeFromTop(randBnds.getHeight() * .5f));
    m_randomiseBtn.setBounds(m_randomiseBtn.getBounds().withSizeKeepingCentre(randBnds.getHeight(), randBnds.getHeight()));
    m_randomiseBtn.SetLabelBounds(randBnds);

    loopParamBounds = loopParamBounds.reduced(10);
    auto bpmBnds{ loopParamBounds.removeFromLeft(bounds.getWidth() * .5f) };
    m_bpmKnob.setBounds(bpmBnds.removeFromTop(bpmBnds.getHeight() * .9f));
    m_bpmKnob.SetLabelBounds(bpmBnds);
    auto loopLengthBnds{ loopParamBounds };
    m_loopLengthKnob.setBounds(loopLengthBnds.removeFromTop(loopLengthBnds.getHeight() * .9f));
    m_loopLengthKnob.SetLabelBounds(loopLengthBnds);
    
    noteParamBounds = noteParamBounds.reduced(10);
    auto gateBnds{ noteParamBounds.removeFromLeft(bounds.getWidth() * .5f) };
    m_gateKnob.setBounds(gateBnds.removeFromTop(gateBnds.getHeight() * .9f));
    m_gateKnob.SetLabelBounds(gateBnds);
    auto densityBnds{ noteParamBounds.removeFromRight(bounds.getWidth() * .5f) };
    m_densityKnob.setBounds(densityBnds.removeFromTop(densityBnds.getHeight() * .9f));
    m_densityKnob.SetLabelBounds(densityBnds);

    auto mutateBnds{ melodyParamBounds.reduced(10) };
    m_mutateKnob.setBounds(mutateBnds.removeFromTop(mutateBnds.getHeight() * .9f));
    m_mutateKnob.SetLabelBounds(mutateBnds);
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
        &m_noteVisualiser,
        &m_lockToggle,
        &m_randomiseBtn,
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