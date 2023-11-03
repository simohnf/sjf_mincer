/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TEXT_HEIGHT 20
#define INDENT 10
#define SLIDERSIZE 100
#define WIDTH SLIDERSIZE*8 +INDENT*13
#define HEIGHT TEXT_HEIGHT*6 + INDENT + SLIDERSIZE*2
//==============================================================================
Sjf_mincerAudioProcessorEditor::Sjf_mincerAudioProcessorEditor (Sjf_mincerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), valueTreeState( vts )
{
    auto divNames = audioProcessor.getDivNames();
    auto interpNames = audioProcessor.getInterpNames();
    
    setLookAndFeel( &otherLookAndFeel );
    otherLookAndFeel.drawComboBoxTick = false;
    
//        outputModeBox, interpolationTypeBox, rateSyncDivisionBox, delayTimeSyncDivisionBox, delaySyncJitterDivisionBox, rateSyncDivision;
//    outputModeBoxAttachment, interpolationTypeBoxAttachment, rateSyncDivisionBoxAttachment, delayTimeSyncDivisionBoxAttachment, delaySyncJitterDivisionBoxAttachment, rateSyncDivisionAttachment;
//    delayTimeSlider, delayTimeJitterSlider, feedbackSlider, reverseSlider, repeatSlider;
    
    addAndMakeVisible( &delayTimeSyncButton );
    delayTimeSyncButton.setButtonText( "Sync" );
    delayTimeSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "delayTimeSync", delayTimeSyncButton ) );
    delayTimeSyncButton.onClick = [ this ]
    {
        delayTimeDisplay();
    };
    
    addAndMakeVisible( &delayTimeSlider );
    delayTimeSlider.setSliderStyle( juce::Slider::Rotary );
    delayTimeSlider.setTextValueSuffix("ms");
    delayTimeSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "delayTime", delayTimeSlider )  );
    delayTimeSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, delayTimeSlider.getWidth(), TEXT_HEIGHT );
//    delayTimeSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    delayTimeSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delayTimeSyncDivisionBox );
    for ( auto i = 0; i < divNames.size(); i++ )
        delayTimeSyncDivisionBox.addItem( divNames[ i ], i+1 );
    delayTimeSyncDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "division", delayTimeSyncDivisionBox )  );
    delayTimeSyncDivisionBox.sendLookAndFeelChange();
    
//    delayTimeSyncDivisionNumberAttachment
    addAndMakeVisible( &delayTimeSyncDivisionNumberSlider );
    delayTimeSyncDivisionNumberAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "numDivisions", delayTimeSyncDivisionNumberSlider )  );
    delayTimeSyncDivisionNumberSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delaySyncOffsetSlider );
    delaySyncOffsetSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "syncOffset", delaySyncOffsetSlider )  );
    delaySyncOffsetSlider.sendLookAndFeelChange();
    delaySyncOffsetSlider.setTextValueSuffix("%");
    
    addAndMakeVisible( &offsetLabel );
    offsetLabel.attachToComponent( &delaySyncOffsetSlider, false );
    offsetLabel.setText( "offset", juce::dontSendNotification );
    offsetLabel.setJustificationType( juce::Justification::centred );
    
    addAndMakeVisible( &feedbackSlider );
    feedbackSlider.setSliderStyle( juce::Slider::Rotary );
    feedbackSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "feedback", feedbackSlider )  );
    feedbackSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, feedbackSlider.getWidth(), TEXT_HEIGHT );
//    feedbackSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    feedbackSlider.sendLookAndFeelChange();
    feedbackSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &feedbackControlButton );
    feedbackControlButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "feedbackControl", feedbackControlButton ) );
    feedbackControlButton.sendLookAndFeelChange();
    
    addAndMakeVisible( &delayTimeJitterSyncButton );
    delayTimeJitterSyncButton.setButtonText( "Sync" );
    delayTimeJitterSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "syncJitter", delayTimeJitterSyncButton ) );
    delayTimeJitterSyncButton.onClick = [ this ]
    {
        jitterDisplayDisplay();
    };
    
    addAndMakeVisible( &delayTimeJitterSlider);
    delayTimeJitterSlider.setSliderStyle( juce::Slider::Rotary );
    delayTimeJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "delayTimeJitter", delayTimeJitterSlider )  );
    delayTimeJitterSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, delayTimeJitterSlider.getWidth(), TEXT_HEIGHT );
