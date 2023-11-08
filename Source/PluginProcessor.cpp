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
    DBG("started initialisation");
    delayTimeParameter = parameters.getRawParameterValue( "delayTime" );
    delayTimeJitterParameter = parameters.getRawParameterValue( "delayTimeJitter" );
    
    delayTimeSyncParameter = parameters.getRawParameterValue( "delayTimeSync" );
    
    delayTimeSyncDivisionParameter = parameters.getRawParameterValue( "division" );
    delayTimeSyncDivisionNumberParameter = parameters.getRawParameterValue( "numDivisions" );
    delaySyncOffsetParameter = parameters.getRawParameterValue( "syncOffset" );
    
    delayTimeJitterSyncParameter = parameters.getRawParameterValue( "syncJitter" );
    delaySyncJitterDivisionParameter = parameters.getRawParameterValue( "syncedJitterDivision" );
    delaySyncJitterNDivisionsParameter = parameters.getRawParameterValue( "syncedJitterMaxNumDivisions" );
    
    feedbackParameter = parameters.getRawParameterValue( "feedback" );
    feedbackControlParameter = parameters.getRawParameterValue( "feedbackControl" );
    
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        harmoniesOnParameters[ i ] = parameters.getRawParameterValue( "harmonyOn" + juce::String( i ) );
        harmoniesParameters[ i ] = parameters.getRawParameterValue( "harmony" + juce::String( i ) );
    }
    transpositionParameter = parameters.getRawParameterValue( "transposition" );
    transpositionJitterParameter = parameters.getRawParameterValue( "transpositionJitter" );
    
    
    reverseParameter = parameters.getRawParameterValue( "reverse" );
    
    
    repeatParameter = parameters.getRawParameterValue( "repeat" );
    
    bitCrushParameter = parameters.getRawParameterValue( "crush" );
    bitDepthParameter = parameters.getRawParameterValue( "bitDepth" );
    srDividerParameter = parameters.getRawParameterValue( "sampleRateDivider" );
    
    filterFreqParameter = parameters.getRawParameterValue( "filterFrequency" );
    filterQParameter = parameters.getRawParameterValue( "filterQ" );
    filterTypeParameter = parameters.getRawParameterValue( "filterType" );
    filterActiveParameter = parameters.getRawParameterValue( "filter" );
    
    filterJitterParameter = parameters.getRawParameterValue( "filterJitter" );
    
    
    ringModActiveParameter = parameters.getRawParameterValue( "ringMod");
    ringModFreqParameter = parameters.getRawParameterValue( "ringModFreq");
    ringModSpreadParameter = parameters.getRawParameterValue( "ringModSpread");
    ringModMixParameter = parameters.getRawParameterValue( "ringModMix");
    ringModJitterParameter = parameters.getRawParameterValue( "ringModJitter");
    
    
    
    rateParameter = parameters.getRawParameterValue( "rate" );
    
    rateSyncParameter = parameters.getRawParameterValue( "rateSync" );
    rateSyncDivisionParameter = parameters.getRawParameterValue( "rateSyncDivision" );
    rateSyncNumDivisionsParameter = parameters.getRawParameterValue( "rateSyncNumDivisions" );
    
    crossTalkParameter = parameters.getRawParameterValue( "crossTalk" );
    
    outputModeParameter = parameters.getRawParameterValue( "outMode" );
    densityParameter = parameters.getRawParameterValue( "density" );
    mixParameter  = parameters.getRawParameterValue( "mix" );
    
    
    interpolationTypeParameter = parameters.getRawParameterValue( "interpolation" );
    
    outLevelParameter = parameters.getRawParameterValue( "outputLevel" );
    
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
    return new Sjf_mincerAudioProcessorEditor ( *this, parameters );
//    return new Sjf_mincerAudioProcessorEditor ( *this );
//    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void Sjf_mincerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Sjf_mincerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sjf_mincerAudioProcessor();
}



