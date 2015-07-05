/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
#include "ParamGraph.h"
#include "ResponseGraph.h"
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (TimeLagFilterAudioProcessor& p)
    : processor(p)
{
    addAndMakeVisible (grpInterpolator = new GroupComponent ("new group",
                                                             TRANS("Interpolator")));

    addAndMakeVisible (grpNonRealtime = new GroupComponent ("new group",
                                                            TRANS("Time-Lag Filter")));

    addAndMakeVisible (grpDecimator = new GroupComponent ("new group",
                                                          TRANS("Decimator")));

    addAndMakeVisible (groupComponent2 = new GroupComponent ("new group",
                                                             TRANS("Phase Bandwidth vs. Filter Index")));

    addAndMakeVisible (groupComponent = new GroupComponent ("new group",
                                                            TRANS("Center Frequency vs. Filter Index")));

    addAndMakeVisible (chkResample = new ToggleButton ("new toggle button"));
    chkResample->setTooltip (TRANS("Reduce sample rate before time-lag filter (reduces CPU usage)."));
    chkResample->setButtonText (TRANS("Enable"));
    chkResample->addListener (this);

    addAndMakeVisible (cbxSampleRate = new ComboBox ("new combo box"));
    cbxSampleRate->setTooltip (TRANS("Sample rate to reduce input signal to."));
    cbxSampleRate->setEditableText (false);
    cbxSampleRate->setJustificationType (Justification::centredLeft);
    cbxSampleRate->setTextWhenNothingSelected (TRANS("48000"));
    cbxSampleRate->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbxSampleRate->addItem (TRANS("48000"), 1);
    cbxSampleRate->addItem (TRANS("44100"), 2);
    cbxSampleRate->addItem (TRANS("32000"), 3);
    cbxSampleRate->addItem (TRANS("24000"), 4);
    cbxSampleRate->addItem (TRANS("16000"), 5);
    cbxSampleRate->addItem (TRANS("12000"), 6);
    cbxSampleRate->addItem (TRANS("8000"), 7);
    cbxSampleRate->addListener (this);

    addAndMakeVisible (chkDecimFilter = new ToggleButton ("new toggle button"));
    chkDecimFilter->setTooltip (TRANS("Use antialiasing filter on input before decimation (recommended, unless you want strange output)."));
    chkDecimFilter->setButtonText (TRANS("Antialiasing"));
    chkDecimFilter->addListener (this);
    chkDecimFilter->setToggleState (true, dontSendNotification);

    addAndMakeVisible (chkInterpFilter = new ToggleButton ("new toggle button"));
    chkInterpFilter->setTooltip (TRANS("Use reconstruction filter after interpolation (recommended, unless you want strange output)."));
    chkInterpFilter->setButtonText (TRANS("Antialiasing"));
    chkInterpFilter->addListener (this);
    chkInterpFilter->setToggleState (true, dontSendNotification);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Total processed (max) filter chain length:\n"
                                          "(Amount of CPU usage, not modulatable)")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (txtMaxChain = new TextEditor ("new text editor"));
    txtMaxChain->setTooltip (TRANS("The plugin is always processing all these filters, even if the output is coming from not the last one in the chain."));
    txtMaxChain->setMultiLine (false);
    txtMaxChain->setReturnKeyStartsNewLine (false);
    txtMaxChain->setReadOnly (false);
    txtMaxChain->setScrollbarsShown (false);
    txtMaxChain->setCaretVisible (true);
    txtMaxChain->setPopupMenuEnabled (false);
    txtMaxChain->setColour (TextEditor::outlineColourId, Colour (0xffb0b0b0));
    txtMaxChain->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    txtMaxChain->setText (TRANS("128"));

    addAndMakeVisible (sldChain = new Slider ("new slider"));
    sldChain->setRange (1, 128, 1);
    sldChain->setSliderStyle (Slider::LinearHorizontal);
    sldChain->setTextBoxStyle (Slider::TextBoxRight, false, 40, 20);
    sldChain->addListener (this);

    addAndMakeVisible (groupComponent4 = new GroupComponent ("new group",
                                                             TRANS("Input")));

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Left Send to Filter")));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("Right Send to Filter")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sldInLeft = new Slider ("new slider"));
    sldInLeft->setTooltip (TRANS("L channel input signal to filter input. Negative is phase invert."));
    sldInLeft->setRange (-100, 100, 1);
    sldInLeft->setSliderStyle (Slider::LinearHorizontal);
    sldInLeft->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    sldInLeft->addListener (this);

    addAndMakeVisible (sldInRight = new Slider ("new slider"));
    sldInRight->setTooltip (TRANS("R channel input signal to filter input. Negative is phase invert."));
    sldInRight->setRange (-100, 100, 1);
    sldInRight->setSliderStyle (Slider::LinearHorizontal);
    sldInRight->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    sldInRight->addListener (this);

    addAndMakeVisible (label7 = new Label ("new label",
                                           TRANS("Filter to take output from (modulatable):")));
    label7->setFont (Font (15.00f, Font::plain));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lblInterpolator = new Label ("new label",
                                                    TRANS("Settings from\n"
                                                    "decimator")));
    lblInterpolator->setFont (Font (15.00f, Font::plain));
    lblInterpolator->setJustificationType (Justification::centredLeft);
    lblInterpolator->setEditable (false, false, false);
    lblInterpolator->setColour (TextEditor::textColourId, Colours::black);
    lblInterpolator->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (groupComponent7 = new GroupComponent ("new group",
                                                             TRANS("Output")));

    addAndMakeVisible (label9 = new Label ("new label",
                                           TRANS("Left:")));
    label9->setFont (Font (15.00f, Font::plain));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label10 = new Label ("new label",
                                            TRANS("Right:")));
    label10->setFont (Font (15.00f, Font::plain));
    label10->setJustificationType (Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label11 = new Label ("new label",
                                            TRANS("Dry L")));
    label11->setFont (Font (15.00f, Font::plain));
    label11->setJustificationType (Justification::centredLeft);
    label11->setEditable (false, false, false);
    label11->setColour (TextEditor::textColourId, Colours::black);
    label11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label12 = new Label ("new label",
                                            TRANS("Dry R")));
    label12->setFont (Font (15.00f, Font::plain));
    label12->setJustificationType (Justification::centredLeft);
    label12->setEditable (false, false, false);
    label12->setColour (TextEditor::textColourId, Colours::black);
    label12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label13 = new Label ("new label",
                                            TRANS("Wet")));
    label13->setFont (Font (15.00f, Font::plain));
    label13->setJustificationType (Justification::centredLeft);
    label13->setEditable (false, false, false);
    label13->setColour (TextEditor::textColourId, Colours::black);
    label13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sldOutLDryL = new Slider ("new slider"));
    sldOutLDryL->setTooltip (TRANS("L channel input to L output. Negative is phase invert."));
    sldOutLDryL->setRange (-100, 100, 0);
    sldOutLDryL->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutLDryL->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutLDryL->addListener (this);

    addAndMakeVisible (sldOutLDryR = new Slider ("new slider"));
    sldOutLDryR->setTooltip (TRANS("R channel input to L output. Negative is phase invert."));
    sldOutLDryR->setRange (-100, 100, 0);
    sldOutLDryR->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutLDryR->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutLDryR->addListener (this);

    addAndMakeVisible (sldOutLWet = new Slider ("new slider"));
    sldOutLWet->setTooltip (TRANS("Time-lag filter to L output. Negative is phase invert."));
    sldOutLWet->setRange (-100, 100, 0);
    sldOutLWet->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutLWet->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutLWet->addListener (this);

    addAndMakeVisible (sldOutRDryL = new Slider ("new slider"));
    sldOutRDryL->setTooltip (TRANS("L channel input to R output. Negative is phase invert."));
    sldOutRDryL->setRange (-100, 100, 0);
    sldOutRDryL->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutRDryL->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutRDryL->addListener (this);

    addAndMakeVisible (sldOutRDryR = new Slider ("new slider"));
    sldOutRDryR->setTooltip (TRANS("R channel input to R output. Negative is phase invert."));
    sldOutRDryR->setRange (-100, 100, 0);
    sldOutRDryR->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutRDryR->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutRDryR->addListener (this);

    addAndMakeVisible (sldOutRWet = new Slider ("new slider"));
    sldOutRWet->setTooltip (TRANS("Time-lag filter to R output. Negative is phase invert."));
    sldOutRWet->setRange (-100, 100, 0);
    sldOutRWet->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    sldOutRWet->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sldOutRWet->addListener (this);

    addAndMakeVisible (groupComponent3 = new GroupComponent ("new group",
                                                             TRANS("Phase Response")));

    addAndMakeVisible (optCtrFixed = new ToggleButton ("new toggle button"));
    optCtrFixed->setButtonText (TRANS("Fixed"));
    optCtrFixed->setRadioGroupId (1);
    optCtrFixed->addListener (this);
    optCtrFixed->setToggleState (true, dontSendNotification);

    addAndMakeVisible (optCtrLinear = new ToggleButton ("new toggle button"));
    optCtrLinear->setButtonText (TRANS("Linear"));
    optCtrLinear->setRadioGroupId (1);
    optCtrLinear->addListener (this);

    addAndMakeVisible (optCtrLog = new ToggleButton ("new toggle button"));
    optCtrLog->setButtonText (TRANS("Log"));
    optCtrLog->setRadioGroupId (1);
    optCtrLog->addListener (this);

    addAndMakeVisible (optCtrGauss = new ToggleButton ("new toggle button"));
    optCtrGauss->setButtonText (TRANS("Gaussian"));
    optCtrGauss->setRadioGroupId (1);
    optCtrGauss->addListener (this);

    addAndMakeVisible (optCtrDual = new ToggleButton ("new toggle button"));
    optCtrDual->setButtonText (TRANS("Dual"));
    optCtrDual->setRadioGroupId (1);
    optCtrDual->addListener (this);

    addAndMakeVisible (optCtrCustom = new ToggleButton ("new toggle button"));
    optCtrCustom->setButtonText (TRANS("Custom"));
    optCtrCustom->setRadioGroupId (1);
    optCtrCustom->addListener (this);

    addAndMakeVisible (optBWFixed = new ToggleButton ("new toggle button"));
    optBWFixed->setButtonText (TRANS("Fixed"));
    optBWFixed->setRadioGroupId (2);
    optBWFixed->addListener (this);
    optBWFixed->setToggleState (true, dontSendNotification);

    addAndMakeVisible (optBWLinear = new ToggleButton ("new toggle button"));
    optBWLinear->setButtonText (TRANS("Linear"));
    optBWLinear->setRadioGroupId (2);
    optBWLinear->addListener (this);

    addAndMakeVisible (optBWLog = new ToggleButton ("new toggle button"));
    optBWLog->setButtonText (TRANS("Log"));
    optBWLog->setRadioGroupId (2);
    optBWLog->addListener (this);

    addAndMakeVisible (optBWGauss = new ToggleButton ("new toggle button"));
    optBWGauss->setButtonText (TRANS("Gaussian"));
    optBWGauss->setRadioGroupId (2);
    optBWGauss->addListener (this);

    addAndMakeVisible (optBWDual = new ToggleButton ("new toggle button"));
    optBWDual->setButtonText (TRANS("Dual"));
    optBWDual->setRadioGroupId (2);
    optBWDual->addListener (this);

    addAndMakeVisible (optBWCustom = new ToggleButton ("new toggle button"));
    optBWCustom->setButtonText (TRANS("Custom"));
    optBWCustom->setRadioGroupId (2);
    optBWCustom->addListener (this);

    addAndMakeVisible (sldCtrParam1 = new Slider ("new slider"));
    sldCtrParam1->setRange (10, 12000, 1);
    sldCtrParam1->setSliderStyle (Slider::LinearHorizontal);
    sldCtrParam1->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    sldCtrParam1->addListener (this);

    addAndMakeVisible (lblCtrParam1 = new Label ("new label",
                                                 TRANS("Param 1:")));
    lblCtrParam1->setFont (Font (15.00f, Font::plain));
    lblCtrParam1->setJustificationType (Justification::centredLeft);
    lblCtrParam1->setEditable (false, false, false);
    lblCtrParam1->setColour (TextEditor::textColourId, Colours::black);
    lblCtrParam1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sldCtrParam2 = new Slider ("new slider"));
    sldCtrParam2->setRange (10, 12000, 1);
    sldCtrParam2->setSliderStyle (Slider::LinearHorizontal);
    sldCtrParam2->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    sldCtrParam2->addListener (this);

    addAndMakeVisible (lblCtrParam2 = new Label ("new label",
                                                 TRANS("Param 2:")));
    lblCtrParam2->setFont (Font (15.00f, Font::plain));
    lblCtrParam2->setJustificationType (Justification::centredLeft);
    lblCtrParam2->setEditable (false, false, false);
    lblCtrParam2->setColour (TextEditor::textColourId, Colours::black);
    lblCtrParam2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sldBWParam1 = new Slider ("new slider"));
    sldBWParam1->setRange (0, 2, 0.001);
    sldBWParam1->setSliderStyle (Slider::LinearHorizontal);
    sldBWParam1->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    sldBWParam1->addListener (this);

    addAndMakeVisible (lblBWParam1 = new Label ("new label",
                                                TRANS("Param 1:")));
    lblBWParam1->setFont (Font (15.00f, Font::plain));
    lblBWParam1->setJustificationType (Justification::centredLeft);
    lblBWParam1->setEditable (false, false, false);
    lblBWParam1->setColour (TextEditor::textColourId, Colours::black);
    lblBWParam1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sldBWParam2 = new Slider ("new slider"));
    sldBWParam2->setRange (0, 2, 0.001);
    sldBWParam2->setSliderStyle (Slider::LinearHorizontal);
    sldBWParam2->setTextBoxStyle (Slider::TextBoxLeft, false, 50, 20);
    sldBWParam2->addListener (this);

    addAndMakeVisible (lblBWParam2 = new Label ("new label",
                                                TRANS("Param 2:")));
    lblBWParam2->setFont (Font (15.00f, Font::plain));
    lblBWParam2->setJustificationType (Justification::centredLeft);
    lblBWParam2->setEditable (false, false, false);
    lblBWParam2->setColour (TextEditor::textColourId, Colours::black);
    lblBWParam2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label15 = new Label ("new label",
                                            TRANS("Phase delay:")));
    label15->setFont (Font (15.00f, Font::plain));
    label15->setJustificationType (Justification::centredLeft);
    label15->setEditable (false, false, false);
    label15->setColour (TextEditor::textColourId, Colours::black);
    label15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label16 = new Label ("new label",
                                            TRANS("Group delay:")));
    label16->setFont (Font (15.00f, Font::plain));
    label16->setJustificationType (Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (TextEditor::textColourId, Colours::black);
    label16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    txtMaxChain->addListener(this);

    addAndMakeVisible (grfCenter = new ParamGraph(processor.getCore(), false));

    addAndMakeVisible (grfBW = new ParamGraph(processor.getCore(), true));

    addAndMakeVisible (grfPhaseDelay = new ResponseGraph(processor.getCore(), false));

    addAndMakeVisible (grfGroupDelay = new ResponseGraph(processor.getCore(), true));

    //Default control values setup:
    //Input
    sldInLeft->setValue(100);
    sldInRight->setValue(0);
    //Decimator/interpolator
    chkResample->setToggleState(false, sendNotification);
    chkResample->setEnabled(false);
    cbxSampleRate->setEnabled(false);
    chkDecimFilter->setEnabled(false);
    chkInterpFilter->setEnabled(false);
    grpDecimator->setEnabled(false);
    grpInterpolator->setEnabled(false);
    lblInterpolator->setEnabled(false);
    //Filter main
    txtMaxChain->setText("128");
    sldChain->setValue(128);
    //Output
    sldOutLDryL->setValue(0);
    sldOutLDryR->setValue(0);
    sldOutLWet->setValue(100);
    sldOutRDryL->setValue(100);
    sldOutRDryR->setValue(0);
    sldOutRWet->setValue(0);
    //Filter parameters
    optCtrFixed->setToggleState(true, sendNotification);
    optBWFixed->setToggleState(true, sendNotification);
    sldCtrParam1->setValue(1000);
    sldCtrParam2->setValue(1000);
    lblCtrParam2->setEnabled(false);
    sldCtrParam2->setEnabled(false);
    sldBWParam1->setValue(0.5);
    sldBWParam2->setValue(0.5);
    lblBWParam2->setEnabled(false);
    sldBWParam2->setEnabled(false);
    //Double-clicking on sliders
    sldInLeft->setDoubleClickReturnValue(true, 0.0);
    sldInRight->setDoubleClickReturnValue(true, 0.0);
    sldOutLDryL->setDoubleClickReturnValue(true, 0.0);
    sldOutLDryR->setDoubleClickReturnValue(true, 0.0);
    sldOutLWet->setDoubleClickReturnValue(true, 0.0);
    sldOutRDryL->setDoubleClickReturnValue(true, 0.0);
    sldOutRDryR->setDoubleClickReturnValue(true, 0.0);
    sldOutRWet->setDoubleClickReturnValue(true, 0.0);

    //[/UserPreSize]

    setSize (960, 600);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    grpInterpolator = nullptr;
    grpNonRealtime = nullptr;
    grpDecimator = nullptr;
    groupComponent2 = nullptr;
    groupComponent = nullptr;
    chkResample = nullptr;
    cbxSampleRate = nullptr;
    chkDecimFilter = nullptr;
    chkInterpFilter = nullptr;
    label = nullptr;
    txtMaxChain = nullptr;
    sldChain = nullptr;
    groupComponent4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    sldInLeft = nullptr;
    sldInRight = nullptr;
    label7 = nullptr;
    lblInterpolator = nullptr;
    groupComponent7 = nullptr;
    label9 = nullptr;
    label10 = nullptr;
    label11 = nullptr;
    label12 = nullptr;
    label13 = nullptr;
    sldOutLDryL = nullptr;
    sldOutLDryR = nullptr;
    sldOutLWet = nullptr;
    sldOutRDryL = nullptr;
    sldOutRDryR = nullptr;
    sldOutRWet = nullptr;
    groupComponent3 = nullptr;
    optCtrFixed = nullptr;
    optCtrLinear = nullptr;
    optCtrLog = nullptr;
    optCtrGauss = nullptr;
    optCtrDual = nullptr;
    optCtrCustom = nullptr;
    optBWFixed = nullptr;
    optBWLinear = nullptr;
    optBWLog = nullptr;
    optBWGauss = nullptr;
    optBWDual = nullptr;
    optBWCustom = nullptr;
    sldCtrParam1 = nullptr;
    lblCtrParam1 = nullptr;
    sldCtrParam2 = nullptr;
    lblCtrParam2 = nullptr;
    sldBWParam1 = nullptr;
    lblBWParam1 = nullptr;
    sldBWParam2 = nullptr;
    lblBWParam2 = nullptr;
    label15 = nullptr;
    label16 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.fillPath (internalPath1);
    g.setColour (Colours::black);
    g.strokePath (internalPath1, PathStrokeType (2.000f));

    g.setColour (Colours::black);
    g.fillPath (internalPath2);
    g.setColour (Colours::black);
    g.strokePath (internalPath2, PathStrokeType (2.000f));

    g.setColour (Colours::black);
    g.fillPath (internalPath3);
    g.setColour (Colours::black);
    g.strokePath (internalPath3, PathStrokeType (2.000f));

    g.setColour (Colours::black);
    g.fillPath (internalPath4);
    g.setColour (Colours::black);
    g.strokePath (internalPath4, PathStrokeType (2.000f));

    g.setColour (Colours::black);
    g.fillRect (92, 140, 380, 156);

    g.setColour (Colours::black);
    g.fillRect (92, 380, 380, 156);

    g.setColour (Colours::black);
    g.fillRect (492, 164, 456, 196);

    g.setColour (Colours::black);
    g.fillRect (492, 392, 456, 196);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    grpInterpolator->setBounds (688, 0, 112, 120);
    grpNonRealtime->setBounds (280, 0, 400, 120);
    grpDecimator->setBounds (160, 0, 112, 120);
    groupComponent2->setBounds (0, 360, 480, 240);
    groupComponent->setBounds (0, 120, 480, 240);
    chkResample->setBounds (168, 16, 96, 24);
    cbxSampleRate->setBounds (168, 40, 96, 24);
    chkDecimFilter->setBounds (168, 88, 96, 24);
    chkInterpFilter->setBounds (696, 88, 96, 24);
    label->setBounds (288, 16, 272, 40);
    txtMaxChain->setBounds (632, 16, 40, 24);
    sldChain->setBounds (288, 88, 384, 24);
    groupComponent4->setBounds (0, 0, 152, 120);
    label5->setBounds (8, 16, 136, 24);
    label6->setBounds (8, 64, 136, 24);
    sldInLeft->setBounds (8, 40, 136, 24);
    sldInRight->setBounds (8, 88, 136, 24);
    label7->setBounds (288, 64, 296, 24);
    lblInterpolator->setBounds (696, 16, 96, 32);
    groupComponent7->setBounds (808, 0, 152, 120);
    label9->setBounds (864, 16, 40, 24);
    label10->setBounds (904, 16, 48, 24);
    label11->setBounds (816, 40, 48, 24);
    label12->setBounds (816, 64, 48, 24);
    label13->setBounds (816, 88, 48, 24);
    sldOutLDryL->setBounds (872, 40, 24, 24);
    sldOutLDryR->setBounds (872, 64, 24, 24);
    sldOutLWet->setBounds (872, 88, 24, 24);
    sldOutRDryL->setBounds (912, 40, 24, 24);
    sldOutRDryR->setBounds (912, 64, 24, 24);
    sldOutRWet->setBounds (912, 88, 24, 24);
    groupComponent3->setBounds (480, 120, 480, 480);
    optCtrFixed->setBounds (8, 136, 80, 24);
    optCtrLinear->setBounds (8, 160, 80, 24);
    optCtrLog->setBounds (8, 184, 80, 24);
    optCtrGauss->setBounds (8, 208, 80, 24);
    optCtrDual->setBounds (8, 232, 80, 24);
    optCtrCustom->setBounds (8, 256, 80, 24);
    optBWFixed->setBounds (8, 376, 80, 24);
    optBWLinear->setBounds (8, 400, 80, 24);
    optBWLog->setBounds (8, 424, 80, 24);
    optBWGauss->setBounds (8, 448, 80, 24);
    optBWDual->setBounds (8, 472, 80, 24);
    optBWCustom->setBounds (8, 496, 80, 24);
    sldCtrParam1->setBounds (96, 304, 376, 24);
    lblCtrParam1->setBounds (8, 304, 88, 24);
    sldCtrParam2->setBounds (96, 328, 376, 24);
    lblCtrParam2->setBounds (8, 328, 88, 24);
    sldBWParam1->setBounds (96, 544, 376, 24);
    lblBWParam1->setBounds (8, 544, 88, 24);
    sldBWParam2->setBounds (96, 568, 376, 24);
    lblBWParam2->setBounds (8, 568, 88, 24);
    label15->setBounds (488, 136, 96, 24);
    label16->setBounds (488, 364, 96, 24);
    internalPath1.clear();
    internalPath1.setUsingNonZeroWinding (false);
    internalPath1.startNewSubPath (152.0f, 64.0f);
    internalPath1.lineTo (160.0f, 64.0f);
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.setUsingNonZeroWinding (false);
    internalPath2.startNewSubPath (272.0f, 64.0f);
    internalPath2.lineTo (280.0f, 64.0f);
    internalPath2.closeSubPath();

    internalPath3.clear();
    internalPath3.setUsingNonZeroWinding (false);
    internalPath3.startNewSubPath (680.0f, 64.0f);
    internalPath3.lineTo (688.0f, 64.0f);
    internalPath3.closeSubPath();

    internalPath4.clear();
    internalPath4.setUsingNonZeroWinding (false);
    internalPath4.startNewSubPath (800.0f, 64.0f);
    internalPath4.lineTo (808.0f, 64.0f);
    internalPath4.closeSubPath();

    //[UserResized] Add your own custom resize handling here..

    grfCenter->setBounds (92, 140, 380, 156);
    grfBW->setBounds (92, 380, 380, 156);
    grfPhaseDelay->setBounds (492, 164, 456, 196);
    grfGroupDelay->setBounds (492, 392, 456, 196);

    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == chkResample)
    {
        //[UserButtonCode_chkResample] -- add your button handler code here..
        bool b = chkResample->getToggleState();
        cbxSampleRate->setEnabled(b);
        chkDecimFilter->setEnabled(b);
        chkInterpFilter->setEnabled(b);
        processor.getParameters()[8]->setValueNotifyingHost(b ? 1.0f : 0.0f);
        //[/UserButtonCode_chkResample]
    }
    else if (buttonThatWasClicked == chkDecimFilter)
    {
        //[UserButtonCode_chkDecimFilter] -- add your button handler code here..
        processor.getParameters()[10]->setValueNotifyingHost(chkDecimFilter->getToggleState() ? 1.0f : 0.0f);
        //[/UserButtonCode_chkDecimFilter]
    }
    else if (buttonThatWasClicked == chkInterpFilter)
    {
        //[UserButtonCode_chkInterpFilter] -- add your button handler code here..
        processor.getParameters()[11]->setValueNotifyingHost(chkInterpFilter->getToggleState() ? 1.0f : 0.0f);
        //[/UserButtonCode_chkInterpFilter]
    }
    else if (buttonThatWasClicked == optCtrFixed)
    {
        //[UserButtonCode_optCtrFixed] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(0.0f);
        lblCtrParam1->setEnabled(true);
        sldCtrParam1->setEnabled(true);
        lblCtrParam2->setEnabled(false);
        sldCtrParam2->setEnabled(false);
        //[/UserButtonCode_optCtrFixed]
    }
    else if (buttonThatWasClicked == optCtrLinear)
    {
        //[UserButtonCode_optCtrLinear] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(0.2f);
        lblCtrParam1->setEnabled(true);
        sldCtrParam1->setEnabled(true);
        lblCtrParam2->setEnabled(true);
        sldCtrParam2->setEnabled(true);
        //[/UserButtonCode_optCtrLinear]
    }
    else if (buttonThatWasClicked == optCtrLog)
    {
        //[UserButtonCode_optCtrLog] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(0.4f);
        lblCtrParam1->setEnabled(true);
        sldCtrParam1->setEnabled(true);
        lblCtrParam2->setEnabled(true);
        sldCtrParam2->setEnabled(true);
        //[/UserButtonCode_optCtrLog]
    }
    else if (buttonThatWasClicked == optCtrGauss)
    {
        //[UserButtonCode_optCtrGauss] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(0.6f);
        lblCtrParam1->setEnabled(true);
        sldCtrParam1->setEnabled(true);
        lblCtrParam2->setEnabled(true);
        sldCtrParam2->setEnabled(true);
        //[/UserButtonCode_optCtrGauss]
    }
    else if (buttonThatWasClicked == optCtrDual)
    {
        //[UserButtonCode_optCtrDual] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(0.8f);
        lblCtrParam1->setEnabled(true);
        sldCtrParam1->setEnabled(true);
        lblCtrParam2->setEnabled(true);
        sldCtrParam2->setEnabled(true);
        //[/UserButtonCode_optCtrDual]
    }
    else if (buttonThatWasClicked == optCtrCustom)
    {
        //[UserButtonCode_optCtrCustom] -- add your button handler code here..
        processor.getParameters()[12]->setValueNotifyingHost(1.0f);
        lblCtrParam1->setEnabled(false);
        sldCtrParam1->setEnabled(false);
        lblCtrParam2->setEnabled(false);
        sldCtrParam2->setEnabled(false);
        //[/UserButtonCode_optCtrCustom]
    }
    else if (buttonThatWasClicked == optBWFixed)
    {
        //[UserButtonCode_optBWFixed] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(0.0f);
        lblBWParam1->setEnabled(true);
        sldBWParam1->setEnabled(true);
        lblBWParam2->setEnabled(false);
        sldBWParam2->setEnabled(false);
        //[/UserButtonCode_optBWFixed]
    }
    else if (buttonThatWasClicked == optBWLinear)
    {
        //[UserButtonCode_optBWLinear] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(0.2f);
        lblBWParam1->setEnabled(true);
        sldBWParam1->setEnabled(true);
        lblBWParam2->setEnabled(true);
        sldBWParam2->setEnabled(true);
        //[/UserButtonCode_optBWLinear]
    }
    else if (buttonThatWasClicked == optBWLog)
    {
        //[UserButtonCode_optBWLog] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(0.4f);
        lblBWParam1->setEnabled(true);
        sldBWParam1->setEnabled(true);
        lblBWParam2->setEnabled(true);
        sldBWParam2->setEnabled(true);
        //[/UserButtonCode_optBWLog]
    }
    else if (buttonThatWasClicked == optBWGauss)
    {
        //[UserButtonCode_optBWGauss] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(0.6f);
        lblBWParam1->setEnabled(true);
        sldBWParam1->setEnabled(true);
        lblBWParam2->setEnabled(true);
        sldBWParam2->setEnabled(true);
        //[/UserButtonCode_optBWGauss]
    }
    else if (buttonThatWasClicked == optBWDual)
    {
        //[UserButtonCode_optBWDual] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(0.8f);
        lblBWParam1->setEnabled(true);
        sldBWParam1->setEnabled(true);
        lblBWParam2->setEnabled(true);
        sldBWParam2->setEnabled(true);
        //[/UserButtonCode_optBWDual]
    }
    else if (buttonThatWasClicked == optBWCustom)
    {
        //[UserButtonCode_optBWCustom] -- add your button handler code here..
        processor.getParameters()[15]->setValueNotifyingHost(1.0f);
        lblBWParam1->setEnabled(false);
        sldBWParam1->setEnabled(false);
        lblBWParam2->setEnabled(false);
        sldBWParam2->setEnabled(false);
        //[/UserButtonCode_optBWCustom]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbxSampleRate)
    {
        //[UserComboBoxCode_cbxSampleRate] -- add your combo box handling code here..
        String text = cbxSampleRate->getText();
        int val = text.getIntValue();
        if(val <= 0) return;
        sldCtrParam1->setRange(1, val/2, 1);
        sldCtrParam1->setValue(val/12);
        sldCtrParam2->setRange(1, val/2, 1);
        sldCtrParam2->setValue(val/6);
        processor.getParameters()[9]->setValueNotifyingHost(
                processor.getParameters()[9]->getValueForText(text));
        //[/UserComboBoxCode_cbxSampleRate]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    float val = (float)sliderThatWasMoved->getValue();
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sldChain)
    {
        //[UserSliderCode_sldChain] -- add your slider handling code here..
        processor.getParameters()[18]->setValueNotifyingHost(val / (float)processor.maxchainlength);
        //[/UserSliderCode_sldChain]
    }
    else if (sliderThatWasMoved == sldInLeft)
    {
        //[UserSliderCode_sldInLeft] -- add your slider handling code here..
        processor.getParameters()[0]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldInLeft]
    }
    else if (sliderThatWasMoved == sldInRight)
    {
        //[UserSliderCode_sldInRight] -- add your slider handling code here..
        processor.getParameters()[1]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldInRight]
    }
    else if (sliderThatWasMoved == sldOutLDryL)
    {
        //[UserSliderCode_sldOutLDryL] -- add your slider handling code here..
        processor.getParameters()[2]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutLDryL]
    }
    else if (sliderThatWasMoved == sldOutLDryR)
    {
        //[UserSliderCode_sldOutLDryR] -- add your slider handling code here..
        processor.getParameters()[3]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutLDryR]
    }
    else if (sliderThatWasMoved == sldOutLWet)
    {
        //[UserSliderCode_sldOutLWet] -- add your slider handling code here..
        processor.getParameters()[4]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutLWet]
    }
    else if (sliderThatWasMoved == sldOutRDryL)
    {
        //[UserSliderCode_sldOutRDryL] -- add your slider handling code here..
        processor.getParameters()[5]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutRDryL]
    }
    else if (sliderThatWasMoved == sldOutRDryR)
    {
        //[UserSliderCode_sldOutRDryR] -- add your slider handling code here..
        processor.getParameters()[6]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutRDryR]
    }
    else if (sliderThatWasMoved == sldOutRWet)
    {
        //[UserSliderCode_sldOutRWet] -- add your slider handling code here..
        processor.getParameters()[7]->setValueNotifyingHost((val + 100.0f) * 0.005f);
        //[/UserSliderCode_sldOutRWet]
    }
    else if (sliderThatWasMoved == sldCtrParam1)
    {
        //[UserSliderCode_sldCtrParam1] -- add your slider handling code here..
        processor.getParameters()[13]->setValueNotifyingHost(
                processor.ctrHzToProportion(val));
        //[/UserSliderCode_sldCtrParam1]
    }
    else if (sliderThatWasMoved == sldCtrParam2)
    {
        //[UserSliderCode_sldCtrParam2] -- add your slider handling code here..
        processor.getParameters()[14]->setValueNotifyingHost(
                processor.ctrHzToProportion(val));
        //[/UserSliderCode_sldCtrParam2]
    }
    else if (sliderThatWasMoved == sldBWParam1)
    {
        //[UserSliderCode_sldBWParam1] -- add your slider handling code here..
        processor.getParameters()[16]->setValueNotifyingHost(
                processor.bwValToProportion(val));
        //[/UserSliderCode_sldBWParam1]
    }
    else if (sliderThatWasMoved == sldBWParam2)
    {
        //[UserSliderCode_sldBWParam2] -- add your slider handling code here..
        processor.getParameters()[17]->setValueNotifyingHost(
                processor.bwValToProportion(val));
        //[/UserSliderCode_sldBWParam2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainComponent::sliderDragStarted (Slider* sliderThatWasMoved){
    if (sliderThatWasMoved == sldChain){
        processor.getParameters()[18]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldInLeft){
        processor.getParameters()[0]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldInRight){
        processor.getParameters()[1]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutLDryL){
        processor.getParameters()[2]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutLDryR){
        processor.getParameters()[3]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutLWet){
        processor.getParameters()[4]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutRDryL){
        processor.getParameters()[5]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutRDryR){
        processor.getParameters()[6]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldOutRWet){
        processor.getParameters()[7]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldCtrParam1){
        processor.getParameters()[13]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldCtrParam2){
        processor.getParameters()[14]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldBWParam1){
        processor.getParameters()[16]->beginChangeGesture();
    }else if (sliderThatWasMoved == sldBWParam2){
        processor.getParameters()[17]->beginChangeGesture();
    }
}

