# Demo

This repo contains a [VCV Rack](https://vcvrack.com) plugin which implements a few VCV Rack synthesizer modules. It's intended to demonstrate how to make a basic VCV Rack plugin rather than an actual "useful" synthesizer module.

As of now, the code is for a VCV 1.x module. It has not yet been ported to VCV 2.0

## Prerequisites

If you intend to build this plugin, or any VCV plugin, you will need to set up a build environment. Please refer to [the VCV Rack manual](https://vcvrack.com/manual/) for instructions on [setting up your build environment for Windows, Mac and Linux](https://vcvrack.com/manual/Building).

The VCV Manual has a lot of [information that is useful to plugin developers](https://vcvrack.com/manual/PluginDevelopmentTutorial), and you should definitely read the manual if you intend to make your own plugins.

To build a plugin, you must have either the VCV Rack Plugin SDK (aka "VCV Rack SDK") installed correctly, or you must first build VCV Rack itself from scratch. We always build it ourselves.

Most VCV Rack plugin code is written using the C++ ("C plus plus") programming language. Developing VCV Rack plugins is a great way to learn C++ and there are many free resources online to help you learn and improve your C++ knowledge, like [Learn X in Y Minutes](https://learnxinyminutes.com/docs/c++/), [LearnCPP.com](https://www.learncpp.com), and [Cprogramming.com](https://www.cprogramming.com).

## Tools needed to build this Demo plugin

You must have an appropriate set of build tools installed. On Windows that means the MSYS2 shell. Please follow the instructions linked to above about setting up your build environment.

You must have some sort of text editor, preferably one designed for writing computer code. There are many good, free ones available. We like [Microsoft Visual Studio Code](https://code.visualstudio.com).

## Don't forget the VCV Rack manual

Although unfinished, [the VCV Rack manual has an excellent chapter on DSP](https://vcvrack.com/manual/DSP) (Digital Signal Processing). The chapter talks about band-limited waveforms, mentions minBlep, and has an excellent section on optimizing code for minimum CPU usage. You don't need to read the chapter as a prerequisite to this guide, but if you're going to build your own VCV Rack plugins you really should read it.

## Demo VCO1

Demo VCO1 is a very simple polyphonic VCO ([Voltage Controlled Oscillator](https://en.wikipedia.org/wiki/Voltage-controlled_oscillator)). It's not a particularly good one, but the code (which is in the file `res/VCO1.cpp`) is very small, and hopefully reasonably readable.

The Demo VCO1 module has several reasons to exist:

* It shows some very simple algorithms for making VCO waveforms.
* It demonstrates some of the pitfalls of overly simplistic DSP code.
* It shows how to implement polyphony in a VCV Rack module.
* It shows how to make a VCV Rack plugin purely in code, without using the Python-based plugin creator utility from the VCV Rack manual.

In most cases plugin authors will want to create their initial plugins using the Python-based utility provided with the VCV Rack SDK, but we've made our plugin purely in code here because:

* It's how we happen to make plugins.
* Some people have expressed interest in how to make a plugin purely in code, so this project can be a reference for those developers.

All three Demo VCO modules (VCO1, VCO2 and VCO3) output three different waveforms: sine, saw, and a mutant parabolic saw. Each of the three different waveforms are sent out through their own dedicated output ports.

## Demo VCO2

Demo VCO2 functions exactly the same as Demo VCO1, but fixes some problems with it, such as aliasing.

## Demo VCO3

Demo VCO3 functions exactly the same as Demo VCO2, but now uses the Intel SIMD instruction set to make the module's code run faster.

## Table of contents

1. [Initial testing results for Demo VCO1](./docs/vco1-first.md)
2. [Looking in detail at Demo VCO1's CPU usage](./docs/vco1-cpu.md)
3. [How to use a spectrum analyzer](./docs/analyzer.md)
4. [How does Demo VCO1 look on the analyzer? (And an introduction to aliasing)](./docs/aliasing.md)
5. [More on measuring aliasing](./docs/aliasing2.md)
6. [Introducing Demo VCO2](./docs/vco2.md)
7. [Introducing Demo VCO3](./docs/vco3.md)
8. [Analysis of Demo VCO3](./docs/vco3-cpu.md)

## Summary

Hopefully you will read all the pages listed above and didn't skip here immediately! In any case, here are some of the points we hope we demonstrated in this repo…

We learned how to easily measure the CPU usage and distortion of VCV modules.

We learned that it's really easy to make a VCV module that's terribly wasteful of CPU. But also that it's really easy to crudely measure CPU usage, and usually very easy to fix. Another plug for [the VCV Rack manual](https://vcvrack.com/manual/) - it has an excellent section on this topic.

We also saw that it's very easy to make a VCO that generates a huge amount of aliasing distortion, even without trying. The good news is that the VCV Rack SDK provides a very easy to use implementation of minBlep that can fix this, at least for VCOs like those in our Demo modules.

We gave a very short introduction to SIMD coding, in particular how much it can reduce the CPU usage of a polyphonic module.

We demonstrated something that is already well known - the C++ standard functions `std::sin()` and `std::pow()` use a lot of CPU and can be effectively approximated to speed up your code.

Lastly, we provided source code that can easily be reused for someone who wants to start a plugin "from code". The code has lots of comments in it that will hopefully make it easier for others to start writing VCV modules.

For more on writing efficient plugins, aside from the excellent info in the VCV manual, [here is a paper on writing efficient plugins](./docs/efficient-plugins.md) which we wrote over a year ago. It's badly in need of an update to reflect changes made in VCV Rack v1.0, but it has been a popular source of information for plugin developers and is well worth reading.

## Addendum

We have updated some of the source code in response to suggestions from other developers. In some cases, these changes were not propagated to Demo VCO2 and Demo VCO3 because we didn't want to re-test everything.

Demo VCO1 now uses the VCV Rack SDK `clamp()` utility function to keep the frequency in a legal range, so the other VCOs probably have bugs at very high pitches because they lack that fix.

We added extensive comments about the decision to sub-sample the CV calculations, so that you can be more aware of the trade-offs involved.