void Sjf_mincerAudioProcessor::setParameters()
{
    auto dt = *delayTimeParameter * 0.001f * getSampleRate();
    m_granDel.setJitterSync( false );
    auto gRate = *rateParameter * 1.0f;


    playHead = this->getPlayHead();
    // if there is an available playhead
    if (playHead != nullptr )
    {
        positionInfo = *playHead->getPosition();
        if ( positionInfo.getBpm() && positionInfo.getTimeInSamples() )
        {
            auto bpm = *positionInfo.getBpm();
            auto wholeNoteSamps = 0.001f * getSampleRate() * 4.0f * 60000.0f / bpm;
            if ( *rateSyncParameter )
            {
                auto wholeNoteS = 60.0 / bpm;
                auto multiplier = m_divNames.getMultiplier( *rateSyncDivisionParameter );
                auto divS = multiplier * *rateSyncNumDivisionsParameter * wholeNoteS;
                gRate = 1.0 / divS;
            }
            if ( *delayTimeSyncParameter )
            {
                auto multiplier = m_divNames.getMultiplier( *delayTimeSyncDivisionParameter );
                dt = wholeNoteSamps * *delayTimeSyncDivisionNumberParameter * multiplier;
                dt += dt * (*delaySyncOffsetParameter * 0.01f);
            }
            if ( *delayTimeJitterSyncParameter )
            {
                m_granDel.setJitterSync( true );
                auto multiplier = m_divNames.getMultiplier( *delaySyncJitterDivisionParameter );
                m_granDel.setSyncedDelayJitterValues( multiplier * wholeNoteSamps, *delaySyncJitterNDivisionsParameter );
            }
        }
    }
    
    m_granDel.setDelayTimeSamps( dt );
    m_granDel.setDelayTimeJitter( *delayTimeJitterParameter );
    
    m_granDel.setRate( gRate );
    
    m_granDel.setFeedback( *feedbackParameter );
    m_granDel.setFeedbackControl( *feedbackControlParameter );
    
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        m_granDel.setHarmony( i, *harmoniesOnParameters[ i ], *harmoniesParameters[ i ] );
    }
    m_granDel.setTransposition( *transpositionParameter );
    m_granDel.setTranspositionJitter( *transpositionJitterParameter );
    
    m_granDel.setReverseChance( *reverseParameter );
    
    m_granDel.setDensity( *densityParameter );
    
    m_granDel.setRepeat( *repeatParameter );
    
    
    m_granDel.setBitCrushOn( *bitCrushParameter );
    m_granDel.setBitDepth( *bitDepthParameter );
    m_granDel.setSampleRateDivider( *srDividerParameter );
    
    
    
    m_granDel.setCrossTalk( *crossTalkParameter );
    
    m_granDel.setMix( *mixParameter );
    
    m_granDel.setOutputMode( *outputModeParameter );
    
    int inter = *interpolationTypeParameter;
    m_granDel.setInterpolationType( inter + 1);
    
    auto type = 0;
    switch ( static_cast< int >(*filterTypeParameter) ) {
        case 0 :
            type = sjf_biquadCalculator< float >::filterType::lowpass;
            break;
        case 1:
            type = sjf_biquadCalculator< float >::filterType::highpass;
            break;
        case 2:
            type = sjf_biquadCalculator< float >::filterType::bandpass;
            break;
        default:
            type = sjf_biquadCalculator< float >::filterType::lowpass;
            break;
    }
    m_granDel.setFilter( *filterFreqParameter, *filterQParameter, type, *filterActiveParameter );
    m_granDel.setFilterJitter( *filterJitterParameter );
    
    
    m_granDel.setRingMod( *ringModFreqParameter, *ringModSpreadParameter, *ringModMixParameter, *ringModActiveParameter );
    m_granDel.setRingModJitter( *ringModJitterParameter );
    
    m_granDel.setOutputLevel( *outLevelParameter );
}



