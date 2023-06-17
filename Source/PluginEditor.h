/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "NoteVisualiser.h"

//==============================================================================
/**
*/
class GenerativeMelodicSequencerAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                                        public juce::ChangeListener, 
                                                        public juce::Timer
{
public:
    GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor&);
    ~GenerativeMelodicSequencerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;

private:

    std::vector<juce::Component*> GetComponents();
    std::vector<ComponentLabel*> GetLabels();

    //==============================================================================
    GenerativeMelodicSequencerAudioProcessor& m_audioProcessor;

    int const m_timerFreq;

    juce::Atomic<bool> m_midiUpdated;

    CustomLookAndFeel m_lookAndFeel;

    NoteVisualiser m_noteVisualiser;

    juce::ToggleButton m_lockToggle;
    juce::TextButton m_randomiseBtn;
    ComponentLabel m_randomiseLbl;

    RotaryKnob m_bpmKnob;
    ComponentLabel m_bpmLbl;
    RotaryKnob m_loopLengthKnob;
    ComponentLabel m_loopLengthLbl;
    RotaryKnob m_scaleKnob;
    ComponentLabel m_scaleLbl;
    RotaryKnob m_gateKnob;
    ComponentLabel m_gateLbl;
    RotaryKnob m_densityKnob;
    ComponentLabel m_densityLbl;
    RotaryKnob m_mutateKnob;
    ComponentLabel m_mutateLbl;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment m_scaleKnobAttachment;
    Attachment m_bpmKnobAttachment;
    Attachment m_loopLengthKnobAttachment;
    Attachment m_gateKnobAttachment;
    Attachment m_densityKnobAttachment;
    Attachment m_mutateKnobAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessorEditor)
};
