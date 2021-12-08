#pragma once

#include "Filter6PButter.h"

// This can be set to any integer greater than 1
#define OVERSAMPLING_FACTOR 16

class OverSamplingShaper {
public:
    OverSamplingShaper() {
        // Set the cutoff for both filters to start an octave below nyquist.
        // This will be 11.025 at 44.1k.
        // It may be possible to move the cutoff higher, especially at high oversample factors.
        interpolatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
        decimatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
    }

    // This function in effect expands a single sample into a buffer of OVERSAMPLING_FACTOR
    // size, processes it, then take back down to single sample. But an actual memory buffer is
    // not necessary.
    float process(float input) {
        float signal;
        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {

            // The input is basically a buffer where the first
            // sample has the value, and the rest is packed with zeros.
            // now the sample is at the higher oversample rate, but will still
            // have images at 2x, 4x, etc...
            // Compensate for all the zeros we stuffed in, which reduce the level.
            signal = (i == 0) ? input * OVERSAMPLING_FACTOR : 0.f;   

            // Then we apply an interpolating filter. Now we just have the
            // oversampled input, but with no high freq images.             
            signal = interpolatingFilter.process(signal);

            // now apply the wave shaping while we are at the oversample rate.
            signal = processShape(signal);

            // filter out all the generated harmonic that are higher than our
            // original nyquist frequency.
            signal = decimatingFilter.process(signal);
        }

        // we could return any sample from our virtual "buffer",
        // but it's convenient to use the last one.
        return signal;
    }

private:
    virtual float processShape(float) = 0;
    Filter6PButter interpolatingFilter;
    Filter6PButter decimatingFilter;
};