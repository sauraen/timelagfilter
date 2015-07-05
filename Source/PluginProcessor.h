/*******************************************************************************
timelagfilter: Open-source VST3 (x86/x64) plugin that uses phase effects to 
    mangle audio in the time domain
Copyright (C) 2015 Sauraen, sauraen@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "JuceHeader.h"
#include "TimeLagFilterCore.h"


class TimeLagFilterAudioProcessorEditor;
class TimeLagFilterAudioProcessor;
class MainComponent;

class TLFParameter : public AudioProcessorParameter {
public:
    TLFParameter(TimeLagFilterAudioProcessor& processor_, int paramIndex_);
    ~TLFParameter();

    float getValue() const override;
    void setValue(float newValue) override;
    inline float getDefaultValue() const override {return defaultValue;}
    inline int getNumSteps() const override {return numSteps;}
    
    inline String getName(int maximumStringLength) const override {return name.substring(0, maximumStringLength);}
    inline String getLabel() const override {return label;}
    String getText(float value, int len) const override;
    float getValueForText(const String& text) const override;
    
    bool isAutomatable() const override;
    inline bool isOrientationInverted() const override {return false;}
    inline bool isMetaParameter() const override {return false;}
    
private:
    TimeLagFilterAudioProcessor& processor;
    int paramIndex;

    float defaultValue;
    int numSteps;
    
    String name, label;
};

//==============================================================================
/**
*/
class TimeLagFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TimeLagFilterAudioProcessor();
    ~TimeLagFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    float getInternalParameter (int index);
    void setInternalParameter (int index, float newValue);
    
    float ctrHzToProportion(float hz);
    float ctrProportionToHz(float proportion);
    float bwValToProportion(float val);
    float bwProportionToVal(float proportion);

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void setMaxChainLength(int mcl);
    
    TimeLagFilterCore& getCore();
    
    float core_samplerate;
    
    float lintofilter, rintofilter;
    float lintolout, rintolout, wettolout;
    float lintorout, rintorout, wettorout;
    bool resamp, decimfilter, interpfilter;
    float resamp_rate;
    int ctr_modtype, bw_modtype;
    float ctr_param1, ctr_param2;
    float bw_param1, bw_param2;
    int maxchainlength, chainlength;

private:
    TimeLagFilterCore core;
    
    TimeLagFilterAudioProcessorEditor* editor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeLagFilterAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