void MainComponent::sliderDragEnded (Slider* sliderThatWasMoved){
    if (sliderThatWasMoved == sldChain){
        processor.getParameters()[18]->endChangeGesture();
    }else if (sliderThatWasMoved == sldInLeft){
        processor.getParameters()[0]->endChangeGesture();
    }else if (sliderThatWasMoved == sldInRight){
        processor.getParameters()[1]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutLDryL){
        processor.getParameters()[2]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutLDryR){
        processor.getParameters()[3]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutLWet){
        processor.getParameters()[4]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutRDryL){
        processor.getParameters()[5]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutRDryR){
        processor.getParameters()[6]->endChangeGesture();
    }else if (sliderThatWasMoved == sldOutRWet){
        processor.getParameters()[7]->endChangeGesture();
    }else if (sliderThatWasMoved == sldCtrParam1){
        processor.getParameters()[13]->endChangeGesture();
    }else if (sliderThatWasMoved == sldCtrParam2){
        processor.getParameters()[14]->endChangeGesture();
    }else if (sliderThatWasMoved == sldBWParam1){
        processor.getParameters()[16]->endChangeGesture();
    }else if (sliderThatWasMoved == sldBWParam2){
        processor.getParameters()[17]->endChangeGesture();
    }
}


void MainComponent::textEditorTextChanged (TextEditor& editor){
    String text = editor.getText();
    int val = text.getIntValue();
    bool turnRed = false;
    if(&editor == &*txtMaxChain){
        if(val <= 2){
            turnRed = true;
        }else{
            sldChain->setRange(1, val, 1);
            sldChain->setValue(val);
            //report value
            processor.setMaxChainLength(val);
            processor.getParameters()[18]->setValueNotifyingHost(val);
        }
    }
    if(turnRed){
        editor.setColour(TextEditor::backgroundColourId, Colours::red);
    }else{
        editor.setColour(TextEditor::backgroundColourId, Colours::white);
    }

}

