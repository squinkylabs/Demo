/**
 * This file contains the entire implementation of the VCO1 demo module.
 * Although it is possible to implement more than one module in a single file,
 * it is rarely done.
 */

#include "demo-plugin.hpp"


// This is a fast sine approximation function from Fundamental VCO-1
template <typename T>
T sin2pi_pade_05_5_4(T x) {
	x -= 0.5f;
	return (T(-6.283185307) * x + T(33.19863968) * simd::pow(x, 3) - T(32.44191367) * simd::pow(x, 5))
	       / (1 + T(1.296008659) * simd::pow(x, 2) + T(0.7028072946) * simd::pow(x, 4));
}
 
// VCV has a limit of 16 channels in a polyphonic cable.
static const int maxPolyphony = 16;

/**
 *  Every synth module must have a Module structure.
 *  This is where all the real-time processing code goes.
 */
struct VCO2Module : Module
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
        PARA_OUTPUT,
        NUM_OUTPUTS
	};
	enum LightIds {
        NUM_LIGHTS
    };

    float phaseAccumulators[maxPolyphony] = {0};
    float phaseAdvance[maxPolyphony] = {0};
    dsp::MinBlepGenerator<16, 16, float> sawMinBlep[maxPolyphony];
    dsp::MinBlepGenerator<16, 16, float> paraMinBlep[maxPolyphony];
    int currentPolyphony = 1;
    int loopCounter = 0;
    bool outputSaw = false;
    bool outputSin = false;
    bool outputPara = false;

    VCO2Module() {
        // Your module must call config from its constructor, passing in
        // how many ins, outs, etc... it has.
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(PITCH_PARAM, 0, 10, 4, "Initial Pitch");
    }

    // Every Module has a process function. This is called once every
    // sample, and must service all the inputs and outputs of the module.
    void process(const ProcessArgs& args) override {

        // There are usually some thing that don't need to be done every single sample.
        // For example: looking at a knob position. You can save a lot of CPU if you do 
        // this less often.
        if (loopCounter-- == 0) {
            loopCounter = 3;
            processEvery4Samples(args);
        }

        generateOutput();
    }

    void processEvery4Samples(const ProcessArgs& args) {
        // Is is very important that you tell the output ports how
        // many channels the should export. In a VCO it is very common
        // to use the number of CV input channels to automatically determine the
        // polyphony.
        // It is also very common to always run one channel, even if there is
        // no input. This lets the VCO generate output with no input.
        currentPolyphony = std::max(1, inputs[CV_INPUT].getChannels());
        outputs[SIN_OUTPUT].setChannels(currentPolyphony);
        outputs[SAW_OUTPUT].setChannels(currentPolyphony);
        outputs[PARA_OUTPUT].setChannels(currentPolyphony);

        //now we are going to look at our input and parameters and
        // save off some values for out audio processing function.
        outputSaw = outputs[SAW_OUTPUT].isConnected();
        outputSin = outputs[SIN_OUTPUT].isConnected();
        outputPara = outputs[PARA_OUTPUT].isConnected();
        float pitchParam = params[PITCH_PARAM].value;
        for (int i = 0; i < currentPolyphony; ++i) {
            float pitchCV = inputs[CV_INPUT].getVoltage(i);
            float combinedPitch = pitchParam + pitchCV - 4.f;

            const float q = float(log2(261.626));       // move up to C
            combinedPitch += q;

            // combined pitch is in volts. Now use the pow function
            // to convert that to a pitch. Not: there are more efficient ways
            // to do this than use std::pow.
           // const float freq = std::pow(2.f, combinedPitch);
            const float freq = rack::dsp::approxExp2_taylor5(combinedPitch);

            // figure out how much to add to our ramp every cycle 
            // to make a saw at the desired frequency.
            const float normalizedFreq = args.sampleTime * freq;
            phaseAdvance[i] = normalizedFreq;
        }
    }

    void generateOutput() {
        for (int i = 0; i < currentPolyphony; ++i) {
            // Every sample, we advance the phase of our ramp by the amount
            // we derived from the CV and knob inputs.
            phaseAccumulators[i] += phaseAdvance[i];
            if (phaseAccumulators[i] > 1.f) {
                // We limit our phase to the range 0..1
                phaseAccumulators[i] -= 1.f;
            }

            // Let's change our saw so that it has it's step when phase = .5.
            // VCO used the phase as the saw, so wrapped at 0/1
            float halfCrossing = (0.5f - (phaseAccumulators[i] -  phaseAdvance[i])) /  phaseAdvance[i];
            bool jump = ((0 < halfCrossing) & (halfCrossing <= 1.f));
            if (jump) {
                float jumpPhase = halfCrossing - 1.f;
                float jumpAmount = -2;
                sawMinBlep[i].insertDiscontinuity(jumpPhase, jumpAmount);
            }

            // rawSaw will go -1..1 and have the minBlep in it
            float minBlep;
            if (outputSaw || outputPara) {
                minBlep = sawMinBlep[i].process();
            }

            if (outputSaw) {
                float rawSaw =  (phaseAccumulators[i] + .5f);
                rawSaw -= std::trunc(rawSaw);
                rawSaw = 2 * rawSaw - 1;
                rawSaw += minBlep;
                float sawWave = 5 * rawSaw;
                outputs[SAW_OUTPUT].setVoltage(sawWave, i);
            }

            if (outputPara) {
                // This simple "parabolic ramp" is an example of a way one could try to 
                // make the sawtooth sound a little different.

                float paraWave = (phaseAccumulators[i] + .5f);
                paraWave -= std::trunc(paraWave);       // now 0 ... 1
                paraWave *= paraWave;                  // squared, but still 0..1
                paraWave = 2 * paraWave; // now 0..2
                paraWave += minBlep;
                paraWave -= .33f * 2;       // subtract out the DC component (use your calculus or trial and error).
                paraWave *= 5;
                outputs[PARA_OUTPUT].setVoltage(paraWave, i);
            }

            if (outputSin) {
                // If the sin output it patched, turn our 0..1 ramp
                // into a -5..+5 sine.
                float sinWave = 5.f * sin2pi_pade_05_5_4( phaseAccumulators[i]);
                outputs[SIN_OUTPUT].setVoltage(sinWave, i);
            }
        }
    }
};

