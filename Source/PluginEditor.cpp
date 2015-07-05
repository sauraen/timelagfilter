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


//==============================================================================
TimeLagFilterAudioProcessorEditor::TimeLagFilterAudioProcessorEditor (TimeLagFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p){
    
    addAndMakeVisible(mainComponent = new MainComponent(processor));
    mainComponent->setBounds(0, 0, 960, 600);
    
    setSize (960, 600);
}

TimeLagFilterAudioProcessorEditor::~TimeLagFilterAudioProcessorEditor(){
    
}

//==============================================================================
void TimeLagFilterAudioProcessorEditor::paint (Graphics& g){
    
}

void TimeLagFilterAudioProcessorEditor::resized(){
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
