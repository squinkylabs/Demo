
#include "demo-plugin.hpp"
 
//using Module = rack::plugin::Module;


struct VCO1Module : Module
{
    enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
    };

    VCO1Module() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		//for (int i = 0; i < 8; i++) {
		//	configParam(GAIN_PARAMS + i, -1.f, 1.f, 0.f, string::f("Ch %d gain", i + 1), "%", 0, 100);
		//}
	}
};

struct VCO1Widget : ModuleWidget {
    VCO1Widget(VCO1Module* module) {
        
    }
};


Model* modelVCO1 = createModel<VCO1Module, VCO1Widget>("demo-vco1");