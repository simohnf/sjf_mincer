/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../sjf_audio/sjf_granularDelay.h"
#include "../sjf_audio/sjf_audioUtilitiesC++.h"
#include "../sjf_audio/sjf_biquadCalculator.h"

//==============================================================================
/**
*/
#define MAX_N_HARMONIES 4
class Sjf_mincerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
private:
    const std::array< std::string, 6 > interpNames = { "linear", "cubic", "pureData", "fourthOrder", "godot", "hermite" };
//    const std::array< std::string, 8 > filterTypes = { "lowpass" == 1, "highpass", "allpass", "lowshelf", "highshelf", "bandpass", "bandcut", "peak" };
    const std::array< std::string, 3 > filterTypes = { "lp", "hp", "bp" };
    
public:
//    static constexpr int MAX_N_HARMONIES = 4;
    
    //==============================================================================
    Sjf_mincerAudioProcessor();
    ~Sjf_mincerAudioProcessor() override;

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

    
    auto getDivNames( ){ return m_divNames.getAllNames(); }
    
    auto getInterpNames(){ return interpNames; }
    
    auto getFilterTypes(){ return filterTypes; }
    
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;
    
    void setParameters();
    
//    static constexpr int MAX_N_HARMONIES = 4;
    
    
    juce::AudioPlayHead* playHead;
    juce::AudioPlayHead::PositionInfo positionInfo;
    
    std::atomic<float>* delayTimeParameter = nullptr;
    std::atomic<float>* delayTimeJitterParameter = nullptr;
    
    std::atomic<float>* delayTimeSyncParameter = nullptr;
    
    std::atomic<float>* delayTimeSyncDivisionParameter = nullptr;
    std::atomic<float>* delayTimeSyncDivisionNumberParameter = nullptr;
    std::atomic<float>* delaySyncOffsetParameter = nullptr;
    
    
    std::atomic<float>* delayTimeJitterSyncParameter = nullptr;
    std::atomic<float>* delaySyncJitterDivisionParameter = nullptr;
    std::atomic<float>* delaySyncJitterNDivisionsParameter = nullptr;
    
    
    std::atomic<float>* feedbackParameter = nullptr;
    std::atomic<float>* feedbackControlParameter = nullptr;
    

    std::array< std::atomic<float>*, MAX_N_HARMONIES > harmoniesOnParameters;
    std::array< std::atomic<float>*, MAX_N_HARMONIES > harmoniesParameters;
    std::atomic<float>* transpositionParameter = nullptr;
    std::atomic<float>* transpositionJitterParameter = nullptr;
    
    
    
    
    std::atomic<float>* rateParameter = nullptr;
    
    std::atomic<float>* rateSyncParameter = nullptr;
    std::atomic<float>* rateSyncDivisionParameter = nullptr;
    std::atomic<float>* rateSyncNumDivisionsParameter = nullptr;
    
    std::atomic<float>* bitCrushParameter = nullptr;
    std::atomic<float>* bitDepthParameter = nullptr;
    std::atomic<float>* srDividerParameter = nullptr;
    
    
    std::atomic<float>* filterFreqParameter = nullptr;
    std::atomic<float>* filterQParameter = nullptr;
    std::atomic<float>* filterTypeParameter = nullptr;
    std::atomic<float>* filterActiveParameter = nullptr;
    
    std::atomic<float>* filterJitterParameter = nullptr;
    
    
    std::atomic<float>* ringModActiveParameter = nullptr;
    std::atomic<float>* ringModFreqParameter = nullptr;
    std::atomic<float>* ringModSpreadParameter = nullptr;
    std::atomic<float>* ringModMixParameter = nullptr;
    std::atomic<float>* ringModJitterParameter = nullptr;
    
    
    std::atomic<float>* crossTalkParameter = nullptr;
    std::atomic<float>* reverseParameter = nullptr;
    std::atomic<float>* repeatParameter = nullptr;
    std::atomic<float>* densityParameter = nullptr;
    
    
    std::atomic<float>* outputModeParameter = nullptr;
    std::atomic<float>* mixParameter = nullptr;
    
    std::atomic<float>* interpolationTypeParameter = nullptr;
    
    
    std::atomic<float>* outLevelParameter = nullptr;
    
    
    sjf_granularDelay< float, 16, MAX_N_HARMONIES > m_granDel;
    
    static constexpr int NDIVLEVELS = 8;
    sjf_divNames< NDIVLEVELS > m_divNames;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sjf_mincerAudioProcessor)
};
