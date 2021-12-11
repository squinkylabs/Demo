/**
 * This file contains the entire implementation of the VCO1 demo module.
 * Although it is possible to implement more than one module in a single file,
 * it is rarely done.
 */

#include "demo-plugin.hpp"

#define OVERSAMPLING_FACTOR 8

/**
 *  Every synth module must have a Module structure.
 *  This is where all the real-time processing code goes.
 */
struct VCOB1Module : Module {
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
        TEST_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    float phaseAccumulatorTest = 0;
    float phaseAdvanceTest = 0;
    float phaseAccumulatorMain = 0;
    float phaseAdvanceMain = 0;

    // int loopCounter = 0;
    int oversampleCounter = 1;

    VCOB1Module() {
        // Your module must call config from its constructor, passing in
        // how many ins, outs, etc... it has.
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(PITCH_PARAM, 0, 10, 4, "Initial Pitch");
    }

    // Every Module has a process function. This is called once every
    // sample, and must service all the inputs and outputs of the module.
    void process(const ProcessArgs& args) override {
#if 0
        // There are usually some thing that don't need to be done every single sample.
        // For example: looking at a knob position. You can some CPU if you do 
        // this less often.
        // Note that doing it like we do here will mean that running audio rate signals into the
        // V/Octave input will sound different and arguably not "correct". We think that's a reasonable
        // trade-off, but if you do this optimization make sure a) that you are aware of the trade-offs
        // in your plugin, and b) that it really is speeding up your code.
        if (loopCounter-- == 0) {
            loopCounter = 3;
            processEvery4Samples(args);
        }

        generateOutput();
#endif
        updateParams(args);
        doOutput();
    }

    void updateParams(const ProcessArgs& args) {
        phaseAdvanceTest = computePhaseAdvance(args.sampleTime);
        phaseAdvanceMain = computePhaseAdvance(args.sampleTime);
    }

    void doOutput() {
        generateBasic();
        generatePartialOversample1();
    }

    void generateBasic() {
        advancePhase( phaseAccumulatorTest, phaseAdvanceTest);
        float sawWave = phaseToSaw(phaseAccumulatorTest);
        outputs[TEST_OUTPUT].setVoltage(sawWave, 0);
    }

    void generatePartialOversample1() {
        float output = 0;
        if (oversampleCounter-- == 0) {
            oversampleCounter = (OVERSAMPLING_FACTOR - 1);

            advancePhase(phaseAccumulatorMain, phaseAdvanceMain);
            output = phaseToSaw(phaseAccumulatorMain);  // * OVERSAMPLING_FACTOR;
        }
        // float sawWave = phaseToSaw(phaseAccumulatorMain);
        outputs[MAIN_OUTPUT].setVoltage(output, 0);
    }

    static void advancePhase(float& accumulator, const float& phaseAdvance) {
        accumulator += phaseAdvance;
        if (accumulator > 1.f) {
            // We limit our phase to the range 0..1
            accumulator -= 1.f;
        }
    }

    static float phaseToSaw(float phase) {
        // return (phase - .5f) * 10;
        float x = phase > .5 ? (1 - phase) : phase;
        return (x - .25) * 10;
    }

    float computePhaseAdvance(float sampleTime) {
        float pitchCV = inputs[CV_INPUT].getVoltage(0);
        float pitchParam = params[PITCH_PARAM].value;
        float combinedPitch = pitchParam + pitchCV - 4.f;

        const float q = float(std::log2(261.626));  // move up to C
        combinedPitch += q;

        // combined pitch is in volts. Now use the pow function
        // to convert that to a pitch. Not: there are more efficient ways
        // to do this than use std::pow.
        const float freq = std::pow(2.f, combinedPitch);

        // figure out how much to add to our ramp every cycle
        // to make a saw at the desired frequency.
        const float normalizedFreq = sampleTime * freq;
        return normalizedFreq;
    }
};

/**
 * At least in VCV 1.0, every module must have a Widget, too.
 * The widget provides the user interface for a module.
 * Widgets may draw to the screen, get mouse and keyboard input, etc...
 * Widgets cannot actually process or generate audio.
 */
struct VCOB1Widget : ModuleWidget {
    VCOB1Widget(VCOB1Module* module) {
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
        //float paraY = 290;
        float labelAbove = 20;

        // Now we place the widgets that represent the inputs, outputs, controls,
        // and lights for the module. VCO1 does not have any lights, but does have
        // the other widgets.

        addInput(createInput<PJ301MPort>(Vec(x, inputY), module, VCOB1Module::CV_INPUT));
        addParam(createParam<RoundBlackKnob>(Vec(x - 4, knobY), module, VCOB1Module::PITCH_PARAM));
        addOutput(createOutput<PJ301MPort>(Vec(x, sawY), module, VCOB1Module::MAIN_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, sinY), module, VCOB1Module::TEST_OUTPUT));

        // Add some quick hack labels to the panel.
        addLabel(Vec(20, headingY), "Demo VCO B");
        addLabel(Vec(x - 16, inputY - labelAbove), "Pitch CV");
        addLabel(Vec(x - 10, knobY - labelAbove), "Pitch");
        addLabel(Vec(x - 16, sawY - labelAbove), "Main Out");
        addLabel(Vec(x - 16, sinY - labelAbove), "Test Out");
    }

    // Simple helper function to add test labels to the panel.
    // In a real module you would draw this on the panel itself.
    // Labels are fine for hacking, but they are discouraged for real use.
    // Some of the problems are that they don't draw particularly efficiently,
    // and they don't give as much control as putting them into the panel SVG.
    Label* addLabel(const Vec& v, const std::string& str) {
        NVGcolor black = nvgRGB(0, 0, 0);
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
// I'm called "demo-vco1", my module is VCOB1Module, and my Widget is VCOB1Widget.
// In effect, it implements a module factory.
Model* modelVCOB1 = createModel<VCOB1Module, VCOB1Widget>("demo-vcob1");