/**
 * At least in VCV 1.0, every module must have a Widget, too.
 * The widget provides the user interface for a module.
 * Widgets may draw to the screen, get mouse and keyboard input, etc...
 * Widgets cannot actually process or generate audio.
 */
struct VCO2Widget : ModuleWidget {
    VCO2Widget(VCO2Module* module) {
        // The widget always retains a reference to the module.
        // you must call this function first in your widget constructor.
        setModule(module);

        // Typically the panel graphic is added first, then the other 
        // UI elements are placed on TOP.
        // In VCV the Z-order of added children is such that later
        // children are always higher than children added earlier.
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/vco1_panel.svg")));

        // VCV modules usually have image is "screws" to make them
        // look more like physical module. You may design your own screws, 
        // or not use screws at all.
		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

        // It's purely personal style whether you want to set variables like this
        // For the position of your widgets. It's fine to do it inline also.
        float x = 50;
        float headingY = 20;
        float inputY = 75;
        float knobY = 130;
        float sawY = 190;
        float sinY = 240;
        float paraY = 290;
        float labelAbove = 20;

        // Now we place the widgets that represent the inputs, outputs, controls,
        // and lights for the module. VCO2 does not have any lights, but does have
        // the other widgets.

        addInput(createInput<PJ301MPort>(Vec(x, inputY), module, VCO2Module::CV_INPUT));
        addParam(createParam<RoundBlackKnob>(Vec(x-4, knobY), module, VCO2Module::PITCH_PARAM));
        addOutput(createOutput<PJ301MPort>(Vec(x, sawY), module, VCO2Module::SAW_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, sinY), module, VCO2Module::SIN_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, paraY), module, VCO2Module::PARA_OUTPUT));
    
        // Add some quick hack labels to the panel.
        addLabel(Vec(20, headingY), "DEMO VCO2");
        addLabel(Vec(x-16, inputY - labelAbove), "Pitch CV");
        addLabel(Vec(x-10, knobY - labelAbove), "Pitch");
        addLabel(Vec(x-16, sawY - labelAbove), "Saw Out");
        addLabel(Vec(x-16, sinY - labelAbove), "Sin Out");
        addLabel(Vec(x-16, paraY - labelAbove), "Para Out");
    }

    // Simple helper function to add test labels to the panel.
    // In a real module you would draw this on the panel itself.
    // Labels are fine for hacking, but they are discouraged for real use.
    // Some of the problems are that they don't draw particularly efficiently,
    // and they don't give as much control as putting them into the panel SVG.
    Label* addLabel(const Vec& v, const char* str)
    {
        NVGcolor black = nvgRGB(0,0,0);
        Label* label = new Label();
        label->box.pos = v;
        label->text = str;
        label->color = black;
        addChild(label);
        return label;
    }
};

// This mysterious line must appear for each module. The
// name in quotes at then end is the same string that will be in 
// plugin.json in the entry for corresponding plugin.

// This line basically tells VCV Rack:
// I'm called "demo-vco2", my module is VCO2Module, and my Widget is VCO2Widget.
// In effect, it implements a module factory.
Model* modelVCO2 = createModel<VCO2Module, VCO2Widget>("demo-vco2");