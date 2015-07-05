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

#ifndef __JUCE_HEADER_9002020A4DD09B20__
#define __JUCE_HEADER_9002020A4DD09B20__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "ParamGraph.h"
#include "ResponseGraph.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public Component,
                       public TextEditor::Listener,
                       public ButtonListener,
                       public ComboBoxListener,
                       public SliderListener
{
public:
    //==============================================================================
    MainComponent (TimeLagFilterAudioProcessor& p);
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void textEditorTextChanged (TextEditor& editor);

    void sampleRateChanged();

    void sliderDragStarted (Slider* sliderThatWasMoved);
    void sliderDragEnded (Slider* sliderThatWasMoved);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    TimeLagFilterAudioProcessor& processor;

    ScopedPointer<ParamGraph> grfCenter;
    ScopedPointer<ParamGraph> grfBW;
    ScopedPointer<ResponseGraph> grfPhaseDelay;
    ScopedPointer<ResponseGraph> grfGroupDelay;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> grpInterpolator;
    ScopedPointer<GroupComponent> grpNonRealtime;
    ScopedPointer<GroupComponent> grpDecimator;
    ScopedPointer<GroupComponent> groupComponent2;
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<ToggleButton> chkResample;
    ScopedPointer<ComboBox> cbxSampleRate;
    ScopedPointer<ToggleButton> chkDecimFilter;
    ScopedPointer<ToggleButton> chkInterpFilter;
    ScopedPointer<Label> label;
    ScopedPointer<TextEditor> txtMaxChain;
    ScopedPointer<Slider> sldChain;
    ScopedPointer<GroupComponent> groupComponent4;
    ScopedPointer<Label> label5;
    ScopedPointer<Label> label6;
    ScopedPointer<Slider> sldInLeft;
    ScopedPointer<Slider> sldInRight;
    ScopedPointer<Label> label7;
    ScopedPointer<Label> lblInterpolator;
    ScopedPointer<GroupComponent> groupComponent7;
    ScopedPointer<Label> label9;
    ScopedPointer<Label> label10;
    ScopedPointer<Label> label11;
    ScopedPointer<Label> label12;
    ScopedPointer<Label> label13;
    ScopedPointer<Slider> sldOutLDryL;
    ScopedPointer<Slider> sldOutLDryR;
    ScopedPointer<Slider> sldOutLWet;
    ScopedPointer<Slider> sldOutRDryL;
    ScopedPointer<Slider> sldOutRDryR;
    ScopedPointer<Slider> sldOutRWet;
    ScopedPointer<GroupComponent> groupComponent3;
    ScopedPointer<ToggleButton> optCtrFixed;
    ScopedPointer<ToggleButton> optCtrLinear;
    ScopedPointer<ToggleButton> optCtrLog;
    ScopedPointer<ToggleButton> optCtrGauss;
    ScopedPointer<ToggleButton> optCtrDual;
    ScopedPointer<ToggleButton> optCtrCustom;
    ScopedPointer<ToggleButton> optBWFixed;
    ScopedPointer<ToggleButton> optBWLinear;
    ScopedPointer<ToggleButton> optBWLog;
    ScopedPointer<ToggleButton> optBWGauss;
    ScopedPointer<ToggleButton> optBWDual;
    ScopedPointer<ToggleButton> optBWCustom;
    ScopedPointer<Slider> sldCtrParam1;
    ScopedPointer<Label> lblCtrParam1;
    ScopedPointer<Slider> sldCtrParam2;
    ScopedPointer<Label> lblCtrParam2;
    ScopedPointer<Slider> sldBWParam1;
    ScopedPointer<Label> lblBWParam1;
    ScopedPointer<Slider> sldBWParam2;
    ScopedPointer<Label> lblBWParam2;
    ScopedPointer<Label> label15;
    ScopedPointer<Label> label16;
    Path internalPath1;
    Path internalPath2;
    Path internalPath3;
    Path internalPath4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9002020A4DD09B20__
