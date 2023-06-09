/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Utility.h"
//==============================================================================
/**
*/
class GenerativeMelodicSequencerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    GenerativeMelodicSequencerAudioProcessor();
    ~GenerativeMelodicSequencerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();

    //==============================================================================
    void AddListenerToBroadcaster(juce::ChangeListener* listener);
    void RemoveListenerFromBroadcaster(juce::ChangeListener* listener);

    //==============================================================================
    int GetActiveNote();
    juce::AudioProcessorValueTreeState* GetAPVTS();
    SequencerSettings GetSequencerSettings(const juce::AudioProcessorValueTreeState&);

    void SetResetMelody();
    

private:
    void UpdateMidiBuffer(juce::MidiBuffer& midiBuffer, int sampleRate, const SequencerSettings& sequencerSettings);
    void AddNoteOnMessageToBuffer(juce::MidiBuffer& midiBuffer, const std::vector<int>& scaleVect, const SequencerSettings& sequencerSettings);
    void AddNoteOffMessageToBuffer(juce::MidiBuffer& midiBuffer, int activeNote, const SequencerSettings& sequencerSettings);
    
    //==============================================================================
    void UpdateMelody(const SequencerSettings& sequencerSettings);
    void MutateNote(int noteIndex, const std::vector<int>& scale, const SequencerSettings& sequencerSettings);
    void GenerateMelody(const std::vector<int>& scale);
    //==============================================================================
    static const int m_maxLoopLength;
    
    int m_samplesProcessed;
    int m_noteCounter;
    int m_activeNote;

    bool m_isNoteOn;
    juce::Atomic<bool> m_resetMelody;

    const int m_rootNote;
    const std::vector<int> m_majorScaleVect;
    const std::vector<int> m_minorScaleVect;
    const std::vector<int> m_pentatonicScaleVect;
    const std::vector<std::vector<int>> m_scalesVect;
    int m_prevScaleIndex;
    std::vector<int> m_melodyVect;

    juce::ChangeBroadcaster m_broadcaster;

    juce::Synthesiser m_synth;

    juce::AudioProcessorValueTreeState m_apvts;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessor)
};