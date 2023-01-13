#pragma once

#include "Filter6PButter.h"

// This can be set to any integer greater than 1
#define OVERSAMPLING_FACTOR 16

/**
 * This we where we are actually doing to oversampling.
 * OverSamplingShaper is an abstract class, and can not be instantiated. 
 * To use if one must derive a concrete class and provide an implementation of
 * virtual float processShape(float)
 */
class OverSamplingShaper {
public:
    OverSamplingShaper() {
        // Set the cutoff for both filters to start an octave below nyquist.
        // This will be 11.025 at 44.1k.
        // It may be possible to move the cutoff higher, especially at high over sample factors.
        // This will be highly dependent on what kind of filter is used, etc...
        // This example shows a plausible way to use Butterworth filter, but as the
        // other documents mention, that are many trade-offs here, and many different filter
        // types that could be used.
        interpolatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
        decimatingFilter.setCutoffFreq(1.f / (OVERSAMPLING_FACTOR * 4));
    }

    // This function in effect expands a single sample into a buffer of OVERSAMPLING_FACTOR
    // size, processes it, then takes back down to single sample. But an actual memory buffer is
    // not necessary, so we don't make one.
    float process(float input) {
        float signal;
        for (int i = 0; i < OVERSAMPLING_FACTOR; ++i) {

            // The input is basically a buffer where the first
            // sample has the value, and the rest is packed with zeros.
            // now the sample is at the higher oversample rate, but will still
            // have frequency images at 2x, 4x, etc...
            // Compensate for all the zeros we stuffed in, which reduce the level, 
            // by scaling up the input
            signal = (i == 0) ? input * OVERSAMPLING_FACTOR : 0.f;   

            // Then we apply an interpolating filter. Now we just have the
            // oversampled input, but with greatly attenuated high frequency images.             
            signal = interpolatingFilter.process(signal);

            // now apply the wave shaping while we are at the over sampled rate.
            signal = processShape(signal);

            // filter out all the generated harmonic that are higher than our
            // original nyquist frequency.
            signal = decimatingFilter.process(signal);
        }

        // We could return any sample from our virtual "buffer",
        // but it's convenient to use the last one.
        return signal;
    }

private:
    virtual float processShape(float) = 0;
    Filter6PButter interpolatingFilter;
    Filter6PButter decimatingFilter;
};