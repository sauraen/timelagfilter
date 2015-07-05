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

#include "JuceHeader.h"
#include "TimeLagFilterCore.h"
#include "ResponseGraph.h"

#define PI 3.14159265358979f


TimeLagFilterCore::TimeLagFilterCore() : dtcalcthread("DTCalcThread", *this),
                                         dlycalcthread("DlyCalcThread", *this){
    reduced_fs = 0.0f;
    resample = decim_filter = interp_filter = false;
    ctr_modtype = bw_modtype = 0;
    ctr_param1 = ctr_param2 = 1000.0f;
    bw_param1 = bw_param2 = 0.5f;
    num_filters = num_points = 0;
    ring_start_idx = 0;
    sampleSwapDT = false;
    paramsChangedDelay = false;
    phasedelay.malloc(ResponseGraph::NUM_RESP_W);
    groupdelay.malloc(ResponseGraph::NUM_RESP_W);
    setup(128, 47999.0f);
}
TimeLagFilterCore::~TimeLagFilterCore(){
    block_u.free();
    block_y.free();
    ct_write.free();
    ct_calc.free();
    dt_read.free();
    dt_calc.free();
}

void TimeLagFilterCore::setup(int numfilters, float samplerate){
    const ScopedReadLock ctReadLock(ctLock);
    const ScopedReadLock dtReadLock(dtLock);
    //Create filters
    if(num_filters != numfilters){
        const ScopedWriteLock ctWriteLock(ctLock);
        const ScopedWriteLock dtWriteLock(dtLock);
        //NativeMessageBox::showMessageBoxAsync(AlertWindow::NoIcon, "TimeLagFilter", "Changing from " + String(num_filters) + " filters to " + String(numfilters));
        num_filters = numfilters;
        num_points = 3 * num_filters;
        block_u.realloc(num_points);
        block_y.realloc(num_points);
        ct_write.realloc(num_filters);
        ct_calc.realloc(num_filters);
        dt_read.realloc(num_filters);
        dt_calc.realloc(num_filters);
        clearData();
    }
    main_fs = samplerate;
    if(reduced_fs > main_fs || reduced_fs < 10.0f) reduced_fs = main_fs;
    if(ctr_param1 > getMaxCtr()) ctr_param1 = getMaxCtr();
    if(ctr_param2 > getMaxCtr()) ctr_param2 = getMaxCtr();
    recalcCtrs();
    recalcBWs();
    if(dtcalcthread.isThreadRunning()){
        dtcalcthread.notify();
    }
    //NativeMessageBox::showMessageBoxAsync(AlertWindow::NoIcon, "TimeLagFilter", "Setup finished with " + String(numfilters) + " filters and " + String(samplerate) + "Hz");
}

void TimeLagFilterCore::preparePlayback(){
    dtcalcthread.isThreadRunning()  ?  dtcalcthread.notify() :   dtcalcthread.startThread();
    dlycalcthread.isThreadRunning() ? dlycalcthread.notify() :  dlycalcthread.startThread();
}

int TimeLagFilterCore::getNumFilters(){
    return num_filters;
}

void TimeLagFilterCore::doResample(bool resamp){
    resample = resamp;
}
void TimeLagFilterCore::setResampleRate(float resamplerate){
    if(resamplerate >= main_fs || resamplerate < 10.0f){
        reduced_fs = main_fs;
    }else{
        reduced_fs = resamplerate;
    }
}
void TimeLagFilterCore::doDecimFilter(bool decimfilter){
    decim_filter = decimfilter;
}
void TimeLagFilterCore::doInterpFilter(bool interpfilter){
    interp_filter = interpfilter;
}

