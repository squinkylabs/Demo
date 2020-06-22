# Demo

This repo contains a VCV Rack plugin which implements a few VCV Rack synthesizer modules. It is intended more as a demonstration for how to make VCV plugins than an actual useful synthesizer module.

## Caveat

This code and documentation is currently under active development and proofreading. It tends to change at least once a day. However please report and issues with the code or the writing.

## Prerequisites

If you intend to build this plugin, or any VCV plugin, you will need to set up a build environment. Instruction are [here](https://vcvrack.com/manual/Building).

The VCV Manual has a lot of information that is useful to plugin developers. Definitely read those sections if you intend to make a plugin. The entire section on plugin development is useful. It starts [here](https://vcvrack.com/manual/PluginDevelopmentTutorial).

To build a plugin, you must have either the VCV Plugin SDK installed correctly, or you must first build VCV itself from scratch. We always build it ourselves.

## Tools needed to build Demo

You must have an appropriate set of build tools installed. On Windows that means the MSYS2 shell. Follow the directions linked above to get this all set up.

You must have some sort of text editor, preferably one designed for writing computer code. There are many good, free ones available. We like Visual Studio Code.

## Don't forget the VCV manual

Although unfinished, the VCV manual has an excellent chapter on DSP. It talks about band-limited waveforms, mentions minBlep, and has an excellent section on optimizing code for minimum CPU usage. You don't need to read that as a prerequisite to our documents, but if you are going to build your own VCV module you really should read it. [link here](https://vcvrack.com/manual/DSP)

## Demo VCO1

Demo VCO1 is a very simple polyphonic VCO. It is not a particularly good one, but the code is very small, and hopefully reasonably readable.

This Module has several reasons to exist:

* It shows how to make a VCV Rack plugin purely in code, without using the normal python plugin creator.
* It shows some very simple algorithms for making VCO waveforms.
* It demonstrates some of the pitfalls of overly simplistic DSP code.
* It shows how to implement polyphony.

VCO1 produces three different waveforms: sine, saw, and a mutant parabolic saw. Each waveforms comes out on a different jack.

## VCO2

VCO2 functions exactly the same as VCO1, but fixed the problems identified in VCO1.

## VCO3

Again, same as VCO2, but now using the Intel SIMD instruction set to make it faster. 

## Table of contents

Initial testing results for VCO1: [link here](./docs/vco1-first.md)

Looking in detail at VCO1's CPU usage: [link here](./docs/vco1-cpu.md)

How to use a spectrum analyzer: [link here](./docs/analyzer.md)

How does Demo VCO1 look on the analyzer? (and introduction to aliasing): [link here](./docs/aliasing.md)

More on measuring aliasing: [link here](./docs/aliasing2.md)

Introducing Demo VCO2: [link here](./docs/vco2.md)

Introducing Demo VCO3: [link here](./docs/vco3.md)

## Summary

Hopefully you read all this pages listed above, and didn't skip here immediately! In any case here are some of the points we hope we demonstrated here:

We learned how to easily measure the CPU usage and Distortion from VCV modules.

We learned that it is really easy to make a VCV module is terribly wasteful of CPU. But it is easy to measure crudely, and usually very easy to fix. Another plug for the VCV manual - it has an excellent section on this topic.

We also saw it is very easy to make a VCO that generates a huge amount of aliasing distortion, even without trying. The good news is that the VCV plugin SDK provides a very easy to use implementation on minBlep that can fix this, at least for VCOs like Demo.

Lastly, we provided source code that can easily be re-used for someone who wants to start a pluigin "from code". The code has lots of comments in it that will hopefully make it easier for others to start writing VCV modules.

For more on writing efficient plugins aside from the excellent info in the VCV manual, there is this paper we wrote over a year ago. It's badly in need of update to VCV 1.0, but it has been a popular source of info: [writing efficient plugins](https://github.com/squinkylabs/SquinkyVCV/blob/master/docs/efficient-plugins.md)
