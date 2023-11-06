/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../sjf_audio/sjf_LookAndFeel.h"
#include "../sjf_audio/sjf_widgets.h"
//==============================================================================
/**
*/
class Sjf_mincerAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    Sjf_mincerAudioProcessorEditor (Sjf_mincerAudioProcessor&, juce::AudioProcessorValueTreeState& vts );
    ~Sjf_mincerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    
    
    void delayTimeDisplay()
    {
        if( delayTimeSyncButton.getToggleState() )
        {
            delaySyncOffsetSlider.setVisible( true );
            delayTimeSyncDivisionNumberSlider.setVisible( true );
            delayTimeSyncDivisionBox.setVisible( true );
            delayTimeSlider.setVisible( false );
            
        }
        else
        {
            delaySyncOffsetSlider.setVisible( false );
            delayTimeSyncDivisionNumberSlider.setVisible( false );
            delayTimeSyncDivisionBox.setVisible( false );
            delayTimeSlider.setVisible( true );
        }
    }
    
    void jitterDisplayDisplay()
    {
        if ( delayTimeJitterSyncButton.getToggleState() )
        {
            delaySyncJitterNDivisionsSlider.setVisible( true );
            delaySyncJitterDivisionBox.setVisible( true );
        }
        else
        {
            delaySyncJitterNDivisionsSlider.setVisible( false );
            delaySyncJitterDivisionBox.setVisible( false );
        }
    }
    
    void rateDisplay()
    {
        if ( rateSyncButton.getToggleState() )
        {
            rateSlider.setVisible( false );
            rateSyncDivisionBox.setVisible( true );
            rateSyncNumDivisionsSlider.setVisible( true );
        }
        else
        {
            rateSlider.setVisible( true );
            rateSyncDivisionBox.setVisible( false );
            rateSyncNumDivisionsSlider.setVisible( false );
        }
    }
    
    
    
    void crushDisplay()
    {
        if ( bitCrushButton.getToggleState() )
        {
            bitDepthSlider.setVisible( true );
            srDividerSlider.setVisible( true );
        }
        else
        {
            bitDepthSlider.setVisible( false );
            srDividerSlider.setVisible( false );
        }
    }
    
    void outmodeDisplay()
    {
        if ( outputModeBox.getSelectedId() == 1 )
        {
            mixSlider.setVisible( true );
        }
        else
        {
            mixSlider.setVisible( false );
        }
    }
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Sjf_mincerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    
    juce::Slider delayTimeSlider, delayTimeJitterSlider, feedbackSlider,
        rateSlider, reverseSlider, repeatSlider, crossTalkSlider, densitySlider,
        transpositionSlider, transpositionJitterSlider, outputLevelSlider, filterFreqSlider;
    
//    static constexpr int maxNHarmonies = audioProcessor.MAX_N_HARMONIES;
    std::array< juce::Slider, MAX_N_HARMONIES > harmonySliders;
    std::array< juce::ToggleButton, MAX_N_HARMONIES > harmonyButtons;
    std::array< std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, MAX_N_HARMONIES > harmonySlidersAttachments;
    std::array< std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>, MAX_N_HARMONIES > harmonyButtonsAttachments;
    
    sjf_numBox delayTimeSyncDivisionNumberSlider, delaySyncOffsetSlider, rateSyncNumDivisionsSlider, delaySyncJitterNDivisionsSlider, bitDepthSlider, srDividerSlider, mixSlider, filterQSlider;
    
    juce::ToggleButton delayTimeSyncButton, delayTimeJitterSyncButton, feedbackControlButton, rateSyncButton, bitCrushButton, filterButton;
    
    
    juce::ComboBox outputModeBox, interpolationTypeBox, rateSyncDivisionBox, delayTimeSyncDivisionBox, delaySyncJitterDivisionBox, harmonyDislayBox, filterTypeBox;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::SliderAttachment > delayTimeSliderAttachment, delayTimeJitterSliderAttachment, feedbackSliderAttachment, reverseSliderAttachment, repeatSliderAttachment, delayTimeSyncDivisionNumberAttachment, delaySyncOffsetSliderAttachment, delaySyncJitterNDivisionsSliderAttachment, rateSliderAttachment, rateSyncNumDivisionsSliderAttachment, transpositionSliderAttachment, transpositionJitterSliderAttachment, crossTalkSliderAttachment, densitySliderAttachment, bitDepthSliderAttachment, srDividerSliderAttachment, mixSliderAttachment, outputLevelSliderAttachment, filterFreqSliderAttachment, filterQSliderAttachment;
    
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ComboBoxAttachment > outputModeBoxAttachment, interpolationTypeBoxAttachment, rateSyncDivisionBoxAttachment, delayTimeSyncDivisionBoxAttachment, delaySyncJitterDivisionBoxAttachment, filterTypeBoxAttachment;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ButtonAttachment > delayTimeSyncButtonAttachment, delayTimeJitterSyncButtonAttachment, feedbackControlButtonAttachment, rateSyncButtonAttachment, bitCrushButtonAttachment, filterButtonAttachment;

    
    juce::Label offsetLabel;
    
    sjf_lookAndFeel otherLookAndFeel;
    
    
    
    juce::ToggleButton tooltipsToggle;
    juce::Label tooltipLabel;
    juce::String MAIN_TOOLTIP = "sjf_mincer: \nGranular Delay\n";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sjf_mincerAudioProcessorEditor)
};

//

//

//
//
//
//
//
//
//std::array< std::atomic<float>*, MAX_N_HARMONIES > harmoniesOnParameters;
//std::array< std::atomic<float>*, MAX_N_HARMONIES > harmoniesParameters;
//
//
//
//
