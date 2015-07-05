/*
  ==============================================================================

    ResponseGraph.h
    Created: 15 May 2015 11:37:52am
    Author:  Sauraen

  ==============================================================================
*/

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
