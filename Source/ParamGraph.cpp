/*
  ==============================================================================

    ParamGraph.cpp
    Created: 13 May 2015 3:11:42pm
    Author:  Sauraen

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimeLagFilterCore.h"
#include "ParamGraph.h"

//==============================================================================
const int ParamGraph::axisspacing = 14;

ParamGraph::ParamGraph(TimeLagFilterCore& c, bool doBW) : core(c){
    mode = doBW;
    startTimerHz(10);
}

ParamGraph::~ParamGraph(){
    
}

void ParamGraph::paint (Graphics& g){
    //Clear the background
    g.fillAll (Colours::black);
    Rectangle<int> bounds = getLocalBounds();

    //Draw axes
    g.setColour(Colours::white);
    g.drawLine(0, bounds.getHeight() - axisspacing, bounds.getWidth(), bounds.getHeight() - axisspacing);
    g.drawLine(axisspacing, 0, axisspacing, bounds.getHeight());
    g.setFont(axisspacing);
    g.drawSingleLineText("0", 3, bounds.getHeight() - 2);
    
    float n = (float)core.getNumFilters();
    float lastval, thisval;
    lastval = mode ? core.getBWValue(0) : core.getCtrValue(0);
    float maxy = mode ? core.getMaxBW() : core.getMaxCtr();
    
    //Scale
    float axisstep = getScaleDivisions(n, 10);
    int x;
    for(float i=axisstep; i<n; i+=axisstep){
        x = axisspacing + ((bounds.getWidth() - axisspacing) * (i-1.0f) / (n-1.0f));
        g.drawSingleLineText(String(i), x - (4 * (int)log10(i)) - 2, bounds.getHeight() - 2);
        g.drawLine(x, bounds.getHeight() - axisspacing + 1, x, bounds.getHeight() - axisspacing - 2);
    }
    
    //Data
    for(int i=1; i<n; ++i){
        thisval = mode ? core.getBWValue(i) : core.getCtrValue(i);
        g.drawLine(axisspacing + ((bounds.getWidth() - axisspacing) * (i-1) / (n-1)),
                   bounds.getHeight() - (axisspacing + ((bounds.getHeight() - axisspacing) * lastval / maxy)),
                   axisspacing + ((bounds.getWidth() - axisspacing) * i / (n-1)),
                   bounds.getHeight() - (axisspacing + ((bounds.getHeight() - axisspacing) * thisval / maxy)));
        lastval = thisval;
    }
}

void ParamGraph::resized(){
    
}


void ParamGraph::timerCallback(){
    repaint();
}

void ParamGraph::mouseDown(const MouseEvent& event){
    mouseDrag(event);
}
void ParamGraph::mouseDrag(const MouseEvent& event){
    if((mode ? core.getBWModType() : core.getCtrModType()) != 5) return;
    Point<int> pos = event.getPosition();
    Rectangle<int> bounds = getLocalBounds();
    int filter = (pos.x - axisspacing) * core.getNumFilters() / (bounds.getWidth() - axisspacing);
    if(filter < 0 || filter >= core.getNumFilters()) return;
    float maxy = mode ? core.getMaxBW() : core.getMaxCtr();
    float yval = maxy - ((float)pos.y * maxy / (float)(bounds.getHeight() - axisspacing));
    if(yval != (mode ? core.getBWValue(filter) : core.getCtrValue(filter))){
        mode ? core.setBWValue(filter, yval) : core.setCtrValue(filter, yval);
    }
}

float ParamGraph::getScaleDivisions(float max, int abouthowmany){
    //Gives a scale value with most-significant decimal digit 1, 2, or 5 such that
    //max / return ~= abouthowmany.
    //
    if(max == 0.0f) return 10.0f;
    float sign = 1.0f;
    if(max < 0.0f){
        max = 0.0f - max;
        sign = -1.0f;
    }
    if(abouthowmany <= 0) return 10.0f;
    //
    float ret = max / (float)abouthowmany;
    float base = floor(log10(ret));
    float power = pow(10.0f, base);
    ret /= power;
    if(ret < 1.5f) ret = 1.0f;
    else if(ret < 3.5f) ret = 2.0f;
    else if(ret < 7.5f) ret = 5.0f;
    else ret = 10.0f;
    ret *= power;
    return ret * sign;
}