int TimeLagFilterCore::getCtrModType(){
    return ctr_modtype;
}
void TimeLagFilterCore::setCtrModType(int ctrmodtype){
    if(ctrmodtype < 0 || ctrmodtype > 5) return;
    ctr_modtype = ctrmodtype;
    recalcCtrs();
}
void TimeLagFilterCore::setCtrParam1(float ctrparam1){
    if(ctrparam1 < 10.0f) ctrparam1 = 10.0f;
    if(ctrparam1 > getMaxCtr()) ctrparam1 = getMaxCtr();
    ctr_param1 = ctrparam1;
    recalcCtrs();
}
void TimeLagFilterCore::setCtrParam2(float ctrparam2){
    if(ctrparam2 < 10.0f) ctrparam2 = 10.0f;
    if(ctrparam2 > getMaxCtr()) ctrparam2 = getMaxCtr();
    ctr_param2 = ctrparam2;
    recalcCtrs();
}
float TimeLagFilterCore::getCtrValue(int filter){
    const ScopedReadLock readLock(ctLock);
    if(filter < 0 || filter >= num_filters) return 0.0f;
    return ct_write[filter].center;
}
void TimeLagFilterCore::setCtrValue(int filter, float ctr){
    const ScopedReadLock readLock(ctLock);
    if(ctr_modtype != 5) return;
    if(ctr < 10.0f) ctr = 10.0f;
    if(ctr > getMaxCtr()) ctr = getMaxCtr();
    if(filter < 0 || filter >= num_filters) return;
    ct_write[filter].center = ctr;
    paramsChanged = paramsChangedDelay = true;
    if(dtcalcthread.isThreadRunning()){
        dtcalcthread.notify();
    }
}
float TimeLagFilterCore::getMinCtr(){
    return 10.0f;
}
float TimeLagFilterCore::getMaxCtr(){
    return reduced_fs*0.25f;
}
 
int TimeLagFilterCore::getBWModType(){
    return bw_modtype;
}
void TimeLagFilterCore::setBWModType(int bwmodtype){
    if(bwmodtype < 0 || bwmodtype > 5) return;
    bw_modtype = bwmodtype;
    recalcBWs();
}
void TimeLagFilterCore::setBWParam1(float bwparam1){
    if(bwparam1 < 0.0f) bwparam1 = 0.0f;
    if(bwparam1 > getMaxBW()) bwparam1 = getMaxBW();
    bw_param1 = bwparam1;
    recalcBWs();
}
void TimeLagFilterCore::setBWParam2(float bwparam2){
    if(bwparam2 < 0.0f) bwparam2 = 0.0f;
    if(bwparam2 > getMaxBW()) bwparam2 = getMaxBW();
    bw_param2 = bwparam2;
    recalcBWs();
}
float TimeLagFilterCore::getBWValue(int filter){
    const ScopedReadLock readLock(ctLock);
    if(filter < 0 || filter >= num_filters) return 0.0f;
    return ct_write[filter].bw;
}
void TimeLagFilterCore::setBWValue(int filter, float bw){
    const ScopedReadLock readLock(ctLock);
    if(bw_modtype != 5) return;
    if(bw < 0.0f) bw = 0.0f;
    if(bw > getMaxBW()) bw = getMaxBW();
    if(filter < 0 || filter >= num_filters) return;
    ct_write[filter].bw = bw;
    paramsChanged = paramsChangedDelay = true;
    if(dtcalcthread.isThreadRunning()){
        dtcalcthread.notify();
    }
}
float TimeLagFilterCore::getMinBW(){
    return 0.0f;
}
float TimeLagFilterCore::getMaxBW(){
    return 2.0f;
}

void TimeLagFilterCore::recalcCtrs(){
    const ScopedReadLock readLock(ctLock);
    float base;
    switch(ctr_modtype){
    case 0:
        //Fixed
        for(int i=0; i<num_filters; ++i){
            ct_write[i].center = ctr_param1;
        }
        break;
    case 1:
        //Linear
        for(int i=0; i<num_filters; ++i){
            ct_write[i].center = ctr_param1 + (ctr_param2 - ctr_param1) * i / (num_filters-1);
        }
        break;
    case 2:
        //Log
        base = (ctr_param2 - ctr_param1) / log((float)num_filters);
        for(int i=0; i<num_filters; ++i){
            ct_write[i].center = log((float)i+1.0f) * base + ctr_param1;
        }
        break;
    case 3:
        //Gaussian
        
        break;
    case 4:
        //Dual
        for(int i=0; i<num_filters; i++){
            ct_write[i].center = (i&1) ? ctr_param2 : ctr_param1;
        }
        break;
    case 5:
        //Custom
        //Just check to make sure all centers are in valid range
        for(int i=0; i<num_filters; ++i){
            if(ct_write[i].center > getMaxCtr()){
                ct_write[i].center = getMaxCtr();
            }
        }
        break;
    default:
        return;
    }
    paramsChanged = paramsChangedDelay = true;
    if(dtcalcthread.isThreadRunning()){
        dtcalcthread.notify();
    }
}
void TimeLagFilterCore::recalcBWs(){
    const ScopedReadLock readLock(ctLock);
    float base;
    switch(bw_modtype){
    case 0:
        //Fixed
        for(int i=0; i<num_filters; ++i){
            ct_write[i].bw = bw_param1;
        }
        break;
    case 1:
        //Linear
        for(int i=0; i<num_filters; ++i){
            ct_write[i].bw = bw_param1 + (bw_param2 - bw_param1) * i / (num_filters-1);
        }
        break;
    case 2:
        //Log
        base = (bw_param2 - bw_param1) / log((float)num_filters);
        for(int i=0; i<num_filters; ++i){
            ct_write[i].bw = log((float)i+1.0f) * base + bw_param1;
        }
        break;
    case 3:
        //Gaussian
        
        break;
    case 4:
        //Dual
        for(int i=0; i<num_filters; i++){
            ct_write[i].bw = (i&1) ? bw_param2 : bw_param1;
        }
        break;
    case 5:
        //Custom
        break;
    default:
        return;
    }
    paramsChanged = paramsChangedDelay = true;
    if(dtcalcthread.isThreadRunning()){
        dtcalcthread.notify();
    }
}

