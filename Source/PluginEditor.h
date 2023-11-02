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
template< int TABLE_SIZE >
struct divNames
{
public:
    constexpr divNames() : m_table()
    {
        for ( auto d = 0; d < NDIVS; d++ )
        {
            for ( auto dt = 0; dt < NDIVTYPES; dt++ )
            {
                m_table[ (d*dt) + d ] =  m_divs[ d ] + m_divTypes[ dt ];
//                m_table[ idx ] = "hello";
            }
        }
    }
    
    const std::string& operator[](std::size_t index) const { return m_table[ index ]; }

private:
    std::string m_table[ TABLE_SIZE ];
    static constexpr int NDIVS = 8;
    static constexpr int NDIVTYPES = 4;
    const std::string m_divs[ NDIVS ] =  { "/1", "/2", "/4", "/8", "/16", "/32", "/64", "/128" };
    const std::string m_divTypes[ NDIVTYPES ] =  { "", "t", "q", "s" };
    
};


class Sjf_mincerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Sjf_mincerAudioProcessorEditor (Sjf_mincerAudioProcessor&, juce::AudioProcessorValueTreeState& vts );
    ~Sjf_mincerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
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
        
    }
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Sjf_mincerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    
    juce::Slider delayTimeSlider, delayTimeJitterSlider, feedbackSlider,
        rateSlider, reverseSlider, repeatSlider, crossTalkSlider, densitySlider, mixSlider,
        transpositionSlider, transpositionJitterSlider,
        bitDepthSlider, srDividerSlider;
    
    
    sjf_numBox delayTimeSyncDivisionNumberSlider, delaySyncOffsetSlider, rateSyncNumDivisionsSlider, delaySyncJitterNDivisionsSlider;
    
    juce::ToggleButton delayTimeSyncButton, delayTimeJitterSyncButton, feedbackControlButton, rateSyncButton, bitCrushButton;
    
    
    juce::ComboBox outputModeBox, interpolationTypeBox, rateSyncDivisionBox, delayTimeSyncDivisionBox, delaySyncJitterDivisionBox;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::SliderAttachment > delayTimeSliderAttachment, delayTimeJitterSliderAttachment, feedbackSliderAttachment, reverseSliderAttachment, repeatSliderAttachment, delayTimeSyncDivisionNumberAttachment, delaySyncOffsetSliderAttachment, delaySyncJitterNDivisionsSliderAttachment, rateSliderAttachment, rateSyncNumDivisionsSliderAttachment, transpositionSliderAttachment, transpositionJitterSliderAttachment, crossTalkSliderAttachment, densitySliderAttachment, bitDepthSliderAttachment, srDividerSliderAttachment;
    
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ComboBoxAttachment > outputModeBoxAttachment, interpolationTypeBoxAttachment, rateSyncDivisionBoxAttachment, delayTimeSyncDivisionBoxAttachment, delaySyncJitterDivisionBoxAttachment;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ButtonAttachment > delayTimeSyncButtonAttachment, delayTimeJitterSyncButtonAttachment, feedbackControlButtonAttachment, rateSyncButtonAttachment, bitCrushButtonAttachment;

    
    sjf_lookAndFeel otherLookAndFeel;
    
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