//    delayTimeJitterSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    delayTimeJitterSlider.sendLookAndFeelChange();
    delayTimeJitterSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &delaySyncJitterNDivisionsSlider );
    delaySyncJitterNDivisionsSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "syncedJitterMaxNumDivisions", delaySyncJitterNDivisionsSlider )  );
    delaySyncJitterNDivisionsSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &delaySyncJitterDivisionBox );
    for ( auto i = 0; i < divNames.size(); i++ )
        delaySyncJitterDivisionBox.addItem( divNames[ i ], i+1 );
    delaySyncJitterDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "syncedJitterDivision", delaySyncJitterDivisionBox )  );
    delaySyncJitterDivisionBox.sendLookAndFeelChange();
    
    
    

    
    
    addAndMakeVisible( &rateSyncButton );
    rateSyncButton.setButtonText( "Sync" );
    rateSyncButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "rateSync", rateSyncButton ) );
    rateSyncButton.onClick = [ this ]
    {
        rateDisplay();
    };
    
    addAndMakeVisible( &rateSlider );
    rateSlider.setSliderStyle( juce::Slider::Rotary );
    rateSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "rate", rateSlider )  );
    rateSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, rateSlider.getWidth(), TEXT_HEIGHT );
//    delayTimeJitterSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    rateSlider.sendLookAndFeelChange();
    rateSlider.setTextValueSuffix( " Hz" );
    
    
    
    
    addAndMakeVisible( &rateSyncDivisionBox );
    for ( auto i = 0; i < divNames.size(); i++ )
        rateSyncDivisionBox.addItem( divNames[ i ], i+1 );
    rateSyncDivisionBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "rateSyncDivision", rateSyncDivisionBox )  );
    rateSyncDivisionBox.sendLookAndFeelChange();
    
//    delayTimeSyncDivisionNumberAttachment
    addAndMakeVisible( &rateSyncNumDivisionsSlider );
    rateSyncNumDivisionsSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "rateSyncNumDivisions", rateSyncNumDivisionsSlider )  );
    rateSyncNumDivisionsSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &transpositionSlider );
    transpositionSlider.setSliderStyle( juce::Slider::Rotary );
    transpositionSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "transposition", transpositionSlider )  );
    transpositionSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, transpositionSlider.getWidth(), TEXT_HEIGHT );
//    delayTimeJitterSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    transpositionSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &transpositionJitterSlider );
    transpositionJitterSlider.setSliderStyle( juce::Slider::Rotary );
    transpositionJitterSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "transpositionJitter", transpositionJitterSlider )  );
    transpositionJitterSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, transpositionJitterSlider.getWidth(), TEXT_HEIGHT );
