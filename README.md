# Demo

This repo contains a VCV Rack plugin which implements one (or more) VCV RAck synthesizer modules. It is intended more as a demonstration for how to make VCV plugins than an actual useful synthesizer module.

## Tools needed to build DEMO

You must have the Msys2 shell installed, as per the VCV Rack manual.

You must have some sort of text editor, preferably one designed for writing computer code. There are many good, free ones available. We like Visual Studio Code.

## VCO1

VCO1 is a very simple polyphonic VCO. It is not a particularly good one, but the code is very small, and hopefully reasonably readable.

This Module has several reasons to exist:

* It shows how to make a VCV Rack plugin purely in code, without using the normal python plugin creator.
* It shows some very simple algorithm for making VCO waveforms.
* It shows some of the pitfalls of overly simplistic DSP code.
