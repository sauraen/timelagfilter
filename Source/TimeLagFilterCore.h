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

#ifndef TIMELAGFILTERCORE_H_INCLUDED
#define TIMELAGFILTERCORE_H_INCLUDED

#include "JuceHeader.h"


class TimeLagFilterCore {
public:
    TimeLagFilterCore();
    ~TimeLagFilterCore();
    
    //Filter array
    void setup(int numfilters, float samplerate);
    int getNumFilters();
    
    //Resample engine
    void doResample(bool resamp);
    void setResampleRate(float resamplerate);
    void doDecimFilter(bool decimfilter);
    void doInterpFilter(bool interpfilter);
    
    //Parameters
    int getCtrModType();
    void setCtrModType(int ctrmodtype);
    void setCtrParam1(float ctrparam1);
    void setCtrParam2(float ctrparam2);
    float getCtrValue(int filter);
    void setCtrValue(int filter, float ctr);
    float getMinCtr();
    float getMaxCtr();
    
    int getBWModType();
    void setBWModType(int bwmodtype);
    void setBWParam1(float bwparam1);
    void setBWParam2(float bwparam2);
    float getBWValue(int filter);
    void setBWValue(int filter, float bw);
    float getMinBW();
    float getMaxBW();
    
    void notifyRecalcDly();
    float getPhaseDelay(int nw);
    float getGroupDelay(int nw);
    float getMaxPhaseDelay();
    float getMaxGroupDelay();
    
    //Sample
    void preparePlayback();
    float sample(float in, int outindex);

protected:
    class DTCalcThread : public Thread {
    public:
        DTCalcThread(const String& threadName, TimeLagFilterCore& filterCore);
        ~DTCalcThread();
        
        void run() override;
    private:
        TimeLagFilterCore& core;
    };
    class DlyCalcThread : public Thread {
    public:
        DlyCalcThread(const String& threadName, TimeLagFilterCore& filterCore);
        ~DlyCalcThread();
        
        void run() override;
    private:
        TimeLagFilterCore& core;
    };
    
    struct CTParams {
        float center;
        float bw;
    };
    struct DTParams {
        float b;
        float c;
    };
    
    void clearData();

    void recalcCtrs();
    void recalcBWs();
    static void filtercalculations(float fs, float center, float bw, float* b, float* c);

    //Parameters
    int num_filters, num_points;
    float main_fs, reduced_fs;
    bool resample, decim_filter, interp_filter;
    
    int ctr_modtype, bw_modtype;
    float ctr_param1, ctr_param2, bw_param1, bw_param2;

    //u and y arrays of signal data--three per filter
    HeapBlock<float> block_u;
    HeapBlock<float> block_y;
    int ring_start_idx;
    
    //Multi-buffered arrays of filter parameters
    HeapBlock<CTParams> ct_write;
    HeapBlock<CTParams> ct_calc;
    HeapBlock<DTParams> dt_read;
    HeapBlock<DTParams> dt_calc;
    //Arrays currently active
    bool sampleSwapDT;
    bool paramsChanged;
    bool paramsChangedDelay;
    
    //Delay calculations
    HeapBlock<float> phasedelay;
    HeapBlock<float> groupdelay;
    
    //Multithreading
    DTCalcThread dtcalcthread;
    DlyCalcThread dlycalcthread;
    ReadWriteLock ctLock;
    ReadWriteLock dtLock;
    

    
};



#endif  // TIMELAGFILTERCORE_H_INCLUDED
