# Demo

This repo contains a VCV Rack plugin which implements a few VCV Rack synthesizer modules. It is intended more as a demonstration for how to make VCV plugins than an actual useful synthesizer module.

## Prerequisites

If you intend to build this plugin, or any VCV plugin, you will need to set up a build environment. Instruction are [here](https://vcvrack.com/manual/Building).

The VCV Manual has a lot of information that is useful to plugin developers. Definitely read those sections if you intend to make a plugin. The entire section on plugin development is useful. It starts [here](https://vcvrack.com/manual/PluginDevelopmentTutorial).

To build a plugin, you must have either the VCV Plugin SDK installed correctly, or you must first build VCV itself from scratch. We always build it ourselves.

## Tools needed to build DEMO

You must have an appropriate set of build tools installed. On Windows that means the MSYS2 shell. Follow the directions linked above to get this all set up.

You must have some sort of text editor, preferably one designed for writing computer code. There are many good, free ones available. We like Visual Studio Code.

## VCO1

VCO1 is a very simple polyphonic VCO. It is not a particularly good one, but the code is very small, and hopefully reasonably readable.

This Module has several reasons to exist:

* It shows how to make a VCV Rack plugin purely in code, without using the normal python plugin creator.
* It shows some very simple algorithms for making VCO waveforms.
* It demonstrates some of the pitfalls of overly simplistic DSP code.

VCO1 produces three different waveforms: sine, saw, and a mutant parabolic saw. Each waveforms comes out on a different jack.

## VCO2

VCO2 functions exactly the same as VCO1, but fixed the problems identified in VCO1.

Initial testing results for VCO1: [link here](./docs/vco1-first.md)

Looking in detail at VCO1's CPU usage: [link here](./docs/vco1-cpu.md)

How to use a spectrum analyzer: [link here](./docs/analyzer.md)

How does Demo VCO1 look on the analyzer? (and introduction to aliasing) [link here](./docs/aliasing.md)

More on measuring aliasing. [link here](./docs/aliasing2.md)