//    delayTimeJitterSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    transpositionJitterSlider.sendLookAndFeelChange();
    transpositionJitterSlider.setTextValueSuffix( "%" );

    addAndMakeVisible( &harmonyDislayBox );
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        addAndMakeVisible( &harmonySliders[ i ] );
        harmonySliders[ i ].setSliderStyle( juce::Slider::Rotary );
        harmonySlidersAttachments[ i ].reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "harmony" + juce::String( i ), harmonySliders[ i ] )  );
        harmonySliders[ i ].setTextBoxStyle( juce::Slider::TextBoxBelow, false, harmonySliders[ i ].getWidth(), TEXT_HEIGHT );
    //    delayTimeJitterSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
        harmonySliders[ i ].sendLookAndFeelChange();
        
        
        addAndMakeVisible( &harmonyButtons[ i ] );
        harmonyButtonsAttachments[ i ].reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "harmonyOn" + juce::String( i ), harmonyButtons[ i ] ) );
        harmonyButtons[ i ].sendLookAndFeelChange();
        
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
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    reverseSlider.sendLookAndFeelChange();
    reverseSlider.setTextValueSuffix( "%" );
    
    addAndMakeVisible( &repeatSlider );
    repeatSlider.setSliderStyle( juce::Slider::Rotary );
    repeatSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "repeat", repeatSlider )  );
    repeatSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, repeatSlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    repeatSlider.sendLookAndFeelChange();
    repeatSlider.setTextValueSuffix( "%" );
    

    addAndMakeVisible( &crossTalkSlider );
    crossTalkSlider.setSliderStyle( juce::Slider::Rotary );
    crossTalkSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "crossTalk", crossTalkSlider )  );
    crossTalkSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, crossTalkSlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    crossTalkSlider.sendLookAndFeelChange();
    crossTalkSlider.setTextValueSuffix( "%" );
    
    addAndMakeVisible( &densitySlider );
    densitySlider.setSliderStyle( juce::Slider::Rotary );
    densitySliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "density", densitySlider )  );
    densitySlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, densitySlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    densitySlider.sendLookAndFeelChange();
    densitySlider.setTextValueSuffix( "%" );

    addAndMakeVisible( &bitCrushButton );
    bitCrushButton.setButtonText( "bitCrush" );
    bitCrushButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "crush", bitCrushButton ) );
    bitCrushButton.onClick = [ this ]
    {
        crushDisplay();
    };
    
    
//    bitDepthSlider, srDividerSlider
    addAndMakeVisible( &bitDepthSlider );
    bitDepthSlider.setTextValueSuffix( " bits" );
//    bitDepthSlider.setSliderStyle( juce::Slider::Rotary );
    bitDepthSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "bitDepth", bitDepthSlider )  );
//    bitDepthSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, bitDepthSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    bitDepthSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &srDividerSlider );
//    srDividerSlider.setSliderStyle( juce::Slider::Rotary );
    srDividerSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "sampleRateDivider", srDividerSlider )  );
//    srDividerSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, reverseSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    srDividerSlider.sendLookAndFeelChange();
    srDividerSlider.setTextValueSuffix(" srDiv");
    
    

    
    addAndMakeVisible( &filterFreqSlider );
    filterFreqSlider.setTextValueSuffix( "Hz" );
    filterFreqSlider.setSliderStyle( juce::Slider::Rotary );
    filterFreqSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "filterFrequency", filterFreqSlider )  );
    filterFreqSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, bitDepthSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    filterFreqSlider.sendLookAndFeelChange();
    
    
    addAndMakeVisible( &filterQSlider );
    filterQSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "filterQ", filterQSlider )  );
    filterQSlider.setTextValueSuffix( "Q" );
    
    auto filterTypes = audioProcessor.getFilterTypes();
    addAndMakeVisible( &filterTypeBox );
    for ( auto i = 0; i < filterTypes.size(); i++ )
        filterTypeBox.addItem( filterTypes[ i ], i + 1 );
    filterTypeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "filterType", filterTypeBox )  );
    filterTypeBox.sendLookAndFeelChange();
    
    addAndMakeVisible( &filterButton );
    filterButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "filterActive", filterButton ) );
    filterButton.setButtonText( "filter" );
    
    
    addAndMakeVisible( &outputModeBox );
    outputModeBox.addItem( "mix", 1 );
    outputModeBox.addItem( "insert", 2 );
    outputModeBox.addItem( "gate", 3 );
    outputModeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "outMode", outputModeBox )  );
    outputModeBox.sendLookAndFeelChange();
    outputModeBox.onChange = [this]
    {
        outmodeDisplay();
    };
    
    addAndMakeVisible( &mixSlider );
    mixSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "mix", mixSlider )  );
    mixSlider.sendLookAndFeelChange();
    mixSlider.setTextValueSuffix( "%" );
    
    
    addAndMakeVisible( &interpolationTypeBox );
    for ( auto i = 0; i < interpNames.size(); i++ )
        interpolationTypeBox.addItem( interpNames[ i ], i+1 );
    interpolationTypeBoxAttachment.reset( new juce::AudioProcessorValueTreeState::ComboBoxAttachment  ( valueTreeState, "interpolation", interpolationTypeBox )  );
    interpolationTypeBox.sendLookAndFeelChange();
    
    addAndMakeVisible( &outputLevelSlider );
    outputLevelSlider.setSliderStyle( juce::Slider::Rotary );
    outputLevelSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, outputLevelSlider.getWidth(), TEXT_HEIGHT );
    outputLevelSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "outputLevel", outputLevelSlider )  );
