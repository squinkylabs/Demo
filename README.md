# Demo

This GitHub repo contains a variety of documents and source code that will hopefully be useful or interesting to plugin developers and curious users. Much of it can be read online right here. The source code only needs to be downloaded ("cloned", in git-speak) if you wish to build or modify any of the code.

While this repo is created, owned, and maintained by SquinkyLabs, there is little here that is about SquinkyLabs modules, and no information that is specific to SquinkyLabs. It is intended to be general.

The "main attraction" here is a [VCV Rack](https://vcvrack.com) plugin which implements a few VCV Rack synthesizer modules. It's intended to demonstrate how to make a basic VCV Rack plugin rather than an actual "useful" synthesizer module. It can be found [here](./demo-readme.md). The documentation that accompanies it talks a lot about aliasing, and how to get rid of it in a VCO. It also talks about methods to reduce CPU usage.

It is worth noting that the last demo in this series, VCO3, uses much less CPU that other comparable VCOs. SquinkyLabs based the BasicVCO module on this demo.

The Demo VCO modules are for VCV 2.0, the 1.0 compatible demos are no longer available.

The other things here are some papers on different aspects of using VCV and measuring the performance of VCV.

We hope to keep general information in this repo, instead of putting it into the main SquinkyLabs VCV Module repo.

New: There are two new modules, FILTER1 and SHAPER1. They mainly show how to use oversampling in VCV.

## Table of contents

1. [The demo VCOs](./demo-readme.md)
2. [The oversampled distortion and filter](./docs/oversample-readme.md)
3. [Writing efficient plugins](./docs/efficient-plugins.md)
4. [DC on module outputs](./docs/dc.md)
5. [Measuring aliasing. Deep link into demo VCO documentation](./docs/aliasing2.md)
6. [Comparing FM and Phase Modulation](./docs/fm.md)
7. [Portable Sequence Format for clipboard](./docs/clipboard-format.md)
8. [Effect of delays using Host plugin](./docs/host-delay.md)
9. [Using the CPU meters in VCV Rack](./docs/cpu-meters.md)
10. [Plugin Licensing Issues](./docs/license-issues.md)
