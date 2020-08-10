# Demo VCO3

We have seen that Demo VCO2 is already extremely fast. If this were a real project we might stop now - it's clearly "good enough". But it is a fun challenge to make it even faster.

## SIMD Instructions

Earlier we mentioned that the Fundamental VCO-1 uses the SIMD instructions to be much more efficient when polyphony is used. Luckily since we already borrowed our complex minBlep code from Fundamental, we can easy "vectorize" it.

First, a little bit about this technology.

SIMD is an acronym for Single Instruction Multiple Data. It means that in a single Intel instruction you can, for example, multiply four numbers by four other numbers in the time that you could only process one instruction with the normal instruction set. This is also called "vector processing", as the CPU registers are now holding vectors rather than scalars.

Intel has been introducing more and more of these extensions year by year. Some of the major milestones were the original MMX, which could only handle vectors of integers (and hence not too useful for audio). SSE is the version where floating point numbers could be processed. AVX increased the vector size from 4 to 8 etc...

But there have always been barriers to the adoption of SIMD:

* It's often been done with "intrinsics", which look a lot like assembly language, and are not easy to use.
* The programming model is hard to get used to. For example, conventional "if" statements aren't allowed.
* Typically you need some kind of "dynamic linking" so you could provide fallback if the user's CPU doesn't have the right SIMD instruction set.

Thankfully, VCV has solved the first and the last of these issues. VCV's SIMD library introduces a new data type, `float_4`, and uses extensive C++ operator overloading to make the syntax much like regular C.

As to the last point, VCV has specified that VCV Rack requires certain SIMD instruction sets, and therefore any plugin that uses the same instruction sets will be guaranteed to work for any VCV user. The small downside of this, however, is that the model VCV has settled on is pretty old, least common denominator model that covers any computer made in the last 9 years or so.  VCV has queried users to find what percentages have what vintages of computers and made a reasonable choice from that.

Regarding the second issue, the general difficulty of the programming model - well, you just need to learn how to do it. Or copy code from someone who does. In many complex cases it's easy to write SIMD code that is actually slower than the equivalent non-SIMD code - so you need to test a lot.

But, the good news is the code provided to plugins by VCV. Most of the SIMD stuff can be found here: [SIMD Includes](https://github.com/VCVRack/Rack/tree/v1/include/simd). You are encouraged to rummage around in there. Especially `vector.hpp`.

## Demo VCO3

Demo VCO3 is of course Demo VCO2 re-implemented using SIMD instructions provide by the VCV Plugin SDK. This was very straight forward, except for the minBlep part. Thankfully VCV Fundamental VCO-1 has a SIMD minBlep so we can just use that code.

We made a new plugin and tested it out. It worked fine, but something was disappointing. While the sawtooth had become four times faster, the sin was not faster at all!

Since the only complicated part of this is the sine approximation we borrowed, we shut that off. While it no longer generated a sine, it was now blindingly fast. So the problem was clearly in the sine approximation.

We noticed that the approximation code itself is fairly complicated:

``` C++
(T(-6.283185307) * x + T(33.19863968) * simd::pow(x, 3) - T(32.44191367) * simd::pow(x, 5))
    / (1 + T(1.296008659) * simd::pow(x, 2) + T(0.7028072946) * simd::pow(x, 4));
```

Maybe the GCC compiler is not able to convert this into SIMD code that will fit in the 8 registers allowed by the VCV legacy compatibility? So, as a lark, we replaced `-march=nacona` with `-march=native`, which is how you tell GCC "go crazy, use all the SIMD features of my computer".

This made the result 3X faster, suggesting our theory could be correct.

So we replaced that sine approximation with one we wrote. That was able to get the 3X speedup, but didn't need the non-standard compiler flags. btw, it would be a very bad idea to put a plugin into the plugin manager that used "unofficial" compiler settings.

So, we used our own approximation, reset the settings back to normal, and went on to testing.

btw: here is our inelegant and not very smart sine approximation:

```c++
/*
 * This time we are using our own sine approximation, rather than the
 * one from Fundamental VCO-1. This one is a little crude, not super
 * accurate, and really could be optimized more.
 *
 * This one is done by doing the calculations with two different
 * equations depending on whether the radian angle is  more or less than pi.
 *
 * The equations themselves are very simple fourth order Taylor series, with
 * an arbitrary fudge factor added in to make it more accurate.
 *
 * This can undoubtedly be improved.
 *
 * input: _x must be >= 0, and <= 2 * pi.
 */

inline float_4 SquinkyLabs_sinTwoPi(float_4 _x) {
    const static float twoPi = 2 * 3.141592653589793238;
    const static float pi =  3.141592653589793238;
    _x -= ifelse((_x > float_4(pi)), float_4(twoPi), float_4::zero());

    float_4 xneg = _x < float_4::zero();
    float_4 xOffset = ifelse(xneg, float_4(pi / 2.f), float_4(-pi  / 2.f));
    xOffset += _x;
    float_4 xSquared = xOffset * xOffset;
    float_4 ret = xSquared * float_4(1.f / 24.f);
    float_4 correction = ret * xSquared *  float_4(.02 / .254);
    ret += float_4(-.5);
    ret *= xSquared;
    ret += float_4(1.f);

    ret -= correction;
    return ifelse(xneg, -ret, ret);
}
```

Up next: analyzing Demo VCO3. [Link here](./vco3-cpu.md)