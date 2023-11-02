/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TEXT_HEIGHT 20
#define INDENT 10
#define SLIDERSIZE 80
#define WIDTH SLIDERSIZE*8 +INDENT*2
#define HEIGHT TEXT_HEIGHT*3 + INDENT*4 + SLIDERSIZE*5
//==============================================================================
Sjf_mincerAudioProcessorEditor::Sjf_mincerAudioProcessorEditor (Sjf_mincerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), valueTreeState( vts )
{
    auto divNames = audioProcessor.getDivNames();
    setLookAndFeel( &otherLookAndFeel );
    
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
    
    
    
    
    addAndMakeVisible( &feedbackSlider );
    feedbackSlider.setSliderStyle( juce::Slider::Rotary );
    feedbackSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "feedback", feedbackSlider )  );
    feedbackSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, feedbackSlider.getWidth(), TEXT_HEIGHT );
//    feedbackSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    feedbackSlider.sendLookAndFeelChange();
    
    
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
    

    
    
    
    
    
    
    
    
    addAndMakeVisible( &reverseSlider );
    reverseSlider.setSliderStyle( juce::Slider::Rotary );
    reverseSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "reverse", reverseSlider )  );
    reverseSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, reverseSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    reverseSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &repeatSlider );
    repeatSlider.setSliderStyle( juce::Slider::Rotary );
    repeatSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "repeat", repeatSlider )  );
    repeatSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, repeatSlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    repeatSlider.sendLookAndFeelChange();
    

    addAndMakeVisible( &crossTalkSlider );
    crossTalkSlider.setSliderStyle( juce::Slider::Rotary );
    crossTalkSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "crossTalk", crossTalkSlider )  );
    crossTalkSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, crossTalkSlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    crossTalkSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &densitySlider );
    densitySlider.setSliderStyle( juce::Slider::Rotary );
    densitySliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "density", densitySlider )  );
    densitySlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, densitySlider.getWidth(), TEXT_HEIGHT );
//    repeatSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    densitySlider.sendLookAndFeelChange();
    

    addAndMakeVisible( &bitCrushButton );
    bitCrushButton.setButtonText( "bitCrush" );
    bitCrushButtonAttachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment( valueTreeState, "crush", bitCrushButton ) );
    bitCrushButton.onClick = [ this ]
    {
        crushDisplay();
    };
    
    
//    bitDepthSlider, srDividerSlider
    addAndMakeVisible( &bitDepthSlider );
    bitDepthSlider.setSliderStyle( juce::Slider::Rotary );
    bitDepthSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "bitDepth", bitDepthSlider )  );
    bitDepthSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, bitDepthSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    bitDepthSlider.sendLookAndFeelChange();
    
    addAndMakeVisible( &srDividerSlider );
    srDividerSlider.setSliderStyle( juce::Slider::Rotary );
    srDividerSliderAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment ( valueTreeState, "sampleRateDivider", srDividerSlider )  );
    srDividerSlider.setTextBoxStyle( juce::Slider::TextBoxBelow, false, reverseSlider.getWidth(), TEXT_HEIGHT );
//    reverseSlider.setTooltip( "This sets the overall probability of different divisions being included in the rhythmic pattern" );
    srDividerSlider.sendLookAndFeelChange();
    
    
    
    delayTimeDisplay();
    jitterDisplayDisplay();
    rateDisplay();
    
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
    g.drawFittedText ("dtJitter", delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getY() - TEXT_HEIGHT, delayTimeJitterSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("rate", rateSyncButton.getX(), rateSyncButton.getY() - TEXT_HEIGHT, rateSyncButton.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("transposition", transpositionSlider.getX(), transpositionSlider.getY() - TEXT_HEIGHT, transpositionSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("trJitter", transpositionJitterSlider.getX(), transpositionJitterSlider.getY() - TEXT_HEIGHT, transpositionJitterSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);

    
    g.drawFittedText ("crosstalk", crossTalkSlider.getX(), crossTalkSlider.getY() - TEXT_HEIGHT, crossTalkSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("density", densitySlider.getX(), densitySlider.getY() - TEXT_HEIGHT, densitySlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("reverse", reverseSlider.getX(), reverseSlider.getY() - TEXT_HEIGHT, reverseSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    g.drawFittedText ("repeat", repeatSlider.getX(), repeatSlider.getY() - TEXT_HEIGHT, repeatSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("feedback", feedbackSlider.getX(), feedbackSlider.getY() - TEXT_HEIGHT, feedbackSlider.getWidth(), TEXT_HEIGHT, juce::Justification::centred, 1);
}

void Sjf_mincerAudioProcessorEditor::resized()
{
    delayTimeSyncButton.setBounds( INDENT, TEXT_HEIGHT*2, SLIDERSIZE, TEXT_HEIGHT );
    
    delayTimeSlider.setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    delayTimeSyncDivisionNumberSlider.setBounds( delayTimeSyncButton.getX(), delayTimeSyncButton.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    delayTimeSyncDivisionBox.setBounds( delayTimeSyncDivisionNumberSlider.getX(), delayTimeSyncDivisionNumberSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    delaySyncOffsetSlider.setBounds( delayTimeSyncDivisionBox.getX(), delayTimeSyncDivisionBox.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    
    feedbackSlider.setBounds( delayTimeSyncDivisionBox.getX(), delayTimeSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
    
    delayTimeJitterSyncButton.setBounds( delayTimeSyncButton.getRight()+INDENT, delayTimeSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    delayTimeJitterSlider.setBounds( delayTimeJitterSyncButton.getX(), delayTimeJitterSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    delaySyncJitterNDivisionsSlider.setBounds( delayTimeJitterSlider.getX(), delayTimeJitterSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    delaySyncJitterDivisionBox.setBounds( delaySyncJitterNDivisionsSlider.getX(), delaySyncJitterNDivisionsSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );

    
    rateSyncButton.setBounds( delayTimeJitterSyncButton.getRight()+INDENT, delayTimeJitterSyncButton.getY(), SLIDERSIZE, TEXT_HEIGHT );
    rateSlider.setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE, SLIDERSIZE );
    rateSyncNumDivisionsSlider.setBounds( rateSyncButton.getX(), rateSyncButton.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    rateSyncDivisionBox.setBounds( rateSyncNumDivisionsSlider.getX(), rateSyncNumDivisionsSlider.getBottom(), SLIDERSIZE, TEXT_HEIGHT );
    
    transpositionSlider.setBounds( rateSyncButton.getRight()+INDENT, rateSyncButton.getY(), SLIDERSIZE, SLIDERSIZE );
    transpositionJitterSlider.setBounds( transpositionSlider.getX(), transpositionSlider.getBottom()+TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
    
    
    crossTalkSlider.setBounds( transpositionSlider.getRight()+INDENT, transpositionSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    densitySlider.setBounds( crossTalkSlider.getRight()+INDENT, crossTalkSlider.getY(), SLIDERSIZE, SLIDERSIZE );
    
    
    reverseSlider.setBounds( crossTalkSlider.getX(), crossTalkSlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
    repeatSlider.setBounds( densitySlider.getX(), densitySlider.getBottom() + TEXT_HEIGHT, SLIDERSIZE, SLIDERSIZE );
}
