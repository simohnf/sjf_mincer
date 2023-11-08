/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Sjf_mincerAudioProcessorEditor::Sjf_mincerAudioProcessorEditor (Sjf_mincerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), valueTreeState( vts )
{
    auto divNames = audioProcessor.getDivNames();
    auto interpNames = audioProcessor.getInterpNames();
    
    for ( auto & l : nDivsLabels )
    {
        addAndMakeVisible( &l );
        l.setText( "nDivs", juce::dontSendNotification );
        l.setTooltip( MAIN_TOOLTIP );
    }
    for ( auto & l : divTypeLabels )
    {
        addAndMakeVisible( &l );
        l.setText( "type", juce::dontSendNotification );
        l.setTooltip( MAIN_TOOLTIP );
    }
    
    
    setLookAndFeel( &otherLookAndFeel );
    otherLookAndFeel.drawComboBoxTick = false;
    
    for ( auto & p : panels )
    {
        addAndMakeVisible( &p );
    }
    
    addAndMakeVisible( &delayTimeSyncButton );
    delayTimeSyncButton.setButtonText( "Sync" );
    delayTimeSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "delayTimeSync", delayTimeSyncButton ) );
    delayTimeSyncButton.onClick = [ this ]
    {
        delayTimeDisplay();
    };
    delayTimeSyncButton.setTooltip( "This allows you to sync the delay time to the underlying tempo" );
    delayTimeSyncButton.sendLookAndFeelChange();
    
    addAndMakeVisible( &delayTimeSlider );
    delayTimeSlider.setSliderStyle( juce::Slider::Rotary );
    delayTimeSlider.setTextValueSuffix("ms");
    delayTimeSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "delayTime", delayTimeSlider )  );
    delayTimeSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, delayTimeSlider.getWidth(), TEXT_HEIGHT );
    delayTimeSlider.setTooltip( "This sets the delay time in milliseconds" );
    delayTimeSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delayTimeSyncDivisionBox );
    auto menu = delayTimeSyncDivisionBox.getRootMenu();
    for ( auto i = 0; i < divNames.size(); i++ )
    {
        if ( i % 5 == 0 )
            menu->addColumnBreak ();
        delayTimeSyncDivisionBox.addItem( divNames[ i ], i+1 );
    }
    delayTimeSyncDivisionBox.setJustificationType( juce::Justification::centred );
    delayTimeSyncDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "division", delayTimeSyncDivisionBox )  );
    delayTimeSyncDivisionBox.setTooltip( "This sets the base rhythmic division when the delay time is synced to the session tempo \n/8n is an 8th note, /8nd is a dotted 8th note, /8nt is an 8th note triplet, /8nq is an 8th note quintuplet, /8ns is an 8th note septuplet, etc");
    delayTimeSyncDivisionBox.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &delayTimeSyncDivisionNumberSlider );
    delayTimeSyncDivisionNumberAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "numDivisions", delayTimeSyncDivisionNumberSlider )  );
    delayTimeSyncDivisionNumberSlider.setTooltip( "This sets the number of base divisions when the delay time is synced to the session tempo. \ne.g. if the base division is /8n and the number of divisions is set to 5 the delay time will be 5 8th notes" );
    delayTimeSyncDivisionNumberSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delaySyncOffsetSlider );
    delaySyncOffsetSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "syncOffset", delaySyncOffsetSlider )  );
    delaySyncOffsetSlider.setTooltip( "This allows you to set an offset from the delaytime determined by the synced delay time values" );
    delaySyncOffsetSlider.sendLookAndFeelChange();
    delaySyncOffsetSlider.setTextValueSuffix("%");
    
    addAndMakeVisible( &offsetLabel );
    offsetLabel.attachToComponent( &delaySyncOffsetSlider, false );
    offsetLabel.setText( "offset", juce::dontSendNotification );
    offsetLabel.setTooltip( MAIN_TOOLTIP );
    offsetLabel.setJustificationType( juce::Justification::centred );
    
    addAndMakeVisible( &feedbackSlider );
    feedbackSlider.setSliderStyle( juce::Slider::Rotary );
    feedbackSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "feedback", feedbackSlider )  );
    feedbackSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, feedbackSlider.getWidth(), TEXT_HEIGHT );
    feedbackSlider.setTooltip( "This sets how much of the granular delay output is fed back into the delay line" );
    feedbackSlider.sendLookAndFeelChange();
    feedbackSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &feedbackControlButton );
    feedbackControlButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "feedbackControl", feedbackControlButton ) );
    feedbackControlButton.setTooltip( "This activates a nonlinear limiter in the feedback loop to control feedback and prevent overloads" );
    feedbackControlButton.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &delayTimeJitterSyncButton );
    delayTimeJitterSyncButton.setButtonText( "Sync" );
    delayTimeJitterSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "syncJitter", delayTimeJitterSyncButton ) );
    delayTimeJitterSyncButton.onClick = [ this ]
    {
        jitterDisplayDisplay();
    };
    delayTimeJitterSyncButton.setTooltip( "This allows you to sync random delay time variations to the underlying tempo" );
    delayTimeJitterSyncButton.sendLookAndFeelChange();
    
    addAndMakeVisible( &delayTimeJitterSlider);
    delayTimeJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "delayTimeJitter", delayTimeJitterSlider )  );
    delayTimeJitterSlider.setTooltip( "This sets the amount of random variations in delay time" );
    delayTimeJitterSlider.sendLookAndFeelChange();
    delayTimeJitterSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &delaySyncJitterNDivisionsSlider );
    delaySyncJitterNDivisionsSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "syncedJitterMaxNumDivisions", delaySyncJitterNDivisionsSlider )  );
    delaySyncJitterNDivisionsSlider.setTooltip( "This sets the maximum number of base divisions when the random delay time variations are synced to the session tempo. \ne.g. if the base division is /8n and the number of divisions is set to 2, random variations of delay will be chosen from:\n [ (delayTime - 2*8th notes), (delayTime - 1*8th note), (delayTime), (delayTime + 1*8th note), (delayTime + 2*8th notes) ]" );
    delaySyncJitterNDivisionsSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delaySyncJitterDivisionBox );
    menu = delaySyncJitterDivisionBox.getRootMenu();
    for ( auto i = 0; i < divNames.size(); i++ )
    {
        if ( i % 5 == 0 )
            menu->addColumnBreak ();
        delaySyncJitterDivisionBox.addItem( divNames[ i ], i+1 );
    }
    delaySyncJitterDivisionBox.setJustificationType( juce::Justification::centred );
    delaySyncJitterDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "syncedJitterDivision", delaySyncJitterDivisionBox )  );
    delaySyncJitterDivisionBox.setTooltip( "This sets the base rhythmic division when the random delay time variations are synced to the session tempo \n/8n is an 8th note, /8nd is a dotted 8th note, /8nt is an 8th note triplet, /8nq is an 8th note quintuplet, /8ns is an 8th note septuplet, etc");
    delaySyncJitterDivisionBox.sendLookAndFeelChange();
    

    
    addAndMakeVisible( &rateSyncButton );
    rateSyncButton.setButtonText( "Sync" );
    rateSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "rateSync", rateSyncButton ) );
    rateSyncButton.onClick = [ this ]
    {
        rateDisplay();
    };
    rateSyncButton.setTooltip( "This allows you to sync the rate of grain production to the underlying tempo" );
    rateSyncButton.sendLookAndFeelChange();
    

    addAndMakeVisible( &rateSlider );
    rateSlider.setSliderStyle( juce::Slider::Rotary );
    rateSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "rate", rateSlider )  );
    rateSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, rateSlider.getWidth(), TEXT_HEIGHT );
    rateSlider.setTooltip( "This sets the rate of grain production in Hz" );
    rateSlider.sendLookAndFeelChange();
    rateSlider.setTextValueSuffix( " Hz" );
    
    
    
    
    addAndMakeVisible( &rateSyncDivisionBox );
    menu = rateSyncDivisionBox.getRootMenu();
    for ( auto i = 0; i < divNames.size(); i++ )
    {
        if ( i % 5 == 0 )
            menu->addColumnBreak ();
        rateSyncDivisionBox.addItem( divNames[ i ], i+1 );
    }
    rateSyncDivisionBox.setJustificationType( juce::Justification::centred );
    rateSyncDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "rateSyncDivision", rateSyncDivisionBox )  );
    rateSyncDivisionBox.setTooltip( "This sets the base rhythmic division when the grain rate is synced to the session tempo \n/8n is an 8th note, /8nd is a dotted 8th note, /8nt is an 8th note triplet, /8nq is an 8th note quintuplet, /8ns is an 8th note septuplet, etc");
    rateSyncDivisionBox.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &rateSyncNumDivisionsSlider );
    rateSyncNumDivisionsSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "rateSyncNumDivisions", rateSyncNumDivisionsSlider )  );
    rateSyncNumDivisionsSlider.setTooltip( "This sets the number of base divisions when grain rate is synced to the session tempo. \ne.g. if the base division is /8n and the number of divisions is set to 7 a new grain will be produced once every seven 8th notes" );
    rateSyncNumDivisionsSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &transpositionSlider );
    transpositionSlider.setSliderStyle( juce::Slider::Rotary );
    transpositionSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "transposition", transpositionSlider )  );
    transpositionSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, transpositionSlider.getWidth(), TEXT_HEIGHT );
    transpositionSlider.setTooltip( "This sets the main playback transposition of the grains. This acts like a simple pitch shifter, but the quality is highly dependent on the grain rate - i.e. if the grain rate is very low, then the result will sound like sections of the audio speeding up/slowing down. If the grain rate is very high then you will hear audible artefacts from the production of grains" );
    transpositionSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &transpositionJitterSlider );
    transpositionJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "transpositionJitter", transpositionJitterSlider )  );
    transpositionJitterSlider.setTooltip( "This sets the depth of random variations in the grain transposition" );
    transpositionJitterSlider.sendLookAndFeelChange();
    transpositionJitterSlider.setTextValueSuffix( "%" );

    addAndMakeVisible( &harmonyDislayBox );
    harmonyDislayBox.setTooltip( "This allows you to select between the four available additional grain voices to create harmonic textures" );
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        addAndMakeVisible( &harmonySliders[ i ] );
        harmonySliders[ i ].setSliderStyle( juce::Slider::Rotary );
        harmonySlidersAttachments[ i ].reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "harmony" + juce::String( i ), harmonySliders[ i ] )  );
        harmonySliders[ i ].setTextBoxStyle( juce::Slider::TextBoxBelow, false, harmonySliders[ i ].getWidth(), TEXT_HEIGHT );
        harmonySliders[ i ].setTooltip( "This sets the transposition of the additional voice number " + juce::String( i + 1 ) + " relative to the transposition of the main grain voice" );
        harmonySliders[ i ].sendLookAndFeelChange();
        
        
        addAndMakeVisible( &harmonyButtons[ i ] );
        harmonyButtonsAttachments[ i ].reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "harmonyOn" + juce::String( i ), harmonyButtons[ i ] ) );
        harmonyButtons[ i ].sendLookAndFeelChange();
        harmonyButtons[ i ].setTooltip( "This allows you to turn on/off additional grain voice " + juce::String( i + 1 ) );
        harmonyDislayBox.addItem( "harmony" + juce::String( i + 1 ), i+1 );
    }
    
    harmonyDislayBox.onChange = [ this ]
    {
        auto num = harmonyDislayBox.getSelectedId() - 1;
        for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
        {
            harmonySliders[ i ].setVisible( i == num  );
            harmonyButtons[ i ].setVisible( i == num  );
        }
    };
    
    harmonyDislayBox.setSelectedId( 1 );
    
    
    
    
    
    
    addAndMakeVisible( &reverseSlider );
    reverseSlider.setSliderStyle( juce::Slider::Rotary );
    reverseSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "reverse", reverseSlider )  );
    reverseSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, reverseSlider.getWidth(), TEXT_HEIGHT );
    reverseSlider.setTooltip( "This sets the overall probability of individual grains being played in reverse" );
    reverseSlider.sendLookAndFeelChange();
    reverseSlider.setTextValueSuffix( "%" );
    
    addAndMakeVisible( &repeatSlider );
    repeatSlider.setSliderStyle( juce::Slider::Rotary );
    repeatSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "repeat", repeatSlider )  );
    repeatSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, repeatSlider.getWidth(), TEXT_HEIGHT );
    repeatSlider.setTooltip( "This sets the overall probability of the next grain being a repetition of the previously triggered grain" );
    repeatSlider.sendLookAndFeelChange();
    repeatSlider.setTextValueSuffix( "%" );
    

    addAndMakeVisible( &crossTalkSlider );
    crossTalkSlider.setSliderStyle( juce::Slider::Rotary );
    crossTalkSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "crossTalk", crossTalkSlider )  );
    crossTalkSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, crossTalkSlider.getWidth(), TEXT_HEIGHT );
    crossTalkSlider.setTooltip( "This sets the extent to which signals in the left channel of the delay line are fed into the right channel, and vice versa" );
    crossTalkSlider.sendLookAndFeelChange();
    crossTalkSlider.setTextValueSuffix( "%" );
    
    addAndMakeVisible( &densitySlider );
    densitySlider.setSliderStyle( juce::Slider::Rotary );
    densitySliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "density", densitySlider )  );
    densitySlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, densitySlider.getWidth(), TEXT_HEIGHT );
    densitySlider.setTooltip( "This sets the overall probability of individual grains being triggered... lower density means more gaps in the granular texture" );
    densitySlider.sendLookAndFeelChange();
    densitySlider.setTextValueSuffix( "%" );


    for ( auto i = 0; i < fxNameButtons.size(); i ++ )
    {
        addAndMakeVisible( &fxNameButtons[ i ] );
        fxNameButtons[ i ].setButtonText( fxDisplayNames[ i ] );
        fxNameButtons[ i ].setTooltip( "This displays the parameters for the " + fxNames[ i ]  + " effect" );
        fxNameButtons[ i ].onClick = [ this, i ]
        {
            fxNameButtons[ i ].setToggleState( true , juce::dontSendNotification );
            for ( auto j = 0; j < fxNameButtons.size(); j++ )
            {
                if ( j != i )
                    fxNameButtons[ j ].setToggleState( false , juce::dontSendNotification );
            }
            fxDisplay();
        };
    }
    fxNameButtons[ effects::crush ].setToggleState( true , juce::dontSendNotification );
    
    
    
    addAndMakeVisible( &bitCrushButton );
    bitCrushButton.setButtonText( "On" );
    bitCrushButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "crush", bitCrushButton ) );
    bitCrushButton.setTooltip( "This turns on/off the bit crusher and sample rate reducer" );
    bitCrushButton.sendLookAndFeelChange();

    addAndMakeVisible( &bitsLabel );
    bitsLabel.setText( "nBits ", juce::dontSendNotification );
    bitsLabel.setTooltip( MAIN_TOOLTIP );
    addAndMakeVisible( &bitDepthSlider );
