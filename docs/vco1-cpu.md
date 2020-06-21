# CPU usage of Demo VCO1

It's always a good thing if a module uses as little CPU as possible. VCV users always want to make bigger and bigger patches, and not all VCV users have the latest, fastest computers. If you make a module that uses "too much" CPU, it may not be as useful as you are hoping.

But how much is "too much" CPU, and how do you measure it?

As to what is an appropriate amount of CPU to use, one easy thing to do is pick a "comparable" VCV module and compare yours to that. Luckily for this project there are plenty of VCOs out there, so it's not too difficult to identify a comparable one. If your module is completely unlike any other it may be more difficult.

As far as measuring - an easy and obvious way is to use VCV's CPU meters. These aren't really accurate enough to see a small difference like 5%, but they are quite good for a quick comparison of different modules.

So, let's compare a monophonic patch with several different VCOs: The VCV Fundamental VCO-1, Bogaudio VCO, Bogaudio FM-OP, Demo VCO1 sin, Demo VCO1 saw.

First a note about these VCOs. They were chosen because they are very high quality, they don't use a lot of CPU, and they are widely used. Of course they all have many, many more features than the Demo VCO1. These features can and often do require more CPU to implement. Different plugins take very different approaches to conserving CPU usage, so some may not use CPU for features that aren't in use, others don't use that trick, they just do everything very efficiently. These VCOs are also all open source, so you are free to look inside and see how they work. Also, please read the license first, but these plugins all appear to grant you the right to re-use code from them. This can be very useful!

One other thing. You may notice the CPU usage number are in general pretty small. Here at Squinky Labs we don't buy new computers very often. Recently Microsoft's discontinuation of support for Windows-7 forced us to buy a new computer for the first time in perhaps a decade. So we now have a current CPU. Not the fastest in the world or anything, but probably faster than what most VCV users have.

As you can see in the screenshot below, the patch is using the VCV Merge module to generate a known CV input with one channel. This CV goes to all the VCOs. An output from each VCO goes to a mixer, and then to the audio card. So it's a realistic patch where everything is doing some work. And, of course, the CPU meters are on. (To toggle the display of the CPU meters, click the `Engine` menu, then `CPU meter`, or press the `F3` key.)

![Monophonic CPU usage](./cpu-usage-1.png)

Looking at the CPU meter readings, it looks like both Bogaudio VCOs, and Demo Saw use a very low 0.1%. Demo Sin and VCO-1 both use considerably more. Of course as we mentioned before VCO-1 has many other tricks up its sleeve. From this simple test, things aren't looking so good for our Demo VCO1: it uses as much or more CPU than the others, while offering many fewer options.

Now let's use the same patch, but this time with 8 polyphonic channels active:

![Polyphonic CPU usage](./cpu-usage-8.png)

Something interesting has happened here. Now the Fundamental VCO-1 shows the best for CPU usage, both Bogaudios and Demo Saw are pretty good, and Demo Sin is terrible.

Why has Fundamental VCO-1 gone from OK CPU usage as a monophonic VCO, too excellent polyphonic? It's because it uses the SIMD instructions of the Intel CPU to generate four voices at once. So, as might be expected, it only takes twice as much CPU for VCO-1 to generate 8 voices as to generate 1. The other VCOs are acting as expected. With 8 voices they take much more CPU than 1.

And then there is Demo VCO1. Very bad CPU usage generating sine, ok generating saw, but no interesting features to distinguish itself.

The next page discusses [using a spectrum analyzer](./analyzer.md).
