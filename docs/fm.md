# FM vs PM

Many synthesizers that claim to be "FM" actually use Phase Modulation to generate their timbres. For example the Yamaha-DX7 is a PM synthesizer. The Bogaudio FM-OP and SquinkyLabs Kitchen-Sink are both PM modules. The VCV Fundamental VCO, on the other hand, uses FM.

In this document, when we say "FM" we always mean through zero liner FM. And when we say "PM" we always mean through zero linear phase modulation.

People who know a bit about DSP or physics know that phase and frequency are closely related by simple calculus:

frequency = change in phase with respect to time ( dp / dp)

phase = integral over time of frequency

This suggests that FM and PM can sound the same if the modulator signal is properly filtered, which is true. But most often these modulators are not filtered, and hence they can sound very different.

There are two things that make PM an attractive choice, rather than FM.

1. With FM, any DC on the modulator input, any at all, will change the tuning of the oscillator.
2. With PM, the ratios of the harmonics stay the same as the oscillator transposes. With FM the high harmonics drop off as the pitch gets higher.

[ people who remember their intro to calculus will realize both of these properties of FM/PM are easily derived from the formulae above that relate frequency and phase ]

The next page shows how [FM can go out of tune](./fm-tune.md).
