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

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimeLagFilterCore.h"
#include "ParamGraph.h"
#include "ResponseGraph.h"

//==============================================================================
const int ResponseGraph::NUM_RESP_W = 200;

ResponseGraph::ResponseGraph(TimeLagFilterCore& c, bool groupDelay) : core(c){
    mode = groupDelay;
    startTimerHz(10);
}

ResponseGraph::~ResponseGraph(){
    
}

void ResponseGraph::paint (Graphics& g){
    //Clear the background
    g.fillAll (Colours::black);
    Rectangle<int> bounds = getLocalBounds();
    const int spacingabove = 14, spacingleft = 56;

    //Draw axes
    g.setColour(Colours::white);
    if(mode){
        g.drawLine(0, bounds.getHeight() - spacingabove, bounds.getWidth(), bounds.getHeight() - spacingabove);
    }else{
        g.drawLine(0, spacingabove, bounds.getWidth(), spacingabove);
    }
    g.drawLine(spacingleft, 0, spacingleft, bounds.getHeight());
    g.setFont(14.0f);
    
    
    //Scale X
    float maxx = core.getMaxCtr(); //reduced_fs / 2
    int axisstep = (int)ParamGraph::getScaleDivisions(maxx*0.001f, 13);
    if(axisstep <= 0) axisstep = 1;
    int x;
    for(int i=axisstep; i*1000<maxx; i+=axisstep){
        x = spacingleft + ((bounds.getWidth() - spacingleft) * i * 1000 / (int)maxx);
        if(mode){
            g.drawSingleLineText(String(i) + "k", x - (4 * (int)log10(i)) - 8, bounds.getHeight() - 2);
            g.drawLine(x, bounds.getHeight() - spacingabove + 1, x, bounds.getHeight() - spacingabove - 2);
        }else{
            g.drawSingleLineText(String(i) + "k", x - (4 * (int)log10(i)) - 8, spacingabove - 2);
            g.drawLine(x, spacingabove - 1, x, spacingabove + 2);
        }
    }
    
    //Scale Y
    float maxy = mode ? core.getMaxGroupDelay() : core.getNumFilters();
    /*
    if(mode){
        g.drawSingleLineText(String(maxy), 0, spacingabove - 2);
    }else{
        g.drawSingleLineText("2pi\u00D7" + String(core.getNumFilters()), 0, bounds.getHeight() - 2);
    }
    */
    float ystep = ParamGraph::getScaleDivisions(maxy, 5);
    int ybase; float xscale, yscale;
    ybase = mode ? bounds.getHeight() - spacingabove : spacingabove;
    yscale = (spacingabove - bounds.getHeight()) / maxy;
    if(!mode) yscale *= -1.0f;
    xscale = (float)(bounds.getWidth() - spacingleft) / (float)NUM_RESP_W;
    float y;
    for(float i=ystep; abs(i)<abs(maxy); i+=ystep){
        y = ybase + (yscale * i);
        g.drawLine(spacingleft - 1, y, spacingleft + 2, y);
        g.drawSingleLineText((mode ? "" : "2pi\u00D7") + String(i), 2, y + 5);
    }
    
    //Change from 2pi scale to real scale
    maxy = mode ? core.getMaxGroupDelay() : core.getMaxPhaseDelay();
    yscale = (spacingabove - bounds.getHeight()) / abs(maxy);
    
    //Data
    float lastval, thisval;
    lastval = mode ? core.getGroupDelay(0) : core.getPhaseDelay(0);
    float z;
    for(int i=1; i<NUM_RESP_W; ++i){
        thisval = mode ? core.getGroupDelay(i) : core.getPhaseDelay(i);
        if(mode){
            z = thisval;
            if(z < 0.001f) z = 0.001f;
            z = 0.7f - ((log10(z) + 3.0f) / 6.0f);
        }else{
            z = thisval / maxy;
        }
        g.setColour(Colour::fromHSV(z, 1.0f, 1.0f, 1.0f));
        g.drawLine(spacingleft + (xscale * (i-1)),
                   ybase + (yscale * lastval),
                   spacingleft + (xscale * i),
                   ybase + (yscale * thisval));
        lastval = thisval;
    }
}

void ResponseGraph::resized(){
    
}

void ResponseGraph::timerCallback(){
    core.notifyRecalcDly();
    repaint();
}