void TimeLagFilterCore::clearData(){
    for(int i=0; i<num_filters; ++i){
        ct_write[i].center = 1000.0f;
        ct_write[i].bw = 0.5f;
        ct_calc[i].center = 1000.0f;
        ct_calc[i].bw = 0.5f;
        dt_read[i].b = 0.0f;
        dt_read[i].c = 0.0f;
        dt_calc[i].b = 0.0f;
        dt_calc[i].c = 0.0f;
    }
    for(int i=0; i<num_points; ++i){
        block_u[i] = 0.0f;
        block_y[i] = 0.0f;
    }
}


TimeLagFilterCore::DTCalcThread::DTCalcThread(const String& threadName, TimeLagFilterCore& filterCore)
        : Thread(threadName), core(filterCore){
    
}
TimeLagFilterCore::DTCalcThread::~DTCalcThread(){
    
}

void TimeLagFilterCore::DTCalcThread::run(){
    //Disable denormalized floats
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    //Start
    float b, c;
    while(!threadShouldExit()){
        if(!core.paramsChanged) wait(-1);
        if(threadShouldExit()) return;
        if(core.sampleSwapDT){ 
            //Don't recalculate if waiting for sample to finish
            wait(1); //Try again soon
        }else{
            //Copy write to calc
            {
                const ScopedWriteLock writeLock(core.ctLock);
                memcpy(core.ct_calc, core.ct_write, core.num_filters * sizeof (CTParams));
                core.paramsChanged = false;
            }
            //Calculate filter coefficients
            const ScopedWriteLock writeLock(core.dtLock);
            for(int i=0; i<core.num_filters; ++i){
                filtercalculations(core.reduced_fs, core.ct_calc[i].center, core.ct_calc[i].bw, &b, &c);
                core.dt_calc[i].b = b;
                core.dt_calc[i].c = c;
            }
            core.sampleSwapDT = true;
        }
    }
}

TimeLagFilterCore::DlyCalcThread::DlyCalcThread(const String& threadName, TimeLagFilterCore& filterCore)
        : Thread(threadName), core(filterCore){
    
}
TimeLagFilterCore::DlyCalcThread::~DlyCalcThread(){
    
}

