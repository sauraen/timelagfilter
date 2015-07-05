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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainComponent.h"
#include "TimeLagFilterCore.h"


//==============================================================================
TLFParameter::TLFParameter(TimeLagFilterAudioProcessor& processor_, int paramIndex_) 
        : processor(processor_), paramIndex(paramIndex_) {
    switch(paramIndex){
        case 0: 
            name = "L In to Filter";
            defaultValue = 1.0f;
            break;
        case 1: 
            name = "R In to Filter";
            defaultValue = 0.5f;
            break;
        case 2: 
            name = "L In to L Out";
            defaultValue = 0.5f;
            break;
        case 3: 
            name = "R In to L Out";
            defaultValue = 0.5f;
            break;
        case 4: 
            name = "Wet to L Out";
            defaultValue = 1.0f;
            break;
        case 5: 
            name = "L In to R Out";
            defaultValue = 1.0f;
            break;
        case 6: 
            name = "R In to R Out";
            defaultValue = 0.5f;
            break;
        case 7: 
            name = "Wet to R Out";
            defaultValue = 0.5f;
            break;
        case 8: 
            name = "Enable Resampling";
            defaultValue = 0.0f;
            break;
        case 9: 
            name = "Resample Rate";
            defaultValue = 1.0f;
            break;
        case 10: 
            name = "Decimator Filter";
            defaultValue = 1.0f;
            break;
        case 11: 
            name = "Interpolator Filter";
            defaultValue = 1.0f;
            break;
        case 12:
             name = "CtrModType";
            defaultValue = 0.0f;
            break;
        case 13: 
            name = "CtrParam1";
            defaultValue = processor.ctrHzToProportion(1000.0f);
            break;
        case 14: 
            name = "CtrParam2";
            defaultValue = processor.ctrHzToProportion(1000.0f);
            break;
        case 15:
             name = "BWModType";
            defaultValue = 0.0f;
            break;
        case 16: 
            name = "BWParam1";
            defaultValue = processor.bwValToProportion(0.5f);
            break;
        case 17: 
            name = "BWParam2";
            defaultValue = processor.bwValToProportion(0.5f);
            break;
        case 18: 
            name = "Chain Length";
            defaultValue = 1.0f;
            break;
        default: 
            name = "Error";
            defaultValue = 1.0f;
    }
    if(paramIndex <= 7){
        label = "%";
    }else if(paramIndex == 9 || paramIndex == 13 || paramIndex == 14){
        label = "Hz";
    }else{
        label = "";
    }
    if(paramIndex == 8 || paramIndex == 10 || paramIndex == 11){
        numSteps = 2;
    }else if(paramIndex == 12 || paramIndex == 15){
        numSteps = 6;
    }else if(paramIndex == 9){
        numSteps = 7;
    }else{
        numSteps = 0x7FFFFFFF;
    }
}
TLFParameter::~TLFParameter() {}

float TLFParameter::getValue() const {
    return processor.getInternalParameter(paramIndex);
}
void TLFParameter::setValue(float newValue){
    processor.setInternalParameter(paramIndex, newValue);
}

