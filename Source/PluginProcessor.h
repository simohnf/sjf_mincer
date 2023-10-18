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
    
    
    std::atomic<float>* delayTimeParameter = nullptr;
    std::atomic<float>* delayTimeJitterParameter = nullptr;
    std::atomic<float>* feedbackParameter = nullptr;
    std::atomic<float>* transpositionParameter = nullptr;
    std::atomic<float>* transpositionJitterParameter = nullptr;
    std::atomic<float>* reverseParameter = nullptr;
    std::atomic<float>* densityParameter = nullptr;
    std::atomic<float>* rateParameter = nullptr;
    std::atomic<float>* crossTalkParameter = nullptr;
    std::atomic<float>* mixParameter = nullptr;
    
    
    void setParameters();
    
    sjf_granularDelay< 8 > m_granDel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sjf_mincerAudioProcessor)
};