void TimeLagFilterCore::DlyCalcThread::run(){
    //Disable denormalized floats
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    //Start
    float phasesum, groupsum;
    float a, b, center, bw, ctrsquared;
    float phase, group;
    float w, wsquared, twow, dw;
    int f, nw;
    while(!threadShouldExit()){
        if(!core.paramsChangedDelay) wait(-1);
        if(threadShouldExit()) return;
        {
            const ScopedReadLock readLock(core.ctLock);
            //Calculate delays
            dw = core.getMaxCtr() / ResponseGraph::NUM_RESP_W;
            w = 0.0f;
            for(nw = 0; nw < ResponseGraph::NUM_RESP_W; ++nw){
                phasesum = 0.0f;
                groupsum = 0.0f;
                wsquared = w * w;
                twow = 2.0f * w;
                for(f=0; f<core.num_filters; ++f){
                    center = core.ct_write[f].center;
                    bw = core.ct_write[f].bw;
                    ctrsquared = center * center;
                    a = ctrsquared - wsquared;
                    b = twow * bw * center;
                    phase = -2.0f * atan2(b, a);
                    group = -4.0f * bw * center * (ctrsquared - (core.getMaxBW() * wsquared)) / (a*a + b*b);
                    phasesum += phase;
                    groupsum += group;
                }
                core.phasedelay[nw] = phasesum;
                core.groupdelay[nw] = groupsum;
                w += dw;
            }
            core.paramsChangedDelay = false;
        }
    }
}
void TimeLagFilterCore::notifyRecalcDly(){
    dlycalcthread.notify();
}
float TimeLagFilterCore::getPhaseDelay(int nw){
    if(nw < 0 || nw >= ResponseGraph::NUM_RESP_W) return 0.0f;
    return phasedelay[nw];
}
float TimeLagFilterCore::getGroupDelay(int nw){
    if(nw < 0 || nw >= ResponseGraph::NUM_RESP_W) return 0.0f;
    return groupdelay[nw];
}
float TimeLagFilterCore::getMaxPhaseDelay(){
    return num_filters * -2.0f * PI;
}
float TimeLagFilterCore::getMaxGroupDelay(){
    float max = 0.0f;
    for(int nw = 0; nw < ResponseGraph::NUM_RESP_W; ++nw){
        if(groupdelay[nw] > max) max = groupdelay[nw];
    }
    return max;
}



void TimeLagFilterCore::filtercalculations(float fs, float center, float bw, float* b, float* c){
    float fL, fH, pioverfs, OmegaL, OmegaH, Ba, Omega0squared, a;
    //Check input parameters
    if(center <= 0.0f || center >= fs * 0.5f){
        center = fs * 0.25f;
    }
    if(bw <= 0.0f || bw >= 1.0f){
        bw = 0.5f;
    }
    //Calculations
    fL = center * (sqrt(bw*bw + 1) - bw);
    fH = bw*center;
    fH += fH + fL;
    pioverfs = PI / fs;
    OmegaL = tan(pioverfs * fL);
    OmegaH = tan(pioverfs * fH);
    Ba = OmegaH - OmegaL;
    Omega0squared = OmegaH * OmegaL;
    a = 1.0f / (Omega0squared + Ba + 1.0f);
    *b = a * (Omega0squared - 1.0f);
    *b += *b;
    *c = a * (Omega0squared - Ba + 1.0f);
}

    
float TimeLagFilterCore::sample(float in, int outindex){
    const ScopedReadLock readLock(dtLock);
    //TODO resampling
    float out = 0.0f;
    float un = in, yn = 0.0f;
    int f, n, n_1, n_2;
    //Convert to raw pointers for speed
    float* u = block_u.getData();
    float* y = block_y.getData();
    DTParams* fp = dt_read.getData();
    if(u == nullptr || y == nullptr || fp == nullptr){
        return 0.0f;
    }
    //Start indices for circular buffer
    n_2 = ring_start_idx;
    if(n_2 >= num_points) n_2 -= num_points;
    n_1 = n_2 + 1;
    if(n_1 >= num_points) n_1 -= num_points;
    n   = n_1 + 1;
    if(n   >= num_points) n   -= num_points;
    //Run all filters
    for(f=0; f<num_filters; ++f){
        //LTI system
        u[n] = un;
        yn = (fp[f].c * (u[n] - y[n_2])) + (fp[f].b * (u[n_1] - y[n_1])) + u[n_2];
        y[n] = yn;
        //Check for outputting from this block
        if(outindex == f) out = yn;
        //Output to next block
        un = yn;
        //Move to next block with circular addressing
        n_2 = n   + 1;
        if(n_2 >= num_points) n_2 -= num_points;
        n_1 = n_2 + 1;
        if(n_1 >= num_points) n_1 -= num_points;
        n   = n_1 + 1;
        if(n   >= num_points) n   -= num_points;
    }
    //Start at next buffer position next time
    ++ring_start_idx;
    if(ring_start_idx >= num_points) ring_start_idx -= num_points;
    //Use next set of parameters if applicable
    if(sampleSwapDT){
        const ScopedWriteLock writeLock(dtLock);
        dt_read.swapWith(dt_calc);
        sampleSwapDT = false;
    }
    //Output
    return out;
}