bool TLFParameter::isAutomatable() const {
    return (paramIndex != 8 && paramIndex != 9 && paramIndex != 12 && paramIndex != 15);
}

    
String TLFParameter::getText(float value, int len) const {
    String str; int i;
    switch(paramIndex){
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        str = String((int)((value * 200.0f) - 100.0f));
        break;
    case 8:
    case 10:
    case 11:
        str = (value >= 0.5f) ? "Yes" : "No";
        break;
    case 12:
    case 15:
        i = (int)((value * 5.0f) + 0.5f);
        switch(i){
            case 0: str = "Fixed"; break;
            case 1: str = "Linear"; break;
            case 2: str = "Log"; break;
            case 3: str = "Gaussian"; break;
            case 4: str = "Dual"; break;
            case 5: str = "Custom"; break;
            default: str = "Error";
        }
        break;
    case 13:
    case 14:
        str = String((int)processor.ctrProportionToHz(value));
        break;
    case 16:
    case 17:
        str = String(processor.bwProportionToVal(value), 3);
        break;
    case 9:
        i = (int)((value * 6.0f) + 0.5f);
        switch(i){
            case 0: str =  "8000"; break;
            case 1: str = "12000"; break;
            case 2: str = "16000"; break;
            case 3: str = "24000"; break;
            case 4: str = "32000"; break;
            case 5: str = "44100"; break;
            case 6: str = "48000"; break;
            default:str = "Error";
        }
        break;
    case 18:
        str = String((int)(value * processor.maxchainlength));
    }
    return str.substring(0, len);
}
float TLFParameter::getValueForText(const String& text) const {
    String t = text.toLowerCase();
    if(t.endsWith("%")){
        t = t.substring(0, t.length() - 1);
    }else if(t.endsWith("hz")){
        t = t.substring(0, t.length() - 2);
    }
    float f = t.getFloatValue();
    switch(paramIndex){
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return (f - 100.0f) * 0.005f;
    case 8:
    case 10:
    case 11:
        if(t == "1" || t == "y" || t == "yes" || t == "on" || t == "true" || t == "enabled"){
            return 1.0f;
        }
        return 0.0f;
    case 12:
    case 15:
        if(t == "fixed") return 0.0f;
        if(t == "linear") return 0.2f;
        if(t == "log") return 0.4f;
        if(t == "gaussian") return 0.6f;
        if(t == "dual") return 0.8f;
        if(t == "custom") return 1.0f;
        break;
    case 13:
    case 14:
        return processor.ctrHzToProportion(f);
    case 16:
    case 17:
        return processor.bwValToProportion(f);
    case 9:
        if(t ==  "8000" || t ==  "8k") return 0.0f / 6.0f;
        if(t == "12000" || t == "12k") return 1.0f / 6.0f;
        if(t == "16000" || t == "16k") return 2.0f / 6.0f;
        if(t == "24000" || t == "24k") return 3.0f / 6.0f;
        if(t == "32000" || t == "32k") return 4.0f / 6.0f;
        if(t == "44100" || t == "44.1k") return 5.0f / 6.0f;
        if(t == "48000" || t == "48k") return 6.0f / 6.0f;
        break;
    case 18:
        return (float)(int)f / processor.maxchainlength;
    }
    jassertfalse;
    return 0.0f;
}



//==============================================================================
TimeLagFilterAudioProcessor::TimeLagFilterAudioProcessor() {
    editor = nullptr;
    core_samplerate = 47999.0f;
    lintofilter = 1.0f;
    rintofilter = 0.0f;
    lintolout = 0.0f;
    rintolout = 0.0f;
    wettolout = 1.0f;
    lintorout = 1.0f;
    rintorout = 0.0f;
    wettorout = 0.0f;
    resamp = false;
    resamp_rate = 48000.0f;
    decimfilter = true;
    interpfilter = true;
    ctr_modtype = 0;
    ctr_param1 = 1000.0f;
    ctr_param2 = 1000.0f;
    bw_modtype = 0;
    bw_param1 = 0.5f;
    bw_param2 = 0.5f;
    chainlength = 128;
    maxchainlength = 128;
    
    //Set up parameters
    for(int i=0; i<19; i++){
        addParameter(new TLFParameter(*this, i));
    }
}

TimeLagFilterAudioProcessor::~TimeLagFilterAudioProcessor() {
    
}

//==============================================================================
const String TimeLagFilterAudioProcessor::getName() const {
    return JucePlugin_Name;
}

