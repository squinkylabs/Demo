# First test of Demo VCO1

## Examine on scope

Since Demo VCO1 will always generate at least one channel of output, we can look at that output with the [VCV Fundamental Scope](https://library.vcvrack.com/Fundamental/Scope) without needing to make a patch which outputs audio.

Let's look at the different waveforms using Scope:

![Each of the three different waveforms output by Demo VCO1 being displayed in three VCV Fundamental Scope modules](./scopes.png)

We can see some good things here. The basic shapes of all the waveforms look correct, with no obvious gaps, jumps, or other strangeness. The output voltage swings between +5V and -5V as desired (except the crazy parabolic - more on that later).

Now let's check out the tuning. First - with no input (zero volts) the VCV spec says we should put out a frequency which is equivalent to a musical note at a pitch of C4, also known as [Middle C](https://en.wikipedia.org/wiki/C_(musical_note)#Middle_C).

Here we are using [NYSTHI Hot Tuna](https://library.vcvrack.com/NYSTHI/HotTuna) to check the pitch:

![Initial tuning check of Demo VCO1's sine wave using NYSTHI Hot Tuna](./tuner-1.png)

Now, let's see if Demo VCO1's sine wave tracks at 1V/Octave like it's supposed to. Here we again use Hot Tuna to check the pitch, [VCV MIDI-CV](https://library.vcvrack.com/Core/MIDIToCVInterface) to generate precise voltages, and [NYSTHI MultiVoltimetro](https://library.vcvrack.com/NYSTHI/MultiVoltimetro) to display the voltage.

![Negative 2 volts showing a pitch of note C2](./tuner-c2.png)

![Negative 1 volt showing a pitch of note C3](./tuner-c3.png)

We see that the pitch changes by an octave when the CV changes by a volt.

So far, it looks like Demo VCO1 is working perfectly, but in our next session we will dig a little deeper and find some pretty bad flaws in how it works.

The next page discusses [CPU consumption](./vco1-cpu.md).
