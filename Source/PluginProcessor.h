/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SineOscillator.h"

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
    void setRandomFrequency();

    //==============================================================================
    double m_startTime, m_elapsedTime;
    float m_frequency;

    juce::dsp::Oscillator<float> m_osc{ [](float x) { return std::sin(x); } };
    // wave shape formula
    // sine: return std::sin(x);
    // saw: return x / juce::MathConstants<float>::pi;
    // square: return x < 0.0f ? -1.0f : 1.0f;

    juce::dsp::Gain<float> m_gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessor)
};