float TimeLagFilterAudioProcessor::getInternalParameter (int index) {
    int i;
    switch(index){
        case 0: return (lintofilter + 1.0f) * 0.5f;
        case 1: return (rintofilter + 1.0f) * 0.5f;
        case 2: return (lintolout + 1.0f) * 0.5f;
        case 3: return (rintolout + 1.0f) * 0.5f;
        case 4: return (wettolout + 1.0f) * 0.5f;
        case 5: return (lintorout + 1.0f) * 0.5f;
        case 6: return (rintorout + 1.0f) * 0.5f;
        case 7: return (wettorout + 1.0f) * 0.5f;
        case 8: return resamp ? 1.0f : 0.0f;
        case 9:
            i = (int)resamp_rate;
            if(i == 48000) return 6.0f / 6.0f;
            if(i == 44100) return 5.0f / 6.0f;
            if(i == 32000) return 4.0f / 6.0f;
            if(i == 24100) return 3.0f / 6.0f;
            if(i == 16100) return 2.0f / 6.0f;
            if(i == 12100) return 1.0f / 6.0f;
            if(i ==  8000) return 0.0f / 6.0f;
            return 0.0f;
        case 10: return decimfilter ? 1.0f : 0.0f;
        case 11: return interpfilter ? 1.0f : 0.0f;
        case 12: return (float)ctr_modtype * 0.2f;
        case 13: return ctrHzToProportion(ctr_param1);
        case 14: return ctrHzToProportion(ctr_param2);
        case 15: return (float)bw_modtype * 0.2f;
        case 16: return bwValToProportion(bw_param1);
        case 17: return bwValToProportion(bw_param2);
        case 18: return (float)chainlength / (float)maxchainlength;
        default: return 0.0f;
    }
}

void TimeLagFilterAudioProcessor::setInternalParameter (int index, float newValue) {
    if(newValue < 0.0f) newValue = 0.0f;
    if(newValue > 1.0f) newValue = 1.0f;
    int v = (int)newValue;
    float x = (newValue * 2.0f) - 1.0f;
    switch(index){
        case 0:
            //lintofilter
            lintofilter = x;
            break;
        case 1:
            //rintofilter
            rintofilter = x;
            break;
        case 2:
            //lintolout
            lintolout = x;
            break;
        case 3:
            //rintolout
            rintolout = x;
            break;
        case 4:
            //wettolout
            wettolout = x;
            break;
        case 5:
            //lintorout
            lintorout = x;
            break;
        case 6:
            //rintorout
            rintorout = x;
            break;
        case 7:
            //wettorout
            wettorout = x;
            break;
        case 8:
            //resamp
            resamp = (newValue >= 0.5f);
            core.doResample(resamp);
            break;
        case 9:
            //resamp_rate
            x = (newValue * 6.0f) + 0.5f;
            v = (int)x;
            switch(v){
                case 0: resamp_rate =  8000.0f; break;
                case 1: resamp_rate = 12000.0f; break;
                case 2: resamp_rate = 16000.0f; break;
                case 3: resamp_rate = 24000.0f; break;
                case 4: resamp_rate = 32000.0f; break;
                case 5: resamp_rate = 44100.0f; break;
                default:resamp_rate = 48000.0f;
            }
            core.setResampleRate(resamp_rate);
            break;
        case 10:
            //decimfilter
            decimfilter = (newValue >= 0.5f);
            core.doDecimFilter(decimfilter);
            break;
        case 11:
            //interpfilter
            interpfilter = (newValue >= 0.5f);
            core.doInterpFilter(interpfilter);
            break;
        case 12:
            //ctr_modtype
            ctr_modtype = (int)((newValue * 5.0f) + 0.5f);
            core.setCtrModType(ctr_modtype);
            break;
        case 13:
            //ctr_param1
            ctr_param1 = ctrProportionToHz(newValue);
            core.setCtrParam1(ctr_param1);
            break;
        case 14:
            //ctr_param2
            ctr_param2 = ctrProportionToHz(newValue);
            core.setCtrParam2(ctr_param2);
            break;
        case 15:
            //bw_modtype
            bw_modtype = (int)((newValue * 5.0f) + 0.5f);
            core.setBWModType(bw_modtype);
            break;
        case 16:
            //bw_param1
            bw_param1 = bwProportionToVal(newValue);
            core.setBWParam1(bw_param1);
            break;
        case 17:
            //bw_param2
            bw_param2 = bwProportionToVal(newValue);
            core.setBWParam2(bw_param2);
            break;
        case 18:
            //chainlength
            chainlength = (int)(newValue * (float)maxchainlength);
    }
}

