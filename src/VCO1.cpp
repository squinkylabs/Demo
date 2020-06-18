
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
        SAW_OUTPUT,
        SIN_OUTPUT,
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
            processEvery4Samples(args);
        }
        generateOutput();
    }

    void processEvery4Samples(const ProcessArgs& args) {
        currentPolyphony = std::max(1, inputs[CV_INPUT].getChannels());
        outputs[SIN_OUTPUT].setChannels(currentPolyphony);
        outputs[SAW_OUTPUT].setChannels(currentPolyphony);

        outputSaw = outputs[SAW_OUTPUT].isConnected();
        outputSin = outputs[SIN_OUTPUT].isConnected();

        float pitchParam = params[PITCH_PARAM].value;
        for (int i = 0; i < currentPolyphony; ++i) {
            float pitchCV = inputs[CV_INPUT].getVoltage(i);
            float combinedPitch = pitchParam + pitchCV - 4.f;

            const float q = float(log2(261.626));       // move up to C
            combinedPitch += q;
            const float freq = std::pow(2.f, combinedPitch);

            const float normalizedFreq = args.sampleTime * freq;
            phaseAdvance[i] = normalizedFreq;
        }
    }

    void generateOutput() {
        for (int i = 0; i < currentPolyphony; ++i) {
            phaseAccumulators[i] += phaseAdvance[i];
            if (phaseAccumulators[i] > 1.f) {
                phaseAccumulators[i] -= 1.f;
            }

            if (outputSaw) {
                float sawWave = (phaseAccumulators[i] - .5f) * 10;
                outputs[SAW_OUTPUT].setVoltage(sawWave, i);
            }

            if (outputSin) {
                float radianPhase = phaseAccumulators[i] * 2 * M_PI;
                float sinWave = std::sin(radianPhase) * 5;
                outputs[SIN_OUTPUT].setVoltage(sinWave, i);
            }
        }
    }

    float phaseAccumulators[maxPolyphony] = {0};
    float phaseAdvance[maxPolyphony] = {0};
    int currentPolyphony = 1;
    int loopCounter = 0;
    bool outputSaw = false;
    bool outputSin = false;
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
        addOutput(createOutput<PJ301MPort>(Vec(x, 140), module, VCO1Module::SAW_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, 170), module, VCO1Module::SIN_OUTPUT));
    }
};

Model* modelVCO1 = createModel<VCO1Module, VCO1Widget>("demo-vco1");