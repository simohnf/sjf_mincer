/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../sjf_audio/sjf_granularDelay.h"
//==============================================================================
/**
*/
class Sjf_mincerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
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

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;
    
    void setParameters();
    
    static constexpr int MAX_N_HARMONIES = 4;
    
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
    
    
    
    std::atomic<float>* reverseParameter = nullptr;
    
    
    std::atomic<float>* repeatParameter = nullptr;
    
    std::atomic<float>* rateParameter = nullptr;
    
    std::atomic<float>* rateSyncParameter = nullptr;
    std::atomic<float>* rateSyncDivisionParameter = nullptr;
    std::atomic<float>* rateSyncNumDivisionsParameter = nullptr;
    
    std::atomic<float>* bitCrushParameter = nullptr;
    std::atomic<float>* bitDepthParameter = nullptr;
    std::atomic<float>* srDividerParameter = nullptr;
    
    std::atomic<float>* crossTalkParameter = nullptr;
    
    
    std::atomic<float>* outputModeParameter = nullptr;
    std::atomic<float>* densityParameter = nullptr;
    std::atomic<float>* mixParameter = nullptr;
    
    std::atomic<float>* interpolationTypeParameter = nullptr;
    
    

    
    
    
    sjf_granularDelay< 16, MAX_N_HARMONIES > m_granDel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sjf_mincerAudioProcessor)
};
