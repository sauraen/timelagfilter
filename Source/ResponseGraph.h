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

#ifndef RESPONSEGRAPH_H_INCLUDED
#define RESPONSEGRAPH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimeLagFilterCore.h"

//==============================================================================
/*
*/
class ResponseGraph    : public Component, public Timer
{
public:
    ResponseGraph(TimeLagFilterCore& c, bool groupDly);
    ~ResponseGraph();

    void paint (Graphics&);
    void resized();
    
    void timerCallback() override;
    
    static const int NUM_RESP_W;
private:
    TimeLagFilterCore& core;
    bool mode;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResponseGraph)
};


#endif  // RESPONSEGRAPH_H_INCLUDED
