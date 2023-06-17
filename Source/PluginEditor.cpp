/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GenerativeMelodicSequencerAudioProcessorEditor::GenerativeMelodicSequencerAudioProcessorEditor(GenerativeMelodicSequencerAudioProcessor& p) :
    AudioProcessorEditor(&p),
    m_audioProcessor(p),
    m_timerFreq(30),
    m_midiUpdated(false),
    m_lookAndFeel(),
    m_noteVisualiser(m_lookAndFeel.GetGradient(), m_lookAndFeel.GetBackgroundColour()),
    m_lockToggle(),
    m_randomiseBtn(),
    m_randomiseLbl(m_randomiseBtn, "Randomise"),
    m_scaleKnobAttachment(*p.GetAPVTS(), "scale", m_scaleKnob),
    m_scaleLbl(m_scaleKnob, "Scale"),
    m_bpmKnobAttachment(*p.GetAPVTS(), "bpm", m_bpmKnob),
    m_bpmLbl(m_bpmKnob, "BPM"),
    m_loopLengthKnobAttachment(*p.GetAPVTS(), "length", m_loopLengthKnob),
    m_loopLengthLbl(m_loopLengthKnob, "Length"),
    m_gateKnobAttachment(*p.GetAPVTS(), "gate", m_gateKnob),
    m_gateLbl(m_gateKnob, "Gate"),
    m_densityKnobAttachment(*p.GetAPVTS(), "density", m_densityKnob),
    m_densityLbl(m_densityKnob, "Density"),
    m_mutateKnobAttachment(*p.GetAPVTS(), "mutate", m_mutateKnob),
    m_mutateLbl(m_mutateKnob, "Mutate")
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
    for (ComponentLabel* l : GetLabels())
    {
        l->InitialiseLabel();
    }

    auto generateMelody{ [this]() { m_audioProcessor.ResetMelody();  } };
    
    m_randomiseBtn.onClick = generateMelody;

    setSize (360, 900);
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
    auto buttonBounds{ bounds.removeFromTop(bounds.getHeight() * .1f) };
    auto loopParamBounds{ bounds.removeFromTop( bounds.getHeight() * .5f) };
    auto noteParamBounds{ bounds.removeFromTop( bounds.getHeight() * .5f) };
    auto melodyParamBounds{ bounds };

    m_noteVisualiser.setBounds(midiEventBounds);

    //buttonBounds = buttonBounds.withSizeKeepingCentre(buttonBounds.getHeight() * .5f, buttonBounds.getHeight());
    
    //auto lockBnds{ buttonBounds.removeFromTop(buttonBounds.getHeight() * .33f) };
    //lockBnds = lockBnds.reduced(5);
    //m_lockToggle.setBounds(lockBnds.withSizeKeepingCentre(lockBnds.getHeight(), lockBnds.getHeight()));

    float margin{ juce::jmin(buttonBounds.getWidth(), buttonBounds.getHeight()) * .1f };
    buttonBounds.reduce(margin, margin);
    m_randomiseBtn.setBounds(buttonBounds.removeFromTop(buttonBounds.getHeight() * .6f));
    m_randomiseBtn.setBounds(m_randomiseBtn.getBounds().withSizeKeepingCentre(m_randomiseBtn.getBounds().getHeight(),
                                                                              m_randomiseBtn.getBounds().getHeight()));
    m_randomiseLbl.setBounds(buttonBounds);

    //loopParamBounds = loopParamBounds.reduced(10);
    //auto loopParamTop{ loopParamBounds.removeFromBottom(loopParamBounds.getHeight() * .5f) };
    //auto bpmBnds{ loopParamBounds.removeFromLeft(loopParamBounds.getWidth() * .5f) };
    //m_bpmKnob.setBounds(bpmBnds.removeFromTop(bpmBnds.getHeight() * .85f));
    //m_bpmLbl.setBounds(bpmBnds);
    //auto loopLengthBnds{ loopParamBounds };
    //m_loopLengthKnob.setBounds(loopLengthBnds.removeFromTop(loopLengthBnds.getHeight() * .85f));
    //m_loopLengthLbl.setBounds(loopLengthBnds);

    loopParamBounds.reduce(10, 10);
    auto LoopTopBnds{ loopParamBounds.removeFromTop(loopParamBounds.getHeight() * .5f) };
    auto bpmBnds{ LoopTopBnds.removeFromLeft(LoopTopBnds.getWidth() * .5f) };
    m_bpmKnob.setBounds(bpmBnds.removeFromTop(bpmBnds.getHeight() * .85f));
    m_bpmLbl.setBounds(bpmBnds);

    auto loopLengthBnds{ LoopTopBnds };
    m_loopLengthKnob.setBounds(loopLengthBnds.removeFromTop(loopLengthBnds.getHeight() * .85f));
    m_loopLengthLbl.setBounds(loopLengthBnds);

    m_scaleKnob.setBounds(loopParamBounds.removeFromTop(loopParamBounds.getHeight() * .85f));
    m_scaleLbl.setBounds(loopParamBounds);
    
    noteParamBounds = noteParamBounds.reduced(10);
    auto gateBnds{ noteParamBounds.removeFromLeft(bounds.getWidth() * .5f) };
    m_gateKnob.setBounds(gateBnds.removeFromTop(gateBnds.getHeight() * .85f));
    m_gateLbl.setBounds(gateBnds);
    auto densityBnds{ noteParamBounds.removeFromRight(bounds.getWidth() * .5f) };
    m_densityKnob.setBounds(densityBnds.removeFromTop(densityBnds.getHeight() * .85f));
    m_densityLbl.setBounds(densityBnds);

    auto mutateBnds{ melodyParamBounds.reduced(10) };
    m_mutateKnob.setBounds(mutateBnds.removeFromTop(mutateBnds.getHeight() * .85f));
    m_mutateLbl.setBounds(mutateBnds);
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
}

//==============================================================================
std::vector<juce::Component*> GenerativeMelodicSequencerAudioProcessorEditor::GetComponents()
{
    return
    {
        &m_noteVisualiser,
        //&m_lockToggle,
        &m_randomiseBtn,
        &m_scaleKnob,
        &m_bpmKnob,
        &m_loopLengthKnob,
        &m_gateKnob,
        &m_densityKnob,
        &m_mutateKnob
    };
}
std::vector<ComponentLabel*> GenerativeMelodicSequencerAudioProcessorEditor::GetLabels()
{
    return
    {
        &m_randomiseLbl,
        &m_scaleLbl,
        &m_bpmLbl,
        &m_loopLengthLbl,
        &m_gateLbl,
        &m_densityLbl,
        &m_mutateLbl
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