//    bitDepthSlider.setTextValueSuffix( " bits" );
    bitDepthSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "bitDepth", bitDepthSlider )  );
    bitDepthSlider.setTooltip( "This sets the number of bits... fewer bits equals more distorted signals" );
    bitDepthSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &srDivLabel );
    srDivLabel.setText( "SR / ", juce::dontSendNotification );
    srDivLabel.setTooltip( MAIN_TOOLTIP );
    addAndMakeVisible( &srDividerSlider );
    srDividerSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "sampleRateDivider", srDividerSlider )  );
    srDividerSlider.setTooltip( "This sets the factor by which the sample rate is reduced. e.g. if set to 2 the sample rate is divided by to, at 3 the sample rate is divided by 3. \nLower sample rate equals more aliasing" );
    srDividerSlider.sendLookAndFeelChange();
    
    
    
    addAndMakeVisible( &ringModButton );
    ringModButton.setButtonText( "On" );
    ringModButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "ringMod", ringModButton ) );
    ringModButton.setTooltip( "This turns on/off the ring modulator" );
    ringModButton.sendLookAndFeelChange();

    addAndMakeVisible( &ringModFreqSlider );
    ringModFreqSlider.setTextValueSuffix( "Hz" );
    ringModFreqSlider.setSliderStyle( juce::Slider::Rotary );
    ringModFreqSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "ringModFreq", ringModFreqSlider )  );
    ringModFreqSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, ringModFreqSlider.getWidth(), TEXT_HEIGHT );
    ringModFreqSlider.setTooltip( "This sets the modulator frequency" );
    ringModFreqSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &ringModMixLabel );
    ringModMixLabel.setText( "mix", juce::dontSendNotification );
    ringModMixLabel.setTooltip( MAIN_TOOLTIP );
    addAndMakeVisible( &ringModMixSlider );
    ringModMixSlider.setTextValueSuffix( "%" );
    ringModMixSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "ringModMix", ringModMixSlider )  );
    ringModMixSlider.setTooltip( "This sets the percentage of wet signal mixed with the original. At 0% no ring modulation is applied, at 100% only the ring modulator is output" );
    ringModMixSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &ringModSpreadLabel );
    ringModSpreadLabel.setText( "spread", juce::dontSendNotification );
    ringModSpreadLabel.setTooltip( MAIN_TOOLTIP );
    addAndMakeVisible( &ringModSpreadSlider );
    ringModSpreadSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "ringModSpread", ringModSpreadSlider )  );
    ringModSpreadSlider.setTextValueSuffix( "%" );
    ringModSpreadSlider.setTooltip( "This sets the difference between the frequency for the left and right channel modulators " );
    ringModSpreadSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &ringModJitterLabel );
    ringModJitterLabel.setText( "jitter", juce::dontSendNotification );
    ringModSpreadLabel.setTooltip( MAIN_TOOLTIP );
    addAndMakeVisible( &ringModJitterSlider );
    ringModJitterSlider.setTextValueSuffix( "%" );
    ringModJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "ringModJitter", ringModJitterSlider )  );
    ringModJitterSlider.setTooltip( "This sets the amount of random variation applied to the frequency of the modulator" );
    ringModJitterSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &filterButton );
    filterButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "filter", filterButton ) );
    filterButton.setButtonText( "On" );
    filterButton.setTooltip( "This turns on/off the filter. This is only applied to the output of the delayline" );
    filterButton.sendLookAndFeelChange();
    
    addAndMakeVisible( &filterFreqSlider );
    filterFreqSlider.setTextValueSuffix( "Hz" );
    filterFreqSlider.setSliderStyle( juce::Slider::Rotary );
    filterFreqSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "filterFrequency", filterFreqSlider )  );
    filterFreqSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, bitDepthSlider.getWidth(), TEXT_HEIGHT );
    filterFreqSlider.setTooltip( "This sets the frequency for the filter" );
    filterFreqSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &filterQSlider );
    filterQSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "filterQ", filterQSlider )  );
    filterQSlider.setTextValueSuffix( "Q" );
    filterQSlider.setTooltip( "This sets the filter Q, higher Q means steeper roll offs and more resonance");
    filterQSlider.sendLookAndFeelChange();
    
    auto filterTypes = audioProcessor.getFilterTypes();
    addAndMakeVisible( &filterTypeBox );
    for ( auto i = 0; i < filterTypes.size(); i++ )
        filterTypeBox.addItem( filterTypes[ i ], i + 1 );
    filterTypeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "filterType", filterTypeBox )  );
    filterTypeBox.setTooltip( "This allows you to choose betwreen low pass, high pass, and band pass filters" );
    filterTypeBox.sendLookAndFeelChange();
    
    addAndMakeVisible( &filterJitLabel );
    filterJitLabel.setTooltip( MAIN_TOOLTIP );
    filterJitLabel.setText( "jitter", juce::dontSendNotification );
    addAndMakeVisible( &filterJitterSlider );
    filterJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "filterJitter", filterJitterSlider )  );
    filterJitterSlider.setTooltip( "This sets the amount of random variations applied to the filter frequency" );
    filterJitterSlider.setTextValueSuffix( "%" );
    filterJitterSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &outputModeBox );
    outputModeBox.addItem( "mix", 1 );
    outputModeBox.addItem( "insert", 2 );
    outputModeBox.addItem( "gate", 3 );
    outputModeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "outMode", outputModeBox )  );
    outputModeBox.setTooltip("This allows you to choose between 3 possible output modes. \nMix - mixes the delay line output with the dry signal\nInsert - this is dependent on the density parameter and will insert the wet signal whenever a grain is triggered and otherwise the dry signal will be output \nGate - again, this is dependent on the density parameter but will only output the  results of the grain engine.. this is essentially the same as the mix output mode with the mix parameter set to 100%" );
    outputModeBox.sendLookAndFeelChange();
    outputModeBox.onChange = [this]
    {
        outmodeDisplay();
    };

    addAndMakeVisible( &mixSlider );
    mixSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "mix", mixSlider )  );
    mixSlider.setTooltip( "This sets the percentage of wet signal mixed into the output when the output mode is set to mix" );
    mixSlider.sendLookAndFeelChange();
    mixSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &interpolationTypeBox );
    for ( auto i = 0; i < interpNames.size(); i++ )
        interpolationTypeBox.addItem( interpNames[ i ], i+1 );
    interpolationTypeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "interpolation", interpolationTypeBox )  );
    interpolationTypeBox.setTooltip( "This changes between different interpolation types... linear is the least cpu intensive, cubic sounds a bit naff to me, the others are all higher quality interpolation which come at various prices with regards to cpu usage" );
    interpolationTypeBox.sendLookAndFeelChange();
    
    addAndMakeVisible( &outputLevelSlider );
    outputLevelSlider.setSliderStyle( juce::Slider::Rotary );
    outputLevelSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, outputLevelSlider.getWidth(), TEXT_HEIGHT );
    outputLevelSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "outputLevel", outputLevelSlider )  );
    outputLevelSlider.setNumDecimalPlacesToDisplay( 1 );
    outputLevelSlider.sendLookAndFeelChange();
    outputLevelSlider.setTooltip( "This sets the output level in dB" );
    outputLevelSlider.setTextValueSuffix( "dB" );
    
    
    addAndMakeVisible( &tooltipLabel );
    tooltipLabel.setColour( juce::Label::backgroundColourId, otherLookAndFeel.backGroundColour.withAlpha( 0.85f ) );
    tooltipLabel.setTooltip( MAIN_TOOLTIP );
    
    addAndMakeVisible( &tooltipsToggle );
    tooltipsToggle.setButtonText( "HINTS" );
    tooltipsToggle.onClick = [this]
    {
        if (tooltipsToggle.getToggleState())
        {
            tooltipLabel.setVisible( true );
            setSize (WIDTH, HEIGHT+tooltipLabel.getHeight());
        }
        else
        {
            tooltipLabel.setVisible( false );
            setSize (WIDTH, HEIGHT);
        }
    };
    tooltipsToggle.setTooltip( "This turns on/off the hints");
    
    
    
    delayTimeDisplay();
    jitterDisplayDisplay();
    rateDisplay();
    fxDisplay();
    outmodeDisplay();
    
    setSize ( WIDTH, HEIGHT );
    
    startTimer( 100 );

}

