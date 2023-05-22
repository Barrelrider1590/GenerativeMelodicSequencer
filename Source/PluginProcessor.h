/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

struct ChainSettings
{
    int m_bpm{ 0 }, m_length{ 0 };
    float m_level{ 0 }, m_gate{ 0 }, m_density{ 0 }, m_mutate{ 0 };
};

ChainSettings getChainSettings(const juce::AudioProcessorValueTreeState& apvts);

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

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState m_apvts{*this, nullptr, "Parameters", createParameterLayout()};

private:
    void updateMidiBuffer(juce::MidiBuffer& midiBuffer, int numSamples);
    void GenerateMelody(std::vector<int>& melody, const std::vector<int>& scale);
    int GenerateRandomNote(const std::vector<int>& scale);

    //============================================================================
    int m_samplesProcessed;
    int m_currentNote;
    int m_loopLength;

    std::vector<int> m_majorScale{ 60, 62, 64, 65, 67, 69, 71 };
    std::vector<int> m_melody;

    juce::Synthesiser m_synth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessor)
};