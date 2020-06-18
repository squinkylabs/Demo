
#include "demo-plugin.hpp"
 
//using Module = rack::plugin::Module;

static const int maxPolyphony = 16;

struct VCO1Module : Module
{
    enum ParamIds {
        PITCH_PARAM,
        NUM_PARAMS
	};
	enum InputIds {
        CV_INPUT,
        NUM_INPUTS
	};
	enum OutputIds {
        MAIN_OUTPUT,
        NUM_OUTPUTS
	};
	enum LightIds {
        NUM_LIGHTS
    };

   

    VCO1Module() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(PITCH_PARAM, 0, 10, 4, "Initial Pitch");
    }

    void process(const ProcessArgs& args) override {
        if (loopCounter-- == 0) {
            loopCounter = 3;
            processEvery4Samples();
        }
    }

    void processEvery4Samples() {
        currentPolyphony = std::max(1, inputs[CV_INPUT].getChannels());
        outputs[MAIN_OUTPUT].setChannels(currentPolyphony);
    }

    float phaseAccumulators[maxPolyphony] = {0};
    float phaseAdvance[maxPolyphony] = {0};
    int currentPolyphony = 1;
    int loopCounter = 0;
};

struct VCO1Widget : ModuleWidget {
    VCO1Widget(VCO1Module* module) {
        setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/vco1_panel.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

        float x = 30;
        addInput(createInput<PJ301MPort>(Vec(x, 50), module, VCO1Module::CV_INPUT));

        addParam(createParam<RoundBlackKnob>(Vec(x-4, 90), module, VCO1Module::PITCH_PARAM));
        
        addOutput(createOutput<PJ301MPort>(Vec(x, 140), module, VCO1Module::MAIN_OUTPUT));
    }
};

Model* modelVCO1 = createModel<VCO1Module, VCO1Widget>("demo-vco1");