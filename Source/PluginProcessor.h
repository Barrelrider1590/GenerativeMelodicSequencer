/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

struct SequencerSettings
{
    int bpm{ 0 }, loopLength{ 0 };
    float gate{ 0 }, density{ 0 }, mutate{ 0 };
};
SequencerSettings GetSequencerSettings(const juce::AudioProcessorValueTreeState& apvts);

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
    int GetCurrentNote();
    bool GetIsNoteOn();
    juce::AudioProcessorValueTreeState* GetAPVTS();

private:
    void UpdateMidiBuffer(juce::MidiBuffer& midiBuffer, int numSamples, const SequencerSettings& sequencerSettings);
    void AddNoteOnMessageToBuffer(juce::MidiBuffer& midiBuffer, const SequencerSettings& sequencerSettings);
    void AddNoteOffMessageToBuffer(juce::MidiBuffer& midiBuffer, const SequencerSettings& sequencerSettings);
    
    //==============================================================================
    void GenerateMelody(std::vector<int>& melody, const std::vector<int>& scale, int loopLength);
    void MutateMelody(std::vector<int>& melody, const std::vector<int>& scale, const SequencerSettings& sequencerSettings);
    int GenerateRandomNote(const std::vector<int>& scale);

    //==============================================================================
    int m_samplesProcessed;
    int m_noteCounter;
    int m_loopLength;
    static const int m_maxLoopLength;

    bool m_isNoteOn;

    static const std::vector<int> m_majorScale;
    std::vector<int> m_melody;

    std::unique_ptr<juce::ChangeBroadcaster> m_broadcaster;

    juce::Synthesiser* m_synth; // creating unique pointer caused memory leak issue

    juce::AudioProcessorValueTreeState m_apvts{ *this, nullptr, "Parameters", CreateParameterLayout() };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessor)
};