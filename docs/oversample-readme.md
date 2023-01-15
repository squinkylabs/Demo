# The oversampled distortion and filter

These are two modules: Demo Shaper1 and Demo Filter1.

They are both as basic as possible, to make it easy to see what's going on.

Oversampling is a simple and effective way to reduce the aliasing that is often created as a side-effect of various DSP operations. For this example, we used a simple clipper.

There is a lot of information out there about oversampling - if you are interested, be sure to ask Google for more info.

There are many effective ways of controlling aliasing. minBLEP is often used to reduce aliasing when generating "classic" waveforms in a digital VCO. There are many other more advanced ways of reducing aliasing, too. But oversampling is such an easy and effective one that it is often used.

The principal is pretty simple. You take the input signal, if any, convert it to a much higher sample rate, do the processing, then filter out all the upper harmonics that are generated, and convert back to the original sample rate.

The first step, converting to a higher sample rate is often called *interpolation* which takes the input samples and interpolates between them to re-create the missing samples that are at the higher sample rate.

Similarly, decimation is used to reduce the over-sampled signal back down to the input rate.

There are many, many ways to do these things, with many tradeoffs. Some of the tradeoffs include:

* Flat frequency response.
* CPU usage.
* Suppression of undesirable artifacts.

We use IIR Butterworth low pass filters for both the interpolation filter, the output filter that removes the high harmonics that would otherwise alias down to audible frequencies.

This is also handy, as the VCV SDK has nice filters in it, but you are pretty much on your own to design them. The Butterworth filter is quite common, so you never know when someone will want to know how to design one.

That said, there are most likely other filters that work "better" for oversampling implementations.

## Demo Filter1

Since the over-sampler will use a filter, we first made a module to do just the filtering.

## Demo Shaper1

Here we made a clipper that uses oversampling to reduce the aliasing. It has outputs both with and without the oversampling, so that it's easy to compare.

A simple clipper like this without oversampling would have quite a bit of aliasing, and would not sound very nice.

There are many ways in software to make an oversampling effect. The way we did it by putting it all in a utility class makes it easy to show how simple this it. In a "real" module one would probably architect things differently.
