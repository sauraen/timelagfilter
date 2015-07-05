/*
  ==============================================================================

    ParamGraph.h
    Created: 13 May 2015 3:11:42pm
    Author:  Sauraen

  ==============================================================================
*/

#ifndef PARAMGRAPH_H_INCLUDED
#define PARAMGRAPH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimeLagFilterCore.h"

//==============================================================================
/*
*/
class ParamGraph    : public Component, public Timer
{
public:
    ParamGraph(TimeLagFilterCore& c, bool doBW);
    ~ParamGraph();

    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    
    static float getScaleDivisions(float max, int abouthowmany);
private:
    TimeLagFilterCore& core;
    bool mode;
    
    static const int axisspacing;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamGraph)
};


#endif  // PARAMGRAPH_H_INCLUDED
