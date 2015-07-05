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

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class TimeLagFilterAudioProcessor;
class MainComponent;


//==============================================================================
/**
*/
class TimeLagFilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TimeLagFilterAudioProcessorEditor (TimeLagFilterAudioProcessor&);
    ~TimeLagFilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    ScopedPointer<MainComponent> mainComponent;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TimeLagFilterAudioProcessor& processor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeLagFilterAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
