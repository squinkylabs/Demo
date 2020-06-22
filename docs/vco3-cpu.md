# CPU usage of Demo VCO3

Now that the CPU usage is getting so low, we turn the sample rate up much higher so we can generate some bigger numbers from the CPU meters. In our experience, these numbers can fluctuate a bit, sometimes reading more CPU, sometimes less, depending on the patch.

But for sure we can see that converting our VCO to use SIMD instructions has made it very efficient in the 16 voice case. And by tuning the sine approximation we have gotten the CPU usage of or sine really low:

![VCO3 CPU](./vco-3-cpu.png)

A few things are clear from this screen shot:

* our CPU usage has gone from pretty poor in Demo VCO1, to very good in Demo VCO3.
* Demo VCO3 now seems to be quite a bit more efficient than the "comparables".

One thing doesn't look right here, and that is the improvement in the sawtooth performance from Demo VCO1 to Demo VCO3. There's no way it could have increased like that. It must be some anomaly in the test setup.
