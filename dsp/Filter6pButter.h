#pragma once

#include "rack.hpp"
#include "dsp/filter.hpp"
#include <assert.h>

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
    rack::dsp::TBiquadFilter<float> f[3];         // we need 3 biquads to make a 6p bw
};