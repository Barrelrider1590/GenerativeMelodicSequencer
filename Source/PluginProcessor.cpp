/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GenerativeMelodicSequencerAudioProcessor::GenerativeMelodicSequencerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    m_startTime(juce::Time::getMillisecondCounterHiRes()), m_elapsedTime(0), m_frequency(440.f)
#endif
{
    m_osc.setFrequency(m_frequency);
}

GenerativeMelodicSequencerAudioProcessor::~GenerativeMelodicSequencerAudioProcessor()
{

}

//==============================================================================
const juce::String GenerativeMelodicSequencerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GenerativeMelodicSequencerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GenerativeMelodicSequencerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GenerativeMelodicSequencerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GenerativeMelodicSequencerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GenerativeMelodicSequencerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GenerativeMelodicSequencerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GenerativeMelodicSequencerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GenerativeMelodicSequencerAudioProcessor::getProgramName (int index)
{
    return {};
}

void GenerativeMelodicSequencerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //juce::dsp::ProcessSpec spec{};
    //spec.maximumBlockSize = samplesPerBlock;
    //spec.sampleRate = sampleRate;
    //spec.numChannels = getTotalNumOutputChannels();

    //m_osc.prepare(spec);

    //m_osc.setFrequency(m_frequency);

    //m_gain.prepare(spec);
    //m_gain.setGainLinear(0.1f);
}

void GenerativeMelodicSequencerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GenerativeMelodicSequencerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GenerativeMelodicSequencerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals{};

    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i{ totalNumInputChannels }; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    //juce::dsp::AudioBlock<float> audioBlock {buffer};

    //m_elapsedTime += juce::Time::getMillisecondCounterHiRes() - m_startTime;
    //if (m_elapsedTime > 1000000)
    //{
    //    setRandomFrequency();
    //    m_osc.setFrequency(m_frequency);
    //    m_elapsedTime = 0;
    //}

    //m_osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    //m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

}

//==============================================================================
bool GenerativeMelodicSequencerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GenerativeMelodicSequencerAudioProcessor::createEditor()
{
    return new GenerativeMelodicSequencerAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GenerativeMelodicSequencerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GenerativeMelodicSequencerAudioProcessor();
}

#pragma region AudioSynthesis


#pragma endregion


#pragma region UI
ChainSettings getChainSettings(const juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

    settings.m_bpm = apvts.getRawParameterValue("bpm")->load();
    settings.m_length = apvts.getRawParameterValue("length")->load();
    settings.m_level = apvts.getRawParameterValue("level")->load();
    settings.m_gate = apvts.getRawParameterValue("gate")->load();
    settings.m_density = apvts.getRawParameterValue("density")->load();
    settings.m_mutate = apvts.getRawParameterValue("mutate")->load();

    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout GenerativeMelodicSequencerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout{};

    layout.add(std::make_unique<juce::AudioParameterInt>("bpm", "BPM", 30, 300, 120));
    layout.add(std::make_unique<juce::AudioParameterInt>("length", "Length", 4, 16, 8));
    layout.add(std::make_unique<juce::AudioParameterFloat>("level", "Level", 0.f, 1.f, .0f));
    layout.add(std::make_unique < juce::AudioParameterFloat>("gate", "Gate", 0.f, 1.f, .5f));
    layout.add(std::make_unique < juce::AudioParameterFloat>("density", "Density", 0.f, 1.f, .5f));
    layout.add(std::make_unique < juce::AudioParameterFloat>("mutate", "Mutate", 0.f, 1.f, .5f));

    return layout;
}

//void GenerativeMelodicSequencerAudioProcessor::setRandomFrequency()
//{
//    auto rand = juce::Random::Random();
//    m_frequency = (rand.nextFloat() * 25163 + 20453 % 46616) / 100 ;
//}

#pragma endregion