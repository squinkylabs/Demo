#pragma once

#include <assert.h>
#include "rack.hpp"

class Filter6PButter {
public:
    void setCutoffFreq(float x) {
        INFO("set cutoff freq %f", x);
        assert(x > 0 && x < .5f);
        f[0].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, .51763809, 1);
        f[1].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 0.70710678, 1);
        f[2].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 1.9318517, 1);
    }

    float process(float x) {
        x = f[0].process(x);
        x = f[1].process(x);
        x = f[2].process(x);
        return x;
    }

private:
    rack::dsp::TBiquadFilter<float> f[3];  // we need 3 biquads to make a 6p bw
};

class Filter12PButter {
public:
    void setCutoffFreq(float x) {
        INFO("set cutoff freq %f", x);
        assert(x > 0 && x < .5f);
        // calulator https://www.earlevel.com/main/2016/09/29/cascading-filters/
        f[0].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, .50411961, 1);
        f[1].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 0.541196108, 1);
        f[2].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 0.63023621, 1);
        f[3].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 0.82133982, 1);
        f[4].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 1.3065630, 1);
        f[5].setParameters(rack::dsp::TBiquadFilter<float>::LOWPASS, x, 3.8306488, 1);
    }

    float process(float x) {
        x = f[0].process(x);
        x = f[1].process(x);
        x = f[2].process(x);
        x = f[3].process(x);
        x = f[4].process(x);
        x = f[5].process(x);
        return x;
    }

private:
    rack::dsp::TBiquadFilter<float> f[6];  // we need 3 biquads to make a 6p bw
};