//    outputLevelSlider.setSkewFactorFromMidPoint( -6.0 );
    outputLevelSlider.setNumDecimalPlacesToDisplay( 1 );
    outputLevelSlider.sendLookAndFeelChange();
    outputLevelSlider.setTextValueSuffix( "dB" );
    
    
    delayTimeDisplay();
    jitterDisplayDisplay();
    rateDisplay();
    crushDisplay();
    outmodeDisplay();
    
    setSize ( WIDTH, HEIGHT );
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
    
//    g.drawFittedText ("offset", delaySyncOffsetSlider.getX(), delaySyncOffsetSlider.getY() - TEXT_HEIGHT, delaySyncOffsetSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("dtJitter", delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getY() - TEXT_HEIGHT, delayTimeJitterSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("rate", rateSyncButton.getX(), rateSyncButton.getY() - TEXT_HEIGHT, rateSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("transposition", transpositionSlider.getX(), transpositionSlider.getY() - TEXT_HEIGHT, transpositionSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("trJitter", transpositionJitterSlider.getX(), transpositionJitterSlider.getY() - TEXT_HEIGHT, transpositionJitterSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);

    
    g.drawFittedText ("crosstalk", crossTalkSlider.getX(), crossTalkSlider.getY() - TEXT_HEIGHT, crossTalkSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("density", densitySlider.getX(), densitySlider.getY() - TEXT_HEIGHT, densitySlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("reverse", reverseSlider.getX(), reverseSlider.getY() - TEXT_HEIGHT, reverseSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("repeat", repeatSlider.getX(), repeatSlider.getY() - TEXT_HEIGHT, repeatSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
//    g.drawFittedText ("bitDepth", bitDepthSlider.getX(), bitDepthSlider.getY() - TEXT_HEIGHT, bitDepthSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
//    g.drawFittedText ("SR Divider", srDividerSlider.getX(), srDividerSlider.getY() - TEXT_HEIGHT, srDividerSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("feedback", feedbackSlider.getX(), feedbackSlider.getY() - TEXT_HEIGHT, feedbackSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    g.drawFittedText ("output", outputLevelSlider.getX(), outputLevelSlider.getY() - TEXT_HEIGHT, outputLevelSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
}

void Sjf_mincerAudioProcessorEditor::resized()
{
    delayTimeSyncButton.setBounds( INDENT, TEXT_HEIGHT*2, SLIDERSIZE, TEXT_HEIGHT );
    
    delayTimeSlider.setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    delayTimeSyncDivisionNumberSlider.setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE / 2, SLIDERSIZE / 2 );
    delayTimeSyncDivisionBox.setBounds( delayTimeSyncDivisionNumberSlider.getRight(), delayTimeSyncDivisionNumberSlider.getY(), SLIDERSIZE / 2, SLIDERSIZE / 2  );
    delaySyncOffsetSlider.setBounds( delayTimeSlider.getX(), delayTimeSyncDivisionBox.getBottom()+TEXT_HEIGHT+INDENT/2, SLIDERSIZE, TEXT_HEIGHT   );

    delayTimeJitterSyncButton.setBounds( delayTimeSyncButton.getX(), delayTimeSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );
    delayTimeJitterSlider.setBounds( delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    delaySyncJitterNDivisionsSlider.setBounds( delayTimeJitterSlider.getX(), delayTimeJitterSlider.getBottom(), SLIDERSIZE / 2, TEXT_HEIGHT );
    delaySyncJitterDivisionBox.setBounds( delaySyncJitterNDivisionsSlider.getRight(), delaySyncJitterNDivisionsSlider.getY(), SLIDERSIZE / 2, TEXT_HEIGHT );

//    auto fbSlY = ( delaySyncJitterDivisionBox.getBottom() - delayTimeSlider.getY() ) / 2;
    feedbackSlider.setBounds( delayTimeSlider.getRight()+INDENT, delayTimeSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    feedbackControlButton.setBounds( feedbackSlider.getRight() - TEXT_HEIGHT, feedbackSlider.getY(), TEXT_HEIGHT, TEXT_HEIGHT );
    
    
    rateSyncButton.setBounds( feedbackSlider.getX(), delayTimeJitterSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    rateSlider.setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    rateSyncNumDivisionsSlider.setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE / 2, SLIDERSIZE / 2 );
    rateSyncDivisionBox.setBounds( rateSyncNumDivisionsSlider.getRight(), rateSyncNumDivisionsSlider.getY(), SLIDERSIZE / 2, SLIDERSIZE / 2 );
    
    transpositionSlider.setBounds( rateSyncButton.getRight()+INDENT*2, delayTimeSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    transpositionJitterSlider.setBounds( transpositionSlider.getRight() + INDENT, transpositionSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    
    harmonyDislayBox.setBounds( transpositionSlider.getX() + SLIDERSIZE/2 + INDENT/2, rateSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    for ( auto i = 0; i < MAX_N_HARMONIES; i++ )
    {
        harmonySliders[ i ].setBounds( harmonyDislayBox.getX(), harmonyDislayBox.getBottom(), SLIDERSIZE, SLIDERSIZE );
        harmonyButtons[ i ].setBounds( harmonySliders[ i ].getRight() - TEXT_HEIGHT, harmonySliders[ i ].getY(), TEXT_HEIGHT, TEXT_HEIGHT );
    }

    crossTalkSlider.setBounds( transpositionJitterSlider.getRight()+INDENT*2, transpositionSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    densitySlider.setBounds( crossTalkSlider.getRight()+INDENT, crossTalkSlider.getY(), SLIDERSIZE, SLIDERSIZE );


    reverseSlider.setBounds( crossTalkSlider.getX(), harmonyDislayBox.getBottom(), SLIDERSIZE, SLIDERSIZE );
    repeatSlider.setBounds( densitySlider.getX(), reverseSlider.getY(), SLIDERSIZE, SLIDERSIZE );


    bitCrushButton.setBounds( densitySlider.getRight() + INDENT*2, densitySlider.getY(), SLIDERSIZE, TEXT_HEIGHT );
    bitDepthSlider.setBounds( bitCrushButton.getX(), bitCrushButton.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    srDividerSlider.setBounds( bitDepthSlider.getX(), bitDepthSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );

    
    filterButton.setBounds( srDividerSlider.getX(), srDividerSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );
    filterTypeBox.setBounds( filterButton.getX(), filterButton.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    filterFreqSlider.setBounds( filterTypeBox.getX(), filterTypeBox.getBottom(), SLIDERSIZE, SLIDERSIZE );
    filterQSlider.setBounds( filterFreqSlider.getX(), filterFreqSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    
    
    outputLevelSlider.setBounds( bitCrushButton.getRight()+INDENT*2, bitCrushButton.getY()+TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
        
    outputModeBox.setBounds( outputLevelSlider.getX(), outputLevelSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    mixSlider.setBounds( outputModeBox.getX(), outputModeBox.getBottom(), SLIDERSIZE, TEXT_HEIGHT );

    interpolationTypeBox.setBounds( mixSlider.getX(), mixSlider.getBottom()+TEXT_HEIGHT, SLIDERSIZE, TEXT_HEIGHT );

    

}