Sjf_mincerAudioProcessorEditor::~Sjf_mincerAudioProcessorEditor()
{
    setLookAndFeel( nullptr );
    
    
}

//==============================================================================
void Sjf_mincerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
#ifdef JUCE_DEBUG
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
#else
    juce::Rectangle<int> r = { (int)( WIDTH ), (int)(HEIGHT + tooltipLabel.getHeight()) };
    sjf_makeBackground< 40 >( g, r );
#endif
    
    
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("sjf_mincer", 0, 0, getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    g.drawFittedText ("delayTime", delayTimeSyncButton.getX(), delayTimeSyncButton.getY() - TEXT_HEIGHT, delayTimeSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("dtJitter", delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getY() - TEXT_HEIGHT, delayTimeJitterSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);

    g.drawFittedText ("grain rate", rateSyncButton.getX(), rateSyncButton.getY() - TEXT_HEIGHT, rateSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("transposition", transpositionSlider.getX(), transpositionSlider.getY() - TEXT_HEIGHT, transpositionSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("trJitter", transpositionSlider.getX(), transpositionSlider.getBottom(), transpositionJitterSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("Grain Parameters", crossTalkSlider.getX(), crossTalkSlider.getY() - TEXT_HEIGHT*2, densitySlider.getRight() - crossTalkSlider.getX(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("crosstalk", crossTalkSlider.getX(), crossTalkSlider.getY() - TEXT_HEIGHT, crossTalkSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("density", densitySlider.getX(), densitySlider.getY() - TEXT_HEIGHT, densitySlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("reverse", reverseSlider.getX(), reverseSlider.getY() - TEXT_HEIGHT, reverseSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("repeat", repeatSlider.getX(), repeatSlider.getY() - TEXT_HEIGHT, repeatSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    g.drawFittedText ("FX", fxNameButtons[ 0 ].getX(), fxNameButtons[ 0 ].getY() - TEXT_HEIGHT, repeatSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("feedback", feedbackSlider.getX(), feedbackSlider.getY() - TEXT_HEIGHT, feedbackSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    g.drawFittedText ("output", outputLevelSlider.getX(), outputLevelSlider.getY() - TEXT_HEIGHT, outputLevelSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
}

void Sjf_mincerAudioProcessorEditor::resized()
{
    auto row1Y = TEXT_HEIGHT*2;
    
    delayTimeSyncButton.setBounds( INDENT, row1Y, SLIDERSIZE, TEXT_HEIGHT );
    delayTimeSlider.setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    nDivsLabels[ divLabels::delT ].setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    divTypeLabels[ divLabels::delT ].setBounds( nDivsLabels[ divLabels::delT ].getX(), nDivsLabels[ divLabels::delT ].getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    delayTimeSyncDivisionNumberSlider.setBounds( nDivsLabels[ divLabels::delT ].getRight(), nDivsLabels[ divLabels::delT ].getY(), SLIDERSIZE/2, TEXT_HEIGHT);
    delayTimeSyncDivisionBox.setBounds( delayTimeSyncDivisionNumberSlider.getX(), delayTimeSyncDivisionNumberSlider.getBottom(), SLIDERSIZE/2, TEXT_HEIGHT  );
    delaySyncOffsetSlider.setBounds( delayTimeSlider.getX(), delayTimeSyncDivisionBox.getBottom()+TEXT_HEIGHT+INDENT/2, SLIDERSIZE, TEXT_HEIGHT   );
    
    
    delayTimeJitterSyncButton.setBounds( delayTimeSyncButton.getX(), delayTimeSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );
    delayTimeJitterSlider.setBounds( delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    nDivsLabels[ divLabels::delTJit ].setBounds( delayTimeJitterSyncButton.getX(), delayTimeJitterSlider.getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    divTypeLabels[ divLabels::delTJit ].setBounds( nDivsLabels[ divLabels::delTJit ].getX(), nDivsLabels[ divLabels::delTJit ].getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    delaySyncJitterNDivisionsSlider.setBounds( nDivsLabels[ divLabels::delTJit ].getRight(), nDivsLabels[ divLabels::delTJit ].getY(), SLIDERSIZE / 2, TEXT_HEIGHT );
    delaySyncJitterDivisionBox.setBounds( divTypeLabels[ divLabels::delTJit ].getRight(), divTypeLabels[ divLabels::delTJit ].getY(), SLIDERSIZE / 2, TEXT_HEIGHT );

    drawPanels( panelNames::delayTime );
    
    
    feedbackSlider.setBounds( delayTimeSlider.getRight()+INDENT*2, row1Y, SLIDERSIZE, SLIDERSIZE );
    feedbackControlButton.setBounds( feedbackSlider.getRight() - TEXT_HEIGHT, feedbackSlider.getY(), TEXT_HEIGHT, TEXT_HEIGHT );
    
    drawPanels( panelNames::feedback );
    
    rateSyncButton.setBounds( feedbackSlider.getX(), delayTimeJitterSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    rateSlider.setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    nDivsLabels[ divLabels::rate ].setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    divTypeLabels[ divLabels::rate ].setBounds( nDivsLabels[ divLabels::rate ].getX(), nDivsLabels[ divLabels::rate ].getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    rateSyncNumDivisionsSlider.setBounds( nDivsLabels[ divLabels::rate ].getRight(), nDivsLabels[ divLabels::rate ].getY(), SLIDERSIZE/2, TEXT_HEIGHT );
    rateSyncDivisionBox.setBounds( divTypeLabels[ divLabels::rate ].getRight(), divTypeLabels[ divLabels::rate ].getY(), SLIDERSIZE/2, TEXT_HEIGHT );
    
    drawPanels( panelNames::gRate );

    transpositionSlider.setBounds( rateSyncButton.getRight()+INDENT*2, row1Y, SLIDERSIZE, SLIDERSIZE );
    transpositionJitterSlider.setBounds( transpositionSlider.getX() + SLIDERSIZE/2, transpositionSlider.getBottom(), SLIDERSIZE/2, TEXT_HEIGHT );
    
    harmonyDislayBox.setBounds( transpositionSlider.getX(), rateSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        harmonySliders[ i ].setBounds( harmonyDislayBox.getX(), harmonyDislayBox.getBottom(), SLIDERSIZE, SLIDERSIZE );
        harmonyButtons[ i ].setBounds( harmonySliders[ i ].getRight() - TEXT_HEIGHT, harmonySliders[ i ].getY(), TEXT_HEIGHT, TEXT_HEIGHT );
    }

    drawPanels( panelNames::transposition );
    
    crossTalkSlider.setBounds( transpositionSlider.getRight()+INDENT*2, row1Y+TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
    densitySlider.setBounds( crossTalkSlider.getRight()+INDENT, row1Y+TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );


    reverseSlider.setBounds( crossTalkSlider.getX(), crossTalkSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
    repeatSlider.setBounds( densitySlider.getX(), reverseSlider.getY(), SLIDERSIZE, SLIDERSIZE );

    drawPanels( panelNames::gParams );

    // good be simpler with this but this just ensures things are aligned
    auto fxSliderSize = sjf_findClosestMultiple< int >( SLIDERSIZE, fxNameButtons.size() );
    auto fxSliderHW = fxSliderSize/2;
    auto fxLabelW = fxSliderSize - fxSliderHW;
    for ( auto i = 0; i < fxNameButtons.size(); i++ )
        fxNameButtons[ i ].setBounds( densitySlider.getRight() + INDENT*2 + i*fxSliderSize/fxNameButtons.size(), row1Y, SLIDERSIZE/ fxNameButtons.size(), TEXT_HEIGHT );
    bitCrushButton.setBounds( fxNameButtons[ 0 ].getX(), fxNameButtons[ 0 ].getBottom(), fxSliderSize, TEXT_HEIGHT );
    bitsLabel.setBounds( bitCrushButton.getX(), bitCrushButton.getBottom(), fxLabelW, TEXT_HEIGHT );
    srDivLabel.setBounds( bitsLabel.getX(), bitsLabel.getBottom(), fxLabelW, TEXT_HEIGHT );
    bitDepthSlider.setBounds( bitsLabel.getRight(), bitsLabel.getY(), fxSliderHW, TEXT_HEIGHT );
    srDividerSlider.setBounds( srDivLabel.getRight(), srDivLabel.getY(), fxSliderHW, TEXT_HEIGHT );

    
    ringModButton.setBounds( fxNameButtons[ 0 ].getX(), fxNameButtons[ 0 ].getBottom(), fxSliderSize, TEXT_HEIGHT );
    ringModFreqSlider.setBounds( ringModButton.getX(), ringModButton.getBottom(), fxSliderSize, SLIDERSIZE );
    ringModMixLabel.setBounds( ringModFreqSlider.getX(), ringModFreqSlider.getBottom(), fxLabelW,  TEXT_HEIGHT );
    ringModMixSlider.setBounds( ringModMixLabel.getRight(), ringModMixLabel.getY(), fxSliderHW, TEXT_HEIGHT );
    ringModSpreadLabel.setBounds( ringModMixLabel.getX(), ringModMixLabel.getBottom(), fxLabelW,  TEXT_HEIGHT );
    ringModSpreadSlider.setBounds( ringModSpreadLabel.getRight(), ringModSpreadLabel.getY(), fxSliderHW, TEXT_HEIGHT );
    ringModJitterLabel.setBounds( ringModSpreadLabel.getX(), ringModSpreadLabel.getBottom(), fxLabelW,  TEXT_HEIGHT );
    ringModJitterSlider.setBounds( ringModJitterLabel.getRight(), ringModJitterLabel.getY(), fxSliderHW, TEXT_HEIGHT );
    
    filterButton.setBounds( bitCrushButton.getX(), bitCrushButton.getY(), fxLabelW, TEXT_HEIGHT );
    filterTypeBox.setBounds( filterButton.getRight(), filterButton.getY(), fxSliderHW, TEXT_HEIGHT );
    filterFreqSlider.setBounds( filterButton.getX(), filterTypeBox.getBottom(), fxSliderSize, SLIDERSIZE );
    filterQSlider.setBounds( filterFreqSlider.getX(), filterFreqSlider.getBottom(), fxSliderSize, TEXT_HEIGHT );
    filterJitLabel.setBounds( filterQSlider.getX(), filterQSlider.getBottom(), fxLabelW, TEXT_HEIGHT );
    filterJitterSlider.setBounds( filterJitLabel.getRight(), filterQSlider.getBottom(), fxSliderHW, TEXT_HEIGHT );
    
    drawPanels( panelNames::fx );
    
    outputLevelSlider.setBounds( bitCrushButton.getRight()+INDENT*2, row1Y, SLIDERSIZE, SLIDERSIZE );
        
    outputModeBox.setBounds( outputLevelSlider.getX(), outputLevelSlider.getBottom()+INDENT, SLIDERSIZE, TEXT_HEIGHT );
    mixSlider.setBounds( outputModeBox.getX(), outputModeBox.getBottom(), SLIDERSIZE, TEXT_HEIGHT );

    interpolationTypeBox.setBounds( mixSlider.getX(), mixSlider.getBottom()+TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );

    drawPanels( panelNames::output);
    
    tooltipsToggle.setBounds( interpolationTypeBox.getX(), HEIGHT - INDENT - TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );
    tooltipLabel.setBounds( 0, HEIGHT, getWidth(), TEXT_HEIGHT*5 );
}



void Sjf_mincerAudioProcessorEditor::timerCallback()
{
    if ( tooltipsToggle.getToggleState() )
        sjf_setTooltipLabel( this, MAIN_TOOLTIP, tooltipLabel );
}


void Sjf_mincerAudioProcessorEditor::delayTimeDisplay()
{
    nDivsLabels[ divLabels::delT ].setVisible( delayTimeSyncButton.getToggleState() );
    divTypeLabels[ divLabels::delT ].setVisible( delayTimeSyncButton.getToggleState() );
    delaySyncOffsetSlider.setVisible( delayTimeSyncButton.getToggleState() );
    delayTimeSyncDivisionNumberSlider.setVisible( delayTimeSyncButton.getToggleState() );
    delayTimeSyncDivisionBox.setVisible( delayTimeSyncButton.getToggleState() );
    delayTimeSlider.setVisible( !delayTimeSyncButton.getToggleState() );
}

void Sjf_mincerAudioProcessorEditor::jitterDisplayDisplay()
{
    nDivsLabels[ divLabels::delTJit ].setVisible( delayTimeJitterSyncButton.getToggleState() );
    divTypeLabels[ divLabels::delTJit ].setVisible( delayTimeJitterSyncButton.getToggleState() );
    delaySyncJitterNDivisionsSlider.setVisible( delayTimeJitterSyncButton.getToggleState() );
    delaySyncJitterDivisionBox.setVisible( delayTimeJitterSyncButton.getToggleState() );
    
    drawPanels( panelNames::delayTime );
}

void Sjf_mincerAudioProcessorEditor::rateDisplay()
{
    nDivsLabels[ divLabels::rate ].setVisible( rateSyncButton.getToggleState() );
    divTypeLabels[ divLabels::rate ].setVisible( rateSyncButton.getToggleState() );
    rateSlider.setVisible( !rateSyncButton.getToggleState() );
    rateSyncDivisionBox.setVisible( rateSyncButton.getToggleState() );
    rateSyncNumDivisionsSlider.setVisible( rateSyncButton.getToggleState() );
    
    drawPanels( panelNames::gRate );
}

void Sjf_mincerAudioProcessorEditor::fxDisplay()
{
    crushDisplay( fxNameButtons[ effects::crush ].getToggleState() );
    filterDisplay( fxNameButtons[ effects::filter ].getToggleState() );
    ringModDisplay( fxNameButtons[ effects::ringMod ].getToggleState() );
    drawPanels( panelNames::fx );
}

void Sjf_mincerAudioProcessorEditor::crushDisplay( bool isDisplayed )
{
    bitCrushButton.setVisible( isDisplayed );
    bitDepthSlider.setVisible( isDisplayed );
    srDividerSlider.setVisible( isDisplayed );
    bitsLabel.setVisible( isDisplayed );
    srDivLabel.setVisible( isDisplayed );
}

void Sjf_mincerAudioProcessorEditor::ringModDisplay( bool isDisplayed )
{
    ringModButton.setVisible( isDisplayed );
    ringModFreqSlider.setVisible( isDisplayed );
    ringModSpreadSlider.setVisible( isDisplayed );
    ringModSpreadLabel.setVisible( isDisplayed );
    ringModMixLabel.setVisible( isDisplayed );
    ringModMixSlider.setVisible( isDisplayed );
    ringModJitterLabel.setVisible( isDisplayed );
    ringModJitterSlider.setVisible( isDisplayed );
}

void Sjf_mincerAudioProcessorEditor::filterDisplay( bool isDisplayed )
{
    filterJitLabel.setVisible( isDisplayed );
    filterJitterSlider.setVisible( isDisplayed );
    filterButton.setVisible( isDisplayed );
    filterQSlider.setVisible( isDisplayed );
    filterTypeBox.setVisible( isDisplayed );
    filterFreqSlider.setVisible( isDisplayed );
}

void Sjf_mincerAudioProcessorEditor::outmodeDisplay()
{
    mixSlider.setVisible( ( outputModeBox.getSelectedId() == 1 ) );
}

void Sjf_mincerAudioProcessorEditor::drawPanels( int panelNum )
{
    float panelX = 0.0, panelY = 0.0, panelW = 0.0, panelH = 0.0;
    switch( panelNum )
    {
        case panelNames::delayTime :
        {
            panelX = delayTimeSyncButton.getX() - PANEL_SPACING;
            panelY = delayTimeSyncButton.getY() - TEXT_HEIGHT;
            panelW = SLIDERSIZE + 2*PANEL_SPACING;
            panelH = delayTimeJitterSyncButton.getToggleState() ? delaySyncJitterDivisionBox.getBottom() - panelY : delayTimeJitterSlider.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::feedback :
        {
            panelX = feedbackSlider.getX() - PANEL_SPACING;
            panelY = feedbackSlider.getY() - TEXT_HEIGHT;
            panelW = SLIDERSIZE + 2*PANEL_SPACING;
            panelH = feedbackSlider.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::gRate :
        {
            panelX = rateSyncButton.getX() - PANEL_SPACING;
            panelY = rateSyncButton.getY() - TEXT_HEIGHT;
            panelW = SLIDERSIZE + 2*PANEL_SPACING;
            panelH = rateSyncButton.getToggleState() ? rateSyncDivisionBox.getBottom() - panelY : rateSlider.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::transposition :
        {
            panelX = transpositionSlider.getX() - PANEL_SPACING;
            panelY = transpositionSlider.getY() - TEXT_HEIGHT;
            panelW = SLIDERSIZE + 2*PANEL_SPACING;
            panelH = harmonySliders[ 0 ].getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::gParams :
        {
            panelX = crossTalkSlider.getX() - PANEL_SPACING;
            panelY = crossTalkSlider.getY() - 2*TEXT_HEIGHT;
            panelW = densitySlider.getRight() - panelX;
            panelW += PANEL_SPACING;
            panelH = reverseSlider.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::fx :
        {
            panelX = fxNameButtons[ effects::crush ].getX() - PANEL_SPACING;
            panelY = fxNameButtons[ effects::crush ].getY() - TEXT_HEIGHT;
            panelW = SLIDERSIZE + 2*PANEL_SPACING;
            if ( fxNameButtons[ effects::crush ].getToggleState() )
                panelH = srDividerSlider.getBottom() - panelY;
            else if ( fxNameButtons[ effects::filter ].getToggleState() )
                panelH =  filterJitterSlider.getBottom() - panelY;
            else if ( fxNameButtons[ effects::ringMod ].getToggleState() )
                panelH = ringModJitterSlider.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
        case panelNames::output :
        {
            panelX = outputLevelSlider.getX() - PANEL_SPACING;
            panelY = outputLevelSlider.getY() - TEXT_HEIGHT;
            panelW = outputLevelSlider.getRight() - panelX;
            panelW += PANEL_SPACING;
            panelH = interpolationTypeBox.getBottom() - panelY;
            panelH += PANEL_SPACING;
            break;
        }
    }
    panels[ panelNum ].setBounds(  panelX, panelY, panelW, panelH );
}
