/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SequencerSettings GetSequencerSettings(const juce::AudioProcessorValueTreeState& apvts)
{
    SequencerSettings settings;

    settings.bpm = apvts.getRawParameterValue("bpm")->load();
    settings.loopLength = apvts.getRawParameterValue("length")->load();
    settings.gate = apvts.getRawParameterValue("gate")->load();
    settings.density = apvts.getRawParameterValue("density")->load();
    settings.mutate = apvts.getRawParameterValue("mutate")->load();

    return settings;
}

//==============================================================================
const int GenerativeMelodicSequencerAudioProcessor::m_maxLoopLength{ 16 };

GenerativeMelodicSequencerAudioProcessor::GenerativeMelodicSequencerAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    m_samplesProcessed(0), 
    m_noteCounter(0),
    m_isNoteOn(false),
    m_resetMelody(false),
    m_majorScaleVect({ 60, 62, 64, 65, 67, 69, 71 }),
    m_melodyVect(),
    m_broadcaster(),
    m_synth(),
    m_apvts(*this, nullptr, "Parameters", CreateParameterLayout())
#endif
{
    m_melodyVect.resize(m_maxLoopLength);

    m_synth.addSound( new SynthSound());
    m_synth.addVoice( new SynthVoice());
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
    //============================================================
    m_synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i{ 0 }; i < m_synth.getNumVoices(); ++i)
    {
        if (SynthVoice* voice = dynamic_cast<SynthVoice*>(m_synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    SequencerSettings sequencerSettings{ GetSequencerSettings(m_apvts) };

    GenerateMelody(m_majorScaleVect);
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

    SequencerSettings sequencerSettings{ GetSequencerSettings(m_apvts) };

    UpdateMidiBuffer(midiMessages, getSampleRate(), sequencerSettings);

    m_synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    m_samplesProcessed += buffer.getNumSamples();
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

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout GenerativeMelodicSequencerAudioProcessor::CreateParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout{};

    layout.add(std::make_unique<juce::AudioParameterInt>("bpm", "BPM", 60, 600, 120));
    layout.add(std::make_unique<juce::AudioParameterInt>("length", "Length", 4, m_maxLoopLength, 4));
    layout.add(std::make_unique<juce::AudioParameterFloat>("gate", "Gate", .1f, 1.f, .5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("density", "Density", .1f, 1.f, .75f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mutate", "Mutate", 0.f, 1.f, 1.f));

    return layout;
}

//==============================================================================
void GenerativeMelodicSequencerAudioProcessor::AddListenerToBroadcaster(juce::ChangeListener* listener)
{
    m_broadcaster.addChangeListener(listener);
}
void GenerativeMelodicSequencerAudioProcessor::RemoveListenerFromBroadcaster(juce::ChangeListener* listener)
{
    m_broadcaster.removeChangeListener(listener);
}

//==============================================================================
int GenerativeMelodicSequencerAudioProcessor::GetCurrentMidiNote()
{
    return m_majorScaleVect[m_melodyVect[m_noteCounter]];
}
bool GenerativeMelodicSequencerAudioProcessor::GetIsNoteOn()
{
    return m_isNoteOn;
}
const std::vector<int>& GenerativeMelodicSequencerAudioProcessor::GetScale()
{
    return m_majorScaleVect;
}
juce::AudioProcessorValueTreeState* GenerativeMelodicSequencerAudioProcessor::GetAPVTS()
{
    return &m_apvts;
}

void GenerativeMelodicSequencerAudioProcessor::ResetMelody()
{
    m_resetMelody = true;
}

//==============================================================================
#pragma region Midi Handling
void GenerativeMelodicSequencerAudioProcessor::UpdateMidiBuffer(juce::MidiBuffer& midiBuffer, 
                                                                int numSamples, 
                                                                const SequencerSettings& sequencerSettings)
{
    int noteOnInterval = numSamples * 60/sequencerSettings.bpm;
    int noteOffInterval = noteOnInterval + (noteOnInterval * sequencerSettings.gate);

    if (m_samplesProcessed >= noteOffInterval)
    {
        if (m_isNoteOn)
        {
            AddNoteOffMessageToBuffer(midiBuffer, m_majorScaleVect, sequencerSettings);
            m_isNoteOn = false;
        }
    }
    if (m_samplesProcessed >= noteOnInterval && m_samplesProcessed < noteOffInterval)
    {
        if (!m_isNoteOn)
        {
            AddNoteOnMessageToBuffer(midiBuffer, m_majorScaleVect, sequencerSettings);
            m_isNoteOn = true;
            m_broadcaster.sendChangeMessage();
        }
    }
    if (m_samplesProcessed >= noteOnInterval * 2)
    {
        m_samplesProcessed = 0;
    }
}
void GenerativeMelodicSequencerAudioProcessor::AddNoteOnMessageToBuffer(juce::MidiBuffer& midiBuffer,
                                                                        const std::vector<int>& scaleVect,
                                                                        const SequencerSettings& sequencerSettings)
{
    juce::MidiMessage message{ juce::MidiMessage::noteOn(1, scaleVect[m_melodyVect[m_noteCounter]], sequencerSettings.density) };
    midiBuffer.addEvent(message, 0);
}
void GenerativeMelodicSequencerAudioProcessor::AddNoteOffMessageToBuffer(juce::MidiBuffer& midiBuffer, 
                                                                         const std::vector<int>& scaleVect,
                                                                         const SequencerSettings& sequencerSettings)
{
    juce::MidiMessage message{ juce::MidiMessage::noteOff(1, scaleVect[m_melodyVect[m_noteCounter]], sequencerSettings.density) };
    midiBuffer.addEvent(message, 0);

    UpdateMelody(scaleVect, sequencerSettings);
}
#pragma endregion
//==============================================================================
#pragma region Melody
void GenerativeMelodicSequencerAudioProcessor::UpdateMelody( const std::vector<int>& scaleVect,
                                                             const SequencerSettings& sequencerSettings)
{
    if (m_resetMelody.compareAndSetBool(false, true))
    {
        GenerateMelody(scaleVect);
    }
    else
    {
        MutateNote(m_melodyVect[m_noteCounter], scaleVect, sequencerSettings);

    }

    ++m_noteCounter;
    if (m_noteCounter >= sequencerSettings.loopLength)
    {
        m_noteCounter = 0;
    }
    // m_noteCounter exceeds loopLength =>  will result in invalid subscript in vector!
    jassert(m_noteCounter <= sequencerSettings.loopLength);
}
void GenerativeMelodicSequencerAudioProcessor::GenerateMelody(const std::vector<int>& scaleVect)
{
    juce::Random rand;
    for (int i{ 0 }; i < m_maxLoopLength; ++i)
    {
        m_melodyVect[i] = rand.nextInt(scaleVect.size());
    }
}
void GenerativeMelodicSequencerAudioProcessor::MutateNote(int noteIndex,
                                                          const std::vector<int>& scaleVect,
                                                          const SequencerSettings& sequencerSettings)
{
    juce::Random random;
    if (random.nextFloat() < sequencerSettings.mutate)
    {
        m_melodyVect[noteIndex] = random.nextInt(scaleVect.size());
    }
}
#pragma endregion