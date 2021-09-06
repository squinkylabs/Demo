# Host delay

## Does host delay

Of course it does. The “block size” menu tells you how much the output will be delayed relative to the input. You can easily measure this to verify it. Here we are running a low frequency square wave through Host, session is set to 44.1kzh sample rate. Host has loaded a VST EQ that is set to bypass. Host is set for the default 128x2  (5.8 ms) block size. In this image you can see that the measured delay is quite close to what VCV claims.

Do the delays add up?
Of course they do. Here is the same patch with three Hosts in series. Again the measured value is pretty close to three times the value for an individual host instance.
Does running host in parallel cause comb filtering?
Of course it does. Mixing un-delayed audio in VCV Rack with the delayed output of Host will cause comb filtering. This is, in fact, exactly how a flanger works. It is well known that mixing delayed and un-delayed signals will cause a series of infinitely deep notch filters going up from the “critical frequency” of the delay. This frequency will get lower and lower as the delay gets longer and longer.
Here you can see a square wave against a square wave added to itself after going though host. In this example the buffer size is set to the smallest possible, 8 x 2.

Doubling the buffer size to 16 causes the notch to move down an octave, as expected.
When is delay noticeable?
Ah, the answer to that is very complicated! It depends on the nature of the sound, if you running signals in parallel, how closely you are listening, whether comb filtering sounds good or bad on signal.
Regular VI
If you put a virtual instrument into Host, will they delay matter? Often it won’t matter much. If it’s a string pad with a slow attack, you are unlikely to notice a few tens of milliseconds of delay. So Host running at the default latency will be fine in this application.
If the VI is making a very rhythmic sound, like a piano, and is very “exposed” in the mix then perhaps 10 milliseconds (or even more) is a safe delay before you might notice the timing getting “odd”.
If the sound is a drum and you are mixing it with other sounds in VCV that occur at “exactly” the same time, then it might be possible to notice just a few milliseconds of delay causing your to “drums” to “flam” slightly. Whether a one or two milliseconds of delay on a drum is audible if going to depend on a lot of factors – quite often it is totally un noticeable. But delays of 10 milliseconds or more will start to produce obvious flaming. 
Effects send on a mixer
If you use host on a mixer effect send, the host delay will basically become a “pre delay” on the effect. So if you are using a reverb it’s probably fine. You either won’t hear it, you will hear it and it will sound better or it will sound slightly worse and you may want to decrease the pre-delay in the reverb itself.
If you are using a rhythmic or slap delay on a send you can probably get it to sound right with a short host buffer setting.
Parallel effects
This one just plain will not work without a lot of effort – except as a special effect. We saw, above, that mixing host and non-host audio in parallel is going to make severe comb filtering that will be audible in many/most situations. Of course if you really want to you can add a matching delay to the other side of the chain and get them in sync again.
And a VST that does parallel processing itself won’t have the problem.
Bottom line
Host is of course a great addition to VCV, and give access to a ton of great sounds. But a careful user should always keep in mind that Host is going to be adding some delay, and either adjust the path/settings, or use Host is situations where the delay is harmless.
My advice would be.
