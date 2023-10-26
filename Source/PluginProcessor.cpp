/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Sjf_mincerAudioProcessor::Sjf_mincerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, parameters( *this, nullptr, juce::Identifier("sjf_mincer"), createParameterLayout() )
#endif
{
    delayTimeParameter = parameters.getRawParameterValue( "delayTime" );
    delayTimeJitterParameter = parameters.getRawParameterValue( "delayTimeJitter" );
    feedbackParameter = parameters.getRawParameterValue( "feedback" );
    transpositionParameter = parameters.getRawParameterValue( "transposition" );
    transpositionJitterParameter = parameters.getRawParameterValue( "transpositionJitter" );
    reverseParameter = parameters.getRawParameterValue( "reverse" );
    densityParameter = parameters.getRawParameterValue( "density" );
    
    repeatParameter = parameters.getRawParameterValue( "repeat" );
    
    bitDepthParameter = parameters.getRawParameterValue( "bitRate" );
    srDividerParameter = parameters.getRawParameterValue( "sampleRateDivider" );
    rateParameter = parameters.getRawParameterValue( "rate" );
    crossTalkParameter = parameters.getRawParameterValue( "crossTalk" );
    mixParameter  = parameters.getRawParameterValue( "mix" );
    
    DBG("Initialised 1");
}

Sjf_mincerAudioProcessor::~Sjf_mincerAudioProcessor()
{
}

//==============================================================================
const juce::String Sjf_mincerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Sjf_mincerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Sjf_mincerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Sjf_mincerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Sjf_mincerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Sjf_mincerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Sjf_mincerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Sjf_mincerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Sjf_mincerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Sjf_mincerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Sjf_mincerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_granDel.initialise( sampleRate );
//    DBG("Initialised 2");
}

void Sjf_mincerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Sjf_mincerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Sjf_mincerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear ( i, 0, buffer.getNumSamples() );

    setParameters();
    
    m_granDel.process( buffer );

}

//==============================================================================
bool Sjf_mincerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Sjf_mincerAudioProcessor::createEditor()
{
//    return new Sjf_mincerAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void Sjf_mincerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Sjf_mincerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sjf_mincerAudioProcessor();
}



void Sjf_mincerAudioProcessor::setParameters()
{
    
    
    
    
    m_granDel.setDelayTimeSamps( *delayTimeParameter * 0.001 * getSampleRate() );
    m_granDel.setDelayTimeJitter( *delayTimeJitterParameter );
    
    
    m_granDel.setFeedback( *feedbackParameter );
    
    m_granDel.setTransposition( *transpositionParameter );
    m_granDel.setTranspositionJitter( *transpositionJitterParameter );
    
    m_granDel.setReverseChance( *reverseParameter );
    
    m_granDel.setDensity( *densityParameter );
    
    m_granDel.setRepeat( *repeatParameter );
    
    m_granDel.setBitDepth( *bitDepthParameter );
    m_granDel.setSampleRateDivider( *srDividerParameter );
    
    m_granDel.setRate( *rateParameter  );
    
    m_granDel.setCrossTalk( *crossTalkParameter );
    
    m_granDel.setMix( *mixParameter );
    
    
}



juce::AudioProcessorValueTreeState::ParameterLayout Sjf_mincerAudioProcessor::createParameterLayout()
{
    static constexpr int pIDVersionNumber = 1;
    juce::AudioProcessorValueTreeState::ParameterLayout params;




//    rateParameter = parameters.getRawParameterValue( "rate" );
//    mixParameter  = parameters.getRawParameterValue( "mix" );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "delayTime", pIDVersionNumber }, "DelayTime", 0, 10000, 500 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "delayTimeJitter", pIDVersionNumber }, "DelayTimeJitter", 0, 100, 0 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "feedback", pIDVersionNumber }, "Feedback", 0, 100, 0 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "transposition", pIDVersionNumber }, "Transposition", -12, 12, 0 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "transpositionJitter", pIDVersionNumber }, "TranspositionJitter", 0, 100, 0 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "reverse", pIDVersionNumber }, "Reverse", 0, 100, 0 ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "density", pIDVersionNumber }, "Density", 0, 100, 100 ) );
    
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "rate", pIDVersionNumber }, "Rate", 1, 100, 1 ) );
    

    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "repeat", pIDVersionNumber }, "Repeat", 1, 100, 0 ) );
    
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "bitRate", pIDVersionNumber }, "BitRate", 1, 32, 32 ) );
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "sampleRateDivider", pIDVersionNumber }, "SampleRateDivider", 1, 16, 1 ) );
    
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "crossTalk", pIDVersionNumber }, "CrossTalk", 1, 100, 1 ) );
    
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "mix", pIDVersionNumber }, "Mix", 0, 100, 100 ) );
    return params;
}