float TimeLagFilterAudioProcessor::ctrHzToProportion(float hz){
    return (hz - core.getMinCtr()) / (core.getMaxCtr() - core.getMinCtr());
}
float TimeLagFilterAudioProcessor::ctrProportionToHz(float proportion){
    return (proportion * (core.getMaxCtr() - core.getMinCtr())) + core.getMinCtr();
}
float TimeLagFilterAudioProcessor::bwValToProportion(float val){
    return (val - core.getMinBW()) / (core.getMaxBW() - core.getMinBW());
}
float TimeLagFilterAudioProcessor::bwProportionToVal(float proportion){
    return (proportion * (core.getMaxBW() - core.getMinBW())) + core.getMinBW();
}


const String TimeLagFilterAudioProcessor::getInputChannelName (int channelIndex) const {
    switch(channelIndex){
        case 0: return "Left";
        case 1: return "Right";
        default: return "Error";
    }
}

const String TimeLagFilterAudioProcessor::getOutputChannelName (int channelIndex) const {
    switch(channelIndex){
        case 0: return "Left";
        case 1: return "Right";
        default: return "Error";
    }
}

bool TimeLagFilterAudioProcessor::isInputChannelStereoPair (int index) const {
    return true;
}

bool TimeLagFilterAudioProcessor::isOutputChannelStereoPair (int index) const {
    return true;
}

bool TimeLagFilterAudioProcessor::acceptsMidi() const {
   return false;
}

bool TimeLagFilterAudioProcessor::producesMidi() const {
    return false;
}

bool TimeLagFilterAudioProcessor::silenceInProducesSilenceOut() const {
    return false;
}

double TimeLagFilterAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int TimeLagFilterAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TimeLagFilterAudioProcessor::getCurrentProgram() {
    return 0;
}

void TimeLagFilterAudioProcessor::setCurrentProgram (int index) {
}

const String TimeLagFilterAudioProcessor::getProgramName (int index) {
    return "There are no programs";
}

void TimeLagFilterAudioProcessor::changeProgramName (int index, const String& newName) {
}

//==============================================================================


TimeLagFilterCore& TimeLagFilterAudioProcessor::getCore(){
    return core;
}

void TimeLagFilterAudioProcessor::setMaxChainLength(int mcl){
    maxchainlength = mcl;
    core.setup(maxchainlength, core_samplerate);
}

void TimeLagFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    if((float)sampleRate != core_samplerate && editor != nullptr){
        editor->mainComponent->sampleRateChanged();
    }
    core_samplerate = (float)sampleRate;
    core.setup(maxchainlength, core_samplerate);
    core.preparePlayback();
}

void TimeLagFilterAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TimeLagFilterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    //Disable denormalized floats
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    
    //Clears output channels that don't have input channels
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(getNumInputChannels() != 2) return;
    
    float* leftbuf = buffer.getWritePointer(0);
    float* rightbuf = buffer.getWritePointer(1);
    
    float left, right, win, wet, lout, rout;
    for(int s=0; s<buffer.getNumSamples(); ++s){
        left = leftbuf[s];
        right = rightbuf[s];
        win = (left*lintofilter)+(right*rintofilter);
        //if(abs(win) < 1e-30f) win = 0.0f; //Suppress denormalization
        wet = core.sample(win, chainlength-1);
        lout = (lintolout*left)+(rintolout*right)+(wettolout*wet);
        rout = (lintorout*left)+(rintorout*right)+(wettorout*wet);
        if(lout > 1.0f) lout = 1.0f;
        if(lout < -1.0f) lout = -1.0f;
        if(rout > 1.0f) rout = 1.0f;
        if(rout < -1.0f) rout = -1.0f;
        leftbuf[s] = lout;
        rightbuf[s] = rout;
    }
    
}

//==============================================================================
bool TimeLagFilterAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TimeLagFilterAudioProcessor::createEditor() {
    editor = new TimeLagFilterAudioProcessorEditor (*this);
    return editor;
}

//==============================================================================
void TimeLagFilterAudioProcessor::getStateInformation (MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TimeLagFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new TimeLagFilterAudioProcessor();
}
