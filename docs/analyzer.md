# Using a frequency analyzer

This may be fairly basic, so feel free to skip ahead.

A frequency analyzer is quite often the best way to visually examine your audio. It will tell you the volume of the audio at specific frequencies. Often this provides insights that would be impossible to discern by looking at the waveform on a scope.

The [Bogaudio Analyzer-XL](https://library.vcvrack.com/Bogaudio/Bogaudio-AnalyzerXL) is a very nice implementation, so we will use that here.

An analyzer will draw a real time graph, where the x axis (going right) is frequency, and the y axis (going up) is amount, or volume, in [decibels](https://en.wikipedia.org/wiki/Decibel) (dB).

The Bogaudio Analyzer-XL has a lot of settings, which you access by right-clicking on the module. Many of the default settings are appropriate for this sensitive measuring, but not all of them:

* Quality: This defaults to "Good", but set it to "Ultra". Very important.
* Window: The default Kaiser window is a good choice.
* Amplitude plot: The default -60 dB range is good. That's what we used here.
* Frequency range: We use the default "Full".

Bogaudio Analyzer-XL has other settings that may be useful, so check them all out.

Our choice here to use the -60 dB amplitude range is motivated by a couple of factors:

* It's easier to see the important details in this "zoomed in" view.
* All of the VCOs here have some amount of "gunk" at extremely low audio levels. We believe this gunk is completely inaudible (that is, below a level of -60 dB), and so we didn't want to get distracted by it.

The screen shot below shows what the [white noise](https://en.wikipedia.org/wiki/White_noise) output from the [VCV Fundamental Noise](https://library.vcvrack.com/Fundamental/Noise) module looks like in the Bogaudio Analyzer-XL's display:

![The Bogaudio Analyzer-XL displaying a white noise signal](./fft-noise.png)

Notice that, while it is a little jagged, the height of the graph is more or less the same at all frequencies. As white noise is a random signal having equal intensity at different frequencies, that is what it should look like.

For contrast, here is what a [sine wave](https://en.wikipedia.org/wiki/Sine_wave) output from the [VCV Fundamental VCO-1](https://library.vcvrack.com/Fundamental/VCO) module looks like in the display:

![The Bogaudio Analyzer-XL displaying a 1 kHz sine wave signal](./vco-1-sin.png)

One of the characteristics of a sine wave is that it produces a pure tone. So, as expected, the analyzer displays no level at all at most frequencies, and a nice sharp line near 1 kHz (one kilohertz, or a thousand [hertz](https://en.wikipedia.org/wiki/Hertz)), which is the frequency of the sine wave being produced by the VCV Fundamental VCO-1.

A periodic waveform that isn't a sine will also have levels at some of its harmonics. A square wave will only have "odd" harmonics, whereas a sawtooth will have all harmonics. For both of them the harmonics extend all the way to infinity, although the level falls off at 6db per octave.

A sawtooth at about 1 kHz from the VCV Fundamental VCO-1 looks like this:

![The Bogaudio Analyzer-XL displaying a 1 kHz sawtooth wave signal](./fft-vco-1-saw.png)

Here we see mostly what we should see. The largest line is very close to 1 kHz. It's the fundamental. And we can see that there are harmonics at 2 kHz, 3 kHz, 4 kHz, repeating every 1 kHz to infinity. Looking at the display, we can see that the level of the harmonics falls about 6 dB every octave.

There are some very minor anomalies, too. All the way to the left, from zero to around 60 Hz, there is a "hump". This tells us there is a small amount of [DC offset](https://en.wikipedia.org/wiki/DC_bias) at the output.

Also we can see that above 10 kHz the harmonics fall off a little faster, and there are some strange signals that are not harmonics in between 10 and 20 kHz. But this "junk" is more than 30 dB below the fundamental, at a very high frequency where we can't hear so well. This distortion (actually called [aliasing](https://en.wikipedia.org/wiki/Aliasing)) is probably well below the level of audibility.

Next, we use the analyzer to look at [the aliasing in Demo VCO1](./aliasing.md).