void MainComponent::sampleRateChanged(){
    sldCtrParam1->setRange(processor.ctrProportionToHz(0.0f), processor.ctrProportionToHz(1.0f));
    sldCtrParam2->setRange(processor.ctrProportionToHz(0.0f), processor.ctrProportionToHz(1.0f));
    sldBWParam1->setRange(processor.bwProportionToVal(0.0f), processor.bwProportionToVal(1.0f));
    sldBWParam2->setRange(processor.bwProportionToVal(0.0f), processor.bwProportionToVal(1.0f));
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="TimeLagFilterAudioProcessor&amp; p" variableInitialisers="processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="960" initialHeight="600">
  <BACKGROUND backgroundColour="ffffffff">
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ff000000" nonZeroWinding="0">s 152 64 l 160 64 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ff000000" nonZeroWinding="0">s 272 64 l 280 64 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ff000000" nonZeroWinding="0">s 680 64 l 688 64 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ff000000" nonZeroWinding="0">s 800 64 l 808 64 x</PATH>
    <RECT pos="92 140 380 156" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="92 380 380 156" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="492 164 456 196" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="492 392 456 196" fill="solid: ff000000" hasStroke="0"/>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="362703fbe612b58e" memberName="grpInterpolator"
                  virtualName="" explicitFocusOrder="0" pos="688 0 112 120" title="Interpolator"/>
  <GROUPCOMPONENT name="new group" id="4bdd2eb745adedd1" memberName="grpNonRealtime"
                  virtualName="" explicitFocusOrder="0" pos="280 0 400 120" title="Time-Lag Filter"/>
  <GROUPCOMPONENT name="new group" id="ec7bbf26fbf84330" memberName="grpDecimator"
                  virtualName="" explicitFocusOrder="0" pos="160 0 112 120" title="Decimator"/>
  <GROUPCOMPONENT name="new group" id="d2b20422da603803" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="0 360 480 240" title="Phase Bandwidth vs. Filter Index"/>
  <GROUPCOMPONENT name="new group" id="3855c53014eae1d2" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 120 480 240" title="Center Frequency vs. Filter Index"/>
  <TOGGLEBUTTON name="new toggle button" id="8bbcfea0396f5e5a" memberName="chkResample"
                virtualName="" explicitFocusOrder="0" pos="168 16 96 24" tooltip="Reduce sample rate before time-lag filter (reduces CPU usage)."
                buttonText="Enable" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="new combo box" id="48a66593ce31ca47" memberName="cbxSampleRate"
            virtualName="" explicitFocusOrder="0" pos="168 40 96 24" tooltip="Sample rate to reduce input signal to."
            editable="0" layout="33" items="48000&#10;44100&#10;32000&#10;24000&#10;16000&#10;12000&#10;8000"
            textWhenNonSelected="48000" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="new toggle button" id="b41bc841643e02d3" memberName="chkDecimFilter"
                virtualName="" explicitFocusOrder="0" pos="168 88 96 24" tooltip="Use antialiasing filter on input before decimation (recommended, unless you want strange output)."
                buttonText="Antialiasing" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="new toggle button" id="1c16c5d49135f2da" memberName="chkInterpFilter"
                virtualName="" explicitFocusOrder="0" pos="696 88 96 24" tooltip="Use reconstruction filter after interpolation (recommended, unless you want strange output)."
                buttonText="Antialiasing" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="1"/>
  <LABEL name="new label" id="e35c0023e95d73" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="288 16 272 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Total processed (max) filter chain length:&#10;(Amount of CPU usage, not modulatable)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="cfa5cb162c62168a" memberName="txtMaxChain"
              virtualName="" explicitFocusOrder="0" pos="632 16 40 24" tooltip="The plugin is always processing all these filters, even if the output is coming from not the last one in the chain."
              outlinecol="ffb0b0b0" shadowcol="0" initialText="128" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="0" caret="1" popupmenu="0"/>
  <SLIDER name="new slider" id="66fdc3ebdf7ccf88" memberName="sldChain"
          virtualName="" explicitFocusOrder="0" pos="288 88 384 24" min="1"
          max="128" int="1" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1"/>
  <GROUPCOMPONENT name="new group" id="ac8403e9dfc1f18d" memberName="groupComponent4"
                  virtualName="" explicitFocusOrder="0" pos="0 0 152 120" title="Input"/>
  <LABEL name="new label" id="f831c3524b113737" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="8 16 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Left Send to Filter" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e03929166f9ae6d6" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="8 64 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right Send to Filter" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="3fa3471fa989a413" memberName="sldInLeft"
          virtualName="" explicitFocusOrder="0" pos="8 40 136 24" tooltip="L channel input signal to filter input. Negative is phase invert."
          min="-100" max="100" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="cb640b08e8ba4f68" memberName="sldInRight"
          virtualName="" explicitFocusOrder="0" pos="8 88 136 24" tooltip="R channel input signal to filter input. Negative is phase invert."
          min="-100" max="100" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="7dd331b7efa7fe73" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="288 64 296 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filter to take output from (modulatable):"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="911a230d3d916e50" memberName="lblInterpolator"
         virtualName="" explicitFocusOrder="0" pos="696 16 96 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Settings from&#10;decimator" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="new group" id="f5716f486260e17c" memberName="groupComponent7"
                  virtualName="" explicitFocusOrder="0" pos="808 0 152 120" title="Output"/>
  <LABEL name="new label" id="aeac2a2c7b3b51de" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="864 16 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Left:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3d84181900f5fdab" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="904 16 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6b02deeaaff3a2cb" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="816 40 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry L" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c59b59a46c5b064e" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="816 64 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry R" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4a5107ed6fcc3069" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="816 88 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="8423f84e3be47104" memberName="sldOutLDryL"
          virtualName="" explicitFocusOrder="0" pos="872 40 24 24" tooltip="L channel input to L output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="17097714d5ec074c" memberName="sldOutLDryR"
          virtualName="" explicitFocusOrder="0" pos="872 64 24 24" tooltip="R channel input to L output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="964f09a4970affd2" memberName="sldOutLWet"
          virtualName="" explicitFocusOrder="0" pos="872 88 24 24" tooltip="Time-lag filter to L output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="f9353e2746ccbf92" memberName="sldOutRDryL"
          virtualName="" explicitFocusOrder="0" pos="912 40 24 24" tooltip="L channel input to R output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="d968c56f1d170061" memberName="sldOutRDryR"
          virtualName="" explicitFocusOrder="0" pos="912 64 24 24" tooltip="R channel input to R output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="40f85a9764e32010" memberName="sldOutRWet"
          virtualName="" explicitFocusOrder="0" pos="912 88 24 24" tooltip="Time-lag filter to R output. Negative is phase invert."
          min="-100" max="100" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <GROUPCOMPONENT name="new group" id="3fbb2067ae4b5760" memberName="groupComponent3"
                  virtualName="" explicitFocusOrder="0" pos="480 120 480 480" title="Phase Response"/>
  <TOGGLEBUTTON name="new toggle button" id="fa587db1247381a0" memberName="optCtrFixed"
                virtualName="" explicitFocusOrder="0" pos="8 136 80 24" buttonText="Fixed"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="1"/>
  <TOGGLEBUTTON name="new toggle button" id="83f629907b155d35" memberName="optCtrLinear"
                virtualName="" explicitFocusOrder="0" pos="8 160 80 24" buttonText="Linear"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="c87af787de2fb449" memberName="optCtrLog"
                virtualName="" explicitFocusOrder="0" pos="8 184 80 24" buttonText="Log"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="6f0a5e8e21961e6e" memberName="optCtrGauss"
                virtualName="" explicitFocusOrder="0" pos="8 208 80 24" buttonText="Gaussian"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="ec56dac40133eab0" memberName="optCtrDual"
                virtualName="" explicitFocusOrder="0" pos="8 232 80 24" buttonText="Dual"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="2f6619f0962537c6" memberName="optCtrCustom"
                virtualName="" explicitFocusOrder="0" pos="8 256 80 24" buttonText="Custom"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="824b1c96adf82f72" memberName="optBWFixed"
                virtualName="" explicitFocusOrder="0" pos="8 376 80 24" buttonText="Fixed"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="1"/>
  <TOGGLEBUTTON name="new toggle button" id="2a0b1b0368b594f3" memberName="optBWLinear"
                virtualName="" explicitFocusOrder="0" pos="8 400 80 24" buttonText="Linear"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="fce4f3e1eb8d024b" memberName="optBWLog"
                virtualName="" explicitFocusOrder="0" pos="8 424 80 24" buttonText="Log"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="61584a22ef1c27f7" memberName="optBWGauss"
                virtualName="" explicitFocusOrder="0" pos="8 448 80 24" buttonText="Gaussian"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="285ca127ad82eb01" memberName="optBWDual"
                virtualName="" explicitFocusOrder="0" pos="8 472 80 24" buttonText="Dual"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="1c92ce20e47e56dc" memberName="optBWCustom"
                virtualName="" explicitFocusOrder="0" pos="8 496 80 24" buttonText="Custom"
                connectedEdges="0" needsCallback="1" radioGroupId="2" state="0"/>
  <SLIDER name="new slider" id="ae4b96ce9e18c773" memberName="sldCtrParam1"
          virtualName="" explicitFocusOrder="0" pos="96 304 376 24" min="10"
          max="12000" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ce43b9e108a92afa" memberName="lblCtrParam1"
         virtualName="" explicitFocusOrder="0" pos="8 304 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Param 1:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="6e75538ae6df717e" memberName="sldCtrParam2"
          virtualName="" explicitFocusOrder="0" pos="96 328 376 24" min="10"
          max="12000" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="e3fa65688502773c" memberName="lblCtrParam2"
         virtualName="" explicitFocusOrder="0" pos="8 328 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Param 2:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="6ce8314f44a98317" memberName="sldBWParam1"
          virtualName="" explicitFocusOrder="0" pos="96 544 376 24" min="0"
          max="2" int="0.001" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="4be1f7b9fc39937b" memberName="lblBWParam1"
         virtualName="" explicitFocusOrder="0" pos="8 544 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Param 1:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="f81a125a92ac5704" memberName="sldBWParam2"
          virtualName="" explicitFocusOrder="0" pos="96 568 376 24" min="0"
          max="2" int="0.001" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="54600ae82b052f31" memberName="lblBWParam2"
         virtualName="" explicitFocusOrder="0" pos="8 568 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Param 2:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="99a48b4ce5724a2c" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="488 136 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Phase delay:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b85642bf644d2649" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="488 364 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Group delay:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
