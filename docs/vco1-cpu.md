# CPU usage of Demo VCO1

It's always a good thing if a VCV Rack module uses as little CPU as possible. VCV users always want to make bigger and bigger patches, and not all VCV users have the latest, fastest computers. If you make a module that uses "too much" CPU, it may not be as useful as you were hoping.

But how much is "too much" CPU, and how do you measure it?

As to what is an appropriate amount of CPU to use, one easy thing to do is pick a "comparable" VCV module and compare yours to that. Luckily for this project there are plenty of [VCOs in the VCV Rack library](https://library.vcvrack.com/?tag=Oscillator) (a "VCO" is a [Voltage Controlled Oscillator](https://en.wikipedia.org/wiki/Voltage-controlled_oscillator)), so it's not too difficult to identify a comparable one. If your module is completely unlike any other, it may be more difficult.

As far as measuring - an easy and obvious way is to use VCV Rack's CPU meters. These aren't really accurate enough to see a small difference like 5%, but they are quite good for a quick comparison of different modules.

So, let's compare a monophonic patch with several different VCOs: [VCV Fundamental VCO-1](https://library.vcvrack.com/Fundamental/VCO), [Bogaudio VCO](https://library.vcvrack.com/Bogaudio/Bogaudio-VCO), [Bogaudio FM-Op](https://library.vcvrack.com/Bogaudio/Bogaudio-FMOp), and our Demo VCO1 Sin and Demo VCO1 Saw.

First a note about the other VCOs. They were chosen for comparison because they are very high quality, they don't use a lot of CPU, and they are widely used. They all have many, many more features than our Demo VCO1, and those features can and often do require more CPU to implement. Different plugins take very different approaches to conserving CPU usage - some may not use CPU for features that aren't in use, others might just do everything very efficiently.

As all the VCOs are open source you're free to look at their code and see how they work. Depending on the licensing, you may even be able to reuse code from them in your own plugins. (Remember to read the license first!) This can be very useful to help you learn and improve your own modules.

One other thing. You may notice that the CPU usage numbers are in general pretty small. Here at Squinky Labs we don't buy new computers very often, but Microsoft's recent discontinuation of support for Windows 7 forced us to buy a new computer for the first time in perhaps a decade. So we now have a current CPU. Not the fastest in the world or anything, but probably faster than what most VCV users have, which is why the CPU usage numbers shown here may be lower than what shows on your computer.

OK, back to our analysis of CPU usage.

The example patch in the screen shot below is using the [VCV Fundamental Merge](https://library.vcvrack.com/Fundamental/Merge) module to generate a known CV input with one channel. This CV goes to all the VCOs. An output from each VCO goes to a mixer, and then to the audio output. So it's a realistic patch where everything is doing some work. And, of course, the CPU meters are on. (To toggle the display of the CPU meters in VCV Rack, go to the `Engine` menu, then turn on `CPU meter`, or just press the `F3` key.)

![Monophonic CPU usage test patch](./cpu-usage-1.png)

Looking at the CPU meter readings in the screen shot above, both of the Bogaudio VCOs and our Demo VCO1's Saw are using a very low 0.1%, whereas Demo VCO1's Sin and the VCV Fundamental VCO-1 are both using considerably more CPU at 0.3%. Of course, as we mentioned before, VCV Fundamental VCO-1 has many other tricks up its sleeve. However, from this simple test, things aren't looking so good for our Demo VCO1 - it uses as much or more CPU than the others, while offering many fewer options.

Now let's use the same patch, but this time with eight polyphonic channels active in VCV Rack:

![Polyphonic CPU usage test patch](./cpu-usage-8.png)

Something interesting has happened here. Now the VCV Fundamental VCO-1 shows the best for CPU usage at 0.6%, both Bogaudios and Demo VCO1 Saw are pretty good at around 0.8%, but the Demo VCO1 Sin is terrible at 2.8%.

Why has VCV Fundamental VCO-1 gone from OK CPU usage as a monophonic VCO to excellent usage with eight polyphonic channels? It's because it uses the SIMD instructions of the Intel CPU to generate four channels at once. So, as might be expected, it only takes around twice as much CPU for VCV Fundamental VCO-1 to generate eight channels (two sets of four channels) as to generate one channel. The other VCOs are acting as expected without this optimization - with eight channels they use much more CPU than just one.

And then there is our Demo VCO1. It has very bad CPU usage generating a sine waveform, OK usage generating a sawtooth waveform, and no interesting features to distinguish itself from the other VCOs.

The next page discusses [using a frequency analyzer](./analyzer.md).
