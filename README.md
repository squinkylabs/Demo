# Demo

This GitHub repo contains a variety of documents and source code that will hopefully be useful or interesting to plugin developers and curious users. Much of it can be read online right here. The source code only needs to be downloaded ("cloned", it git-speak) if you wish to build or modify any of the code.

While this repo is created, owned, and maintained by Squinky Labs, there is little here that is about SquinkyLabs modules, and no information that is specific to SquinkyLabs. It is intended to be general.

The "main attraction" here is a [VCV Rack](https://vcvrack.com) plugin which implements a few VCV Rack synthesizer modules. It's intended to demonstrate how to make a basic VCV Rack plugin rather than an actual "useful" synthesizer module. It can be found [here](./demo-readme.md). The documentation that accompanies it talks a lot about aliasing, and how to get rid of it in a VCO. It also talks about methods to reduce CPU usage.

It is worth noting that the last demo in this series, VCO3, uses much less CPU that other comparable VCOs. SquinkyLabs based the BasicVCO module on this demo.

As of now, the code for the Demo VCOs is for a VCV 1.x module. It has not yet been ported to VCV 2.0.

The other things here are some papers on different aspects of using VCV and measuring the performance of VCV.

We hope to keep general information in this repo, instead of putting it into the main SquinkyLabs VCV Module repo.

## Table of contents

1. [Plugin Licensing Issues](./docs/license-issues.md)
2. [The demo VCOs](./demo-readme.md)
3. [Using the CPU meters in VCV Rack](./docs/cpu-meters.md)
4. [Effect of delays using Host plugin](./docs/host-delay.md)
5. [Writing efficient plugins](./docs/efficient-plugins.md)
6. [DC on module outputs](./docs/dc.md)
7. [Measuring aliasing. Deep link into demo VCO documentation](./docs/aliasing2.md)
