# FM goes out of tune with DC on the modulator



In this patch we have:

* A Fundamental VCO on the first row. This will be our modulator.
* A Fundamental VCO on the second row. This will be our FM VCO.
* A Bogaudio FM-OP on the third row. This will be our PM VCO.

Beside each VCO is a tuner showing the pitch, and a spectrum analyzer showing the VCO spectrum.

This patch has everything at the default, so all three VCOs are at middle C, and the modulation amount to the FM and PM VCOs is zero, so there is no FM or PM.

![patch at middle C](./fm-middle-c.png)

Looking at the tuners, you can see they are all at C4 like the should be.

There are a couple of things on the spectrum analyzer that may seem strange:

* The sawtooth wave on top is showing a bit of DC on the output. This is normal for Fundamental VCO.
* The middle sine wave has some visible odd harmonics. This is the analog emulation of Fundamental VCO. I puts out a slightly distorted sin, much like a typical analog VCO would.

The DC on the saw output is, in fact, a bug in Fundamental VCO, and is logged here: https://github.com/VCVRack/Fundamental/issues/111. But we have seen before that small amounts of DC on module outputs are very common. See [this paper](./dc.md) for more info.
