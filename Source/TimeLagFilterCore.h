/*
  ==============================================================================

    TimeLagFilterCore.h
    Created: 2 May 2015 9:39:40pm
    Author: Sauraen

  ==============================================================================
*/

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
