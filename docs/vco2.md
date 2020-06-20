# Introducing Demo VCO2

So far we have seen that Demo VCO1 pretty much works, but has two terrible flaws:

* It uses way too much CPU to generate a sin wave.
* The sawtooth output has way too much aliasing and other "digital nasties".

Along the way we have learned how to measure these things - knowledge that is very useful when evaluating which modules you might want to invest your time in.

There are very command and straight forward ways to fix these things, and VCV provides the tools to make it easy for a module developer.

First the easy one - the CPU usage. There are two immediate "red flags" in the VCO1 source code, and they are both cases where we called the standard C++ functions. In general you are playing with fire calling any standard functions from your time sensitive code.

You may wonder - "why should that be true"? Why, after all, would today's compilers and runtimes provide bad implementations of these functions, and how can we do better? The answer is, the standard implementations are not bad, they are very, very good. But they have different requirements than VCV:

* They are required by the C++ spec (and common sense) to be extremely precise.
* They are required to accept any valid number as an input.
* Probably some other things I'm not thinking of.

With this in mind, it's pretty clear that using std::sin() to shape our sine wave is going to be using up a lot of CPU. If we didn't know that, and merely suspected it, we could just remove that function and run the test again. But here it is obvious, because the only difference between our sawtooth and our sinewave is that one function call.

What are our requirements?

* We will be sending it only numbers between 0 and 1, where 1 represents 2 * pi in radians.
* It needs to be accurate enough that the resulting sine is "pure enough".

In this case we don't have to to agonize over this, or do our own math, because we can just use the code from Fundamental VCO-1.
We can see from the documentation that the function domain is 0..1 like we want, and the sine purity of VCO-1 seems fine.

While we are at it, we can see that where we convert the V/Octave into a linear frequency we use std::pow. Now, two things about this:

* pow is usually not as slow as sin.
* As a "pre optimization" we are already only doing this calculation every 4 samples.

None the less, VCV provides a faster function just for this purpose, so we might as well use it.

Later we will talk about alias reduction, but now let's look at the CPU usage.

![VCO2 CPU](./vco-2-cpu.png)

This time we increased the patch polyphony to 16, to make the number large enough to read.

The first surprising thing - almost shocking - it that VCO2's sine is almost 15 X more efficient now!

Another surprise is that VCO2 sin is at least three times more efficient that the other VCOs. Those other VCOs do a lot of things, but at least it shows that if you want to write a dedicated VCO that just does one thing really well, it's usually possible, and not even particularly difficult.

Now let's look address the aliasing. There are, broadly speaking, four well know way to deal with aliasing, all of them used in some VCV modules:

* Oversampling
* minBlep
* Something else
* Do nothing

The first two techniques, oversampling and minBlep, are often used by VCV modules. "Something else" is usually something the will be very specific to the individual module, and probably required a lot of skill and effort. We believe the Vult modules use "something else".

Oversampling can work with any module, but can be CPU intensive and/or less effective than minBlep. Several Squinky Labs modules use it: Shapes (waveshaper), Stairway (filter), and Functional VCO-1.

MinBlep is perfect for our sawtooth. Minblep is commonly used for digital emulation of "classic analog" wave-forms like Saw, Square, Triangle, Hard-synced sine, etc...

MinBlep stands for "minimum phase band limited step". It makes use of the handy fact that waveforms like sawtooth really only have bad problems when they jump suddenly. MinBlep involves calculating what the analog signal "would have been" in only those regions where there is a step. Or something that can be mathematically transformed into a step, like a triangle.

minBlep:

* Is very effective at reducing aliasing and phase jitter.
* Is quite math intensive.
* Requires at least a mid-level of DSP learning to understand.
* Cannot be used to reduce aliasing for thru-zero-FM, or other waveforms that don't fit its model.
* almost no CPU at low frequencies, but the CPU load goes up linearly with frequency.

Luckily for plugin developers, the VCV SDK ships with a very good implementation of minBlep, eliminating the need for a masters degree in DSP. It can still be tricky to apply it correctly, but copying VCV's VCO-1 and a bit of trial and error can make it work. For Demo VCO2 we took the minBlep code from VCV's Fundamental VCO-1 and modified it very little. In fact we modified Demo VCO2 to be more like Fundamental just to make it easier to steal the minBlep code. (and, remember, it isn't stealing, it's legal and encouraged by the open source license. Just make sure you actually read the license and abide by the terms).

So, here is what the aliasing looks like now on VCO2
![VCO2 ALIAS](./vco-2-alias.png)

Not surprisingly, it looks exactly like VCV's VCO. Which is should, since it's the same code. You will also notice that the small DC offset that we previously saw in VCO-1 has now crept into our VCO. Is must be a small flaw in the minBlep library.

The analyzer also shows that we succeeded in getting rid of the aliasing from our parabolic VCO with the same minBlep. Disappointingly, the analyzer also suggests that it won't sound much different that a straight saw. We included it because:

* You can see that minBlep can be used with almost any waveform that consists of mostly smooth curves and abrupt steps.
* It allowed us to do a "clever" trick to reduce CPU usage. Both the sawtooth and the parabola use the same minBlep, rather than using two, one for each.

minBlep is a large topic. Of course we can't cover all of it here. Here are some links to some of the more commonly sited papers. Of course being university research they are very heavy on the math and DSP.

This one covers a [bunch of different techniques](https://ccrma.stanford.edu/~stilti/papers/blit.pdf)

This one [focuses on minBlep](http://www.cs.cmu.edu/~eli/papers/icmc01-hardsync.pdf)