juce::AudioProcessorValueTreeState::ParameterLayout Sjf_mincerAudioProcessor::createParameterLayout()
{
    DBG("creating parameter layout");
    static constexpr int pIDVersionNumber = 1;
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    
    sjf_divNames< NDIVLEVELS > temp;
    
    auto dn = temp.getAllNames();
    juce::StringArray divNames;
    auto eightNoteIndex = 0;
    for ( auto i = 0; i < dn.size(); i++ )
    {
        if ( dn[i] == "/8n")
            eightNoteIndex= i;
        divNames.add( juce::String( dn[ i ] ) );
    }

    auto range = juce::NormalisableRange< float > ( 0, 10000 );
    auto attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("ms");
    // delay time parameters
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "delayTimeSync", pIDVersionNumber }, "DelayTimeSync", false ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "delayTime", pIDVersionNumber }, "DelayTime", range, 500, attributes ) );

    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "numDivisions", pIDVersionNumber }, "NumDivision", 1, 128, 1 ) );
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "division", pIDVersionNumber }, "Division", divNames, eightNoteIndex ) );

    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "syncOffset", pIDVersionNumber }, "SyncOffset", -33.3333, 33.3333, 0 ) );
    
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "feedback", pIDVersionNumber }, "Feedback", range, 0, attributes ) );
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "feedbackControl", pIDVersionNumber }, "feedbackControl", false ) );
    
    
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "delayTimeJitter", pIDVersionNumber }, "DelayTimeJitter", range, 0, attributes ) );
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "syncJitter" , pIDVersionNumber }, "SyncJitter" , false ) );
    
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "syncedJitterMaxNumDivisions", pIDVersionNumber }, "SyncedJitterMaxNumDivisions",  1, 128, 16 ) );
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "syncedJitterDivision", pIDVersionNumber }, "SyncedJitterDivision", divNames, eightNoteIndex ) );
    
    
    
    
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "harmonyOn" + juce::String( i ), pIDVersionNumber }, "HarmonyOn"  + juce::String( i ), false ) );
        params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "harmony"  + juce::String( i ), pIDVersionNumber }, "Harmony"  + juce::String( i ), -12, 12, 0 ) );
    }
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "transposition", pIDVersionNumber }, "Transposition", -12, 12, 0 ) );
    
    
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "transpositionJitter", pIDVersionNumber }, "TranspositionJitter", range, 0, attributes ) );
    
    
    
    
    
    
    // Grain Rate
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "rateSync", pIDVersionNumber }, "RateSync", false ) );
    
    range = juce::NormalisableRange< float > ( 1, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("Hz");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "rate", pIDVersionNumber }, "Rate", range, 10, attributes ) );
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "rateSyncNumDivisions", pIDVersionNumber }, "RateSyncNumDivisions", 1, 128, 1 ) );
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "rateSyncDivision", pIDVersionNumber }, "RateSyncDivision", divNames, eightNoteIndex ) );
    

    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "reverse", pIDVersionNumber }, "Reverse", range, 0, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "repeat", pIDVersionNumber }, "Repeat", range, 0, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "crossTalk", pIDVersionNumber }, "CrossTalk", range, 0, attributes ) );
    
    
    
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "crush", pIDVersionNumber }, "Crush", false ) );
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "bitDepth", pIDVersionNumber }, "BitDepth", 1, 16, 16 ) );
    params.add( std::make_unique<juce::AudioParameterInt>( juce::ParameterID{ "sampleRateDivider", pIDVersionNumber }, "SampleRateDivider", 1, 16, 1 ) );
    
    
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "ringMod", pIDVersionNumber }, "RingMod", false ) );
    range = juce::NormalisableRange< float > ( 0.01, 10000 );
    range.setSkewForCentre( 1000 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("Hz");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "ringModFreq", pIDVersionNumber }, "RingModFreq", range, 100, attributes ) );
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "ringModSpread", pIDVersionNumber }, "RingModSpread", range, 0, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "ringModMix", pIDVersionNumber }, "RingModMix", range, 50, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "ringModJitter", pIDVersionNumber }, "RingModJitter", range, 0, attributes ) );
    
    
    
    params.add( std::make_unique<juce::AudioParameterBool>( juce::ParameterID{ "filter", pIDVersionNumber }, "Filter", false ) );
    range = juce::NormalisableRange< float > ( 20, 10000 );
    range.setSkewForCentre( 1000 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("Hz");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "filterFrequency", pIDVersionNumber }, "FilterFrequency", range, 1000, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "filterQ", pIDVersionNumber }, "FilterQ", 0.01, 10, 1 ) );
    juce::StringArray filtTypes;
    for ( auto i = 0; i < filterTypes.size(); i++ )
        filtTypes.add( filterTypes[ i ] );
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "filterType", pIDVersionNumber }, "FilterType", filtTypes, 0 ) );
    
//    filterJitter
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "filterJitter", pIDVersionNumber }, "FilterJitter", range, 0, attributes ) );
    
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "outMode", pIDVersionNumber }, "OutMode", juce::StringArray { "Mix", "Insert", "Gate" }, 0 ) );
    
    range = juce::NormalisableRange< float > ( 0, 100, 0.1 );
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("%");
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "density", pIDVersionNumber }, "Density", range, 100, attributes ) );
    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "mix", pIDVersionNumber }, "Mix", range, 100, attributes ) );
    
    juce::StringArray intNames;
    int defaultInterp = 0;
    for ( auto i = 0; i < interpNames.size(); i++ )
    {
        if ( interpNames[ i ] == "hermite" )
            defaultInterp = i;
        intNames.add( interpNames[ i ] );
    }
    params.add( std::make_unique<juce::AudioParameterChoice> (juce::ParameterID{ "interpolation", pIDVersionNumber }, "Interpolation", intNames, defaultInterp ) );
    
//    range = juce::NormalisableRange< float > ( -120, 6 );
    range = juce::NormalisableRange< float >( -100.0, 0, 0.1 );
    range.setSkewForCentre( -6);
    attributes = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (x); }).withLabel ("dB");

    params.add( std::make_unique<juce::AudioParameterFloat>( juce::ParameterID{ "outputLevel", pIDVersionNumber }, "OutputLevel", range, 0.0, attributes ) );
    
    return params;
}
