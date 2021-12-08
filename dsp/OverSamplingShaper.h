#pragma once

#include "Filter6PButter.h"

#define OVERSAMPLING_FACTOR 16

class OverSamplingShaper {
public:
    OverSamplingShaper() {
        interpolatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
        decimatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
    }

    float process(float input) {
        float output=0;
        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {
            float signal = (i == 0) ? input : 0.f;
            signal = interpolatingFilter.process(signal);
            signal *= OVERSAMPLING_FACTOR;
            signal = processShape(signal);
            signal = decimatingFilter.process(signal);
            if (i == 0) {
                output = signal;
            }
        }
        return output;

    }

    float processGood(float x) {
        // Initialize the array with zeros
        float oversampleBuffer[OVERSAMPLING_FACTOR]{0.0f};
        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {
            assert(oversampleBuffer[i] == 0.f);
        }
        // Set the first sample to the input
        oversampleBuffer[0] = x;

        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {
            // Pre-calc low pass filter
            oversampleBuffer[i] = interpolatingFilter.process(oversampleBuffer[i]);
            oversampleBuffer[i] *= OVERSAMPLING_FACTOR;
            // Run shaping calculation
           // oversampleBuffer[i] = processShape(oversampleBuffer[i]);
        }
#if 1
        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {
            oversampleBuffer[i] = processShape(oversampleBuffer[i]);
        }
#endif

        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {
            // Post-calc low pass filter
            oversampleBuffer[i] = decimatingFilter.process(oversampleBuffer[i]);
        }

        // Ignore zero padding
        return oversampleBuffer[0];
    }

private:
    virtual float processShape(float) = 0;

    Filter6PButter interpolatingFilter;
    Filter6PButter decimatingFilter;
};