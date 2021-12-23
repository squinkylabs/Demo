/**
 * This file contains the entire implementation of the FILTER1 demo module.
 */
#include "demo-plugin.hpp"
#include "Filter6PButter.h"

/**
 *  Every synth module must have a Module structure.
 *  This is where all the real-time processing code goes.
 */
struct FILTER1Module : Module
{
    enum ParamIds {
        NUM_PARAMS
	};
	enum InputIds {
        INPUT,
        NUM_INPUTS
	};
	enum OutputIds {
        OUTPUT,
        NUM_OUTPUTS
	};
	enum LightIds {
        NUM_LIGHTS
    };

    Filter6PButter filter;
    FILTER1Module() {
        // Your module must call config from its constructor, passing in
        // how many ins, outs, etc... it has.
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        //configParam(PITCH_PARAM, 0, 10, 4, "Initial Pitch");
        filter.setCutoffFreq(1.f / 44.1f);
    }

    // Every Module has a process function. This is called once every
    // sample, and must service all the inputs and outputs of the module.
    void process(const ProcessArgs& args) override {

        const float input = inputs[INPUT].value;
        const float output = filter.process(input);
        outputs[OUTPUT].value = output;
    }
};

/**
 * At least in VCV 1.0, every module must have a Widget, too.
 * The widget provides the user interface for a module.
 * Widgets may draw to the screen, get mouse and keyboard input, etc...
 * Widgets cannot actually process or generate audio.
 */
struct FILTER1Widget : ModuleWidget {
    FILTER1Widget(FILTER1Module* module) {
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
        float outputY = 140;
        float labelAbove = 20;

        // Now we place the widgets that represent the inputs, outputs, controls,
        // and lights for the module. VCO1 does not have any lights, but does have
        // the other widgets.
        addInput(createInput<PJ301MPort>(Vec(x, inputY), module, FILTER1Module::INPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, outputY), module, FILTER1Module::OUTPUT));
    
        // Add some quick hack labels to the panel.
        addLabel(Vec(20, headingY), "Demo Filter1");
        addLabel(Vec(x-5, outputY - labelAbove), "Out");
        addLabel(Vec(x-1, inputY - labelAbove), "In");
    }

    // Simple helper function to add test labels to the panel.
    // In a real module you would draw this on the panel itself.
    // Labels are fine for hacking, but they are discouraged for real use.
    // Some of the problems are that they don't draw particularly efficiently,
    // and they don't give as much control as putting them into the panel SVG.
    Label* addLabel(const Vec& v, const std::string& str)
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
// I'm called "demo-filter1", my module is FILTER1Module, and my Widget is FILTER1Widget.
// In effect, it implements a module factory.
Model* modelFILTER1 = createModel<FILTER1Module, FILTER1Widget>("demo-filter1");