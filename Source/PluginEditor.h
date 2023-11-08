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

#define TEXT_HEIGHT 20
#define INDENT 10
#define SLIDERSIZE 100
#define WIDTH SLIDERSIZE*7 +INDENT*13
#define HEIGHT TEXT_HEIGHT*5 + INDENT + SLIDERSIZE*2
#define PANEL_SPACING (TEXT_HEIGHT/5);
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
    
    struct panel : public juce::Component, sjf_lookAndFeel
    {
        panel(){}
        ~panel(){}
        
        void paint(juce::Graphics &g) override
        {
            g.setColour( sjf_lookAndFeel::panelColour.withAlpha(0.1f) );
            g.fillRoundedRectangle( 0, 0, getWidth(), getHeight(), CORNER_SIZE );
        }
        
        void resized() override
        {
            repaint();
        }
        
        static constexpr int CORNER_SIZE = 5;
    };
    
    void timerCallback() override;
    void delayTimeDisplay();
    void jitterDisplayDisplay();
    void rateDisplay();
    void fxDisplay();
    void crushDisplay( bool isDisplayed );
    void ringModDisplay( bool isDisplayed );
    void filterDisplay( bool isDisplayed );
    void outmodeDisplay();
    void drawPanels( int panelNum );
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Sjf_mincerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    
    juce::Slider delayTimeSlider, feedbackSlider, rateSlider, reverseSlider, repeatSlider, crossTalkSlider, densitySlider, transpositionSlider, outputLevelSlider, filterFreqSlider, ringModFreqSlider;
    
    std::array< juce::Slider, MAX_N_HARMONIES > harmonySliders;
    std::array< juce::ToggleButton, MAX_N_HARMONIES > harmonyButtons;
    std::array< std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, MAX_N_HARMONIES > harmonySlidersAttachments;
    std::array< std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>, MAX_N_HARMONIES > harmonyButtonsAttachments;
    
    sjf_numBox delayTimeSyncDivisionNumberSlider, delaySyncOffsetSlider, rateSyncNumDivisionsSlider, delaySyncJitterNDivisionsSlider, bitDepthSlider, srDividerSlider, mixSlider, filterQSlider, filterJitterSlider, ringModMixSlider, ringModSpreadSlider, ringModJitterSlider;
    
    sjf_numBox delayTimeJitterSlider, transpositionJitterSlider;
    
    juce::ToggleButton delayTimeSyncButton, delayTimeJitterSyncButton, feedbackControlButton, rateSyncButton, bitCrushButton, filterButton, ringModButton;
    
    juce::ComboBox outputModeBox, interpolationTypeBox, rateSyncDivisionBox, delayTimeSyncDivisionBox, delaySyncJitterDivisionBox, harmonyDislayBox, filterTypeBox;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::SliderAttachment > delayTimeSliderAttachment, delayTimeJitterSliderAttachment, feedbackSliderAttachment, reverseSliderAttachment, repeatSliderAttachment, delayTimeSyncDivisionNumberAttachment, delaySyncOffsetSliderAttachment, delaySyncJitterNDivisionsSliderAttachment, rateSliderAttachment, rateSyncNumDivisionsSliderAttachment, transpositionSliderAttachment, transpositionJitterSliderAttachment, crossTalkSliderAttachment, densitySliderAttachment, bitDepthSliderAttachment, srDividerSliderAttachment, mixSliderAttachment, outputLevelSliderAttachment, filterFreqSliderAttachment, filterQSliderAttachment, filterJitterSliderAttachment, ringModFreqSliderAttachment, ringModMixSliderAttachment, ringModSpreadSliderAttachment, ringModJitterSliderAttachment;
    
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ComboBoxAttachment > outputModeBoxAttachment, interpolationTypeBoxAttachment, rateSyncDivisionBoxAttachment, delayTimeSyncDivisionBoxAttachment, delaySyncJitterDivisionBoxAttachment, filterTypeBoxAttachment;
    
    std::unique_ptr< juce::AudioProcessorValueTreeState::ButtonAttachment > delayTimeSyncButtonAttachment, delayTimeJitterSyncButtonAttachment, feedbackControlButtonAttachment, rateSyncButtonAttachment, bitCrushButtonAttachment, filterButtonAttachment, ringModButtonAttachment;

    std::array< std::string, 3 > fxNames { "bitCrusher", "ringModulator", "filter"  };
    std::array< std::string, 3 > fxDisplayNames { "bc", "rm", "f"  };
    std::array< juce::ToggleButton, 3 > fxNameButtons;
    
    juce::Label offsetLabel, bitsLabel, srDivLabel, filterJitLabel, ringModMixLabel, ringModSpreadLabel, ringModJitterLabel;
    
    std::array< juce::Label, 3 > nDivsLabels, divTypeLabels;
    
    sjf_lookAndFeel otherLookAndFeel;
    
    
    
    juce::ToggleButton tooltipsToggle;
    juce::Label tooltipLabel;
    juce::String MAIN_TOOLTIP = "sjf_mincer: \nGranular Delay\n\nThe incomming signal is stored in a delayline and then individual grains are triggered from this delayline \nVariations and effects are then applied to individual grain";
    
    
    std::array< panel, 7 > panels;
    
    enum divLabels { delT, delTJit, rate };
    
    enum effects { crush, ringMod, filter };
    
    enum panelNames { delayTime, feedback, gRate, transposition, gParams, fx, output };
    
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
