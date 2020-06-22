/**
 * This file contains the entire implementation of the VCO2 demo module.
 *
 * The code for VCO2 is very, very close to the code for VCO1.
 * We have marked the few changes with:
 * NEW 4 VCO2
 * 
 */

#include "demo-plugin.hpp"
#include <simd/vector.hpp>

using float_4 = simd::float_4;


/**
 * This is a fast sine approximation function from VCV Fundamental VCO-1.
 * We have literally copied and pasted it unchanged.
 * NEW 4 VCO2
 */
template <typename T>
T sin2pi_pade_05_5_4(T x) {
	x -= 0.5f;
	return (T(-6.283185307) * x + T(33.19863968) * simd::pow(x, 3) - T(32.44191367) * simd::pow(x, 5))
	       / (1 + T(1.296008659) * simd::pow(x, 2) + T(0.7028072946) * simd::pow(x, 4));
}


/*
 * only accurate for 0 <= x <= two Pi
 */

inline float_4 SquinkyLabs_sinTwoPi(float_4 _x) {
    const static float twoPi = 2 * 3.141592653589793238;
    const static float pi =  3.141592653589793238;
 //   _x -= SimdBlocks::ifelse((_x > float_4(pi)), float_4(twoPi), float_4::zero()); 
    _x -= ifelse((_x > float_4(pi)), float_4(twoPi), float_4::zero()); 

    float_4 xneg = _x < float_4::zero();
    float_4 xOffset = ifelse(xneg, float_4(pi / 2.f), float_4(-pi  / 2.f));
    xOffset += _x;
    float_4 xSquared = xOffset * xOffset;
    float_4 ret = xSquared * float_4(1.f / 24.f);
    float_4 correction = ret * xSquared *  float_4(.02 / .254);
    ret += float_4(-.5);
    ret *= xSquared;
    ret += float_4(1.f);

    ret -= correction;
    return ifelse(xneg, -ret, ret);    
}
 
// VCV has a limit of 16 channels in a polyphonic cable.
static const int maxPolyphony = 16;

// Since simd processes 4 floats at once,
// we will need four banks of four to make 16 voices.
static const int maxBanks = maxPolyphony / 4;

/**
 *  Every synth module must have a Module structure.
 *  This is where all the real-time processing code goes.
 */
struct VCO3Module : Module
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

    float_4 phaseAccumulators[maxBanks] = {0};
    float_4 phaseAdvance[maxBanks] = {0};

    
    /**
     * Reduce the aliasing by using the minBlep technique.
     * Luckily VCV SDK contains a good implementation of this.
     * Otherwise it would be a lot of work to do it ourselves.
     * 
     * Because our para waveform jumps just like the saw,
     * at the same time, they can share the same minBlep generator.
     * NEW 4 VCO2
     */
    dsp::MinBlepGenerator<16, 16, float_4> sawMinBlep[maxBanks];
    int currentPolyphony = 1;
    int currentBanks = 1;
    int loopCounter = 0;
    bool outputSaw = false;
    bool outputSin = false;
    bool outputPara = false;

    VCO3Module() {
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
        currentBanks = currentPolyphony / 4;
        if (currentPolyphony % 4) {
            ++currentBanks;
        }
        outputs[SIN_OUTPUT].setChannels(currentPolyphony);
        outputs[SAW_OUTPUT].setChannels(currentPolyphony);
        outputs[PARA_OUTPUT].setChannels(currentPolyphony);

        //now we are going to look at our input and parameters and
        // save off some values for out audio processing function.
        outputSaw = outputs[SAW_OUTPUT].isConnected();
        outputSin = outputs[SIN_OUTPUT].isConnected();
        outputPara = outputs[PARA_OUTPUT].isConnected();

        // Note that assigning a float to a float_4 silently copies the float into all
        // four floats in the float_4.
        float_4 pitchParam = params[PITCH_PARAM].value;
        for (int bank = 0; bank < currentBanks; ++bank) {
            const int currentChannel = bank * 4;

            // This API lets us fetch the CV for four channels at once.
            float_4 pitchCV = inputs[CV_INPUT].getPolyVoltageSimd<float_4>(currentChannel);
        
            // Normal arithmetic operators work transparently with float_4.
            // Sometimes when expressions mix float_4 and float you need
            // to explicitly convert, like we do here with the number f.
            float_4 combinedPitch = pitchParam + pitchCV - float_4(4.f);

            const float_4 q = float(log2(261.626));       // move up to C
            combinedPitch += q;

            // Combined pitch is in volts. Now use an exponential function
            // to convert that to a pitch.
            // This time we use the fast exp approximation from the VCV SDK.
            // NEW 4 VCO2
            const float_4 freq = rack::dsp::approxExp2_taylor5<float_4>(combinedPitch);

            // figure out how much to add to our ramp every cycle 
            // to make a saw at the desired frequency.
            const float_4 normalizedFreq = float_4(args.sampleTime) * freq;
            phaseAdvance[bank] = normalizedFreq;
        }
    }

    void generateOutput() {
        for (int bank = 0; bank < currentBanks; ++bank) {
            const int baseChannel = bank * 4;
            const int relativeChannel = currentPolyphony - baseChannel;

            // Just as before, be advance the phase and wrap around 0.
            // This time we do four oscillators at once.
            phaseAccumulators[bank] += phaseAdvance[bank];
            phaseAccumulators[bank] -= simd::floor(phaseAccumulators[bank]);
           

            float_4 minBlepValue;
            if (outputSaw || outputPara) {

                // Evaluate the phase, and determine if we are at a discontinuity.
                // Determine if the saw "should have" already crossed .5V in the last sample period
                // This is the SIMD version of the minBlep code from VCO2. Well, more properly it's the
                // code from VCV Fundamental VCO-1. I find this algorithm pretty difficult to figure out
                // in SIMD. Thank goodness the Fundamental code is such a rich well.
                float_4 halfCrossing = (0.5f - (phaseAccumulators[bank] -  phaseAdvance[bank])) /  phaseAdvance[bank];
                int halfMask = simd::movemask((0 < halfCrossing) & (halfCrossing <= 1.f));
                if (halfMask) {
                    for (int subChannel=0; subChannel < relativeChannel; ++subChannel) {
                        if (halfMask & (1 << subChannel)) {
                            float_4 mask = simd::movemaskInverse<float_4>(1 << subChannel);
                            float jumpPhase = halfCrossing[subChannel] - 1.f;
                            float_4 jumpAmount = mask & -2.f;
                            sawMinBlep[bank].insertDiscontinuity(jumpPhase, jumpAmount);
                        }
                    }
                }

                minBlepValue = sawMinBlep[bank].process();
            }

            if (outputSaw) {
                // NEW 4 VCO2
                // Because of the decision mentioned above to have out
                //output waveform have a different phase, we need to do
                // the same arithmetic that VCV VCO-1 uses to calculate the
                // output waveform.
                float_4 rawSaw = phaseAccumulators[bank] + float_4(.5f);
                rawSaw -= simd::trunc(rawSaw);
                rawSaw = 2 * rawSaw - 1;

                //  NEW 4 VCO2: add in the minBlep correction that we calculated above.
                rawSaw += minBlepValue;
                float_4 sawWave = float_4(5) * rawSaw;
                outputs[SAW_OUTPUT].setVoltageSimd(sawWave, baseChannel);
            }

            if (outputPara) {
                // This simple "parabolic ramp" is an example of a way one could try to 
                // make the sawtooth sound a little different.
                // NEW 4 VCO2: do math like we did with the saw to shift
                // the phase so it matches the phase of the minBlep.
                float_4 paraWave = phaseAccumulators[bank] + float_4(.5f);
                paraWave -= simd::trunc(paraWave);       // now 0 ... 1
                paraWave *= paraWave;                   // squared, but still 0..1
                paraWave = 2 * paraWave;                // now 0..2
                paraWave += minBlepValue;
                paraWave -= .33f * 2;                   // subtract out the DC component
                                                        // (use your calculus or trial and error).
                paraWave *= 5;
                outputs[PARA_OUTPUT].setVoltageSimd(paraWave, baseChannel);
            }

            if (outputSin) {
                // If the sin output it patched, turn our 0..1 ramp
                // into a -5..+5 sine.
                // Use fast sin approximation from VCV VCO-1
                // NEW 4 VCO2
              //  float_4 sinWave = float_4(5.f) * sin2pi_pade_05_5_4<float_4>( phaseAccumulators[bank]);
            //    float_4 sinWave = phaseAccumulators[bank];
                const static float twoPi = 2 * 3.141592653589793238;
                float_4 sinWave = SquinkyLabs_sinTwoPi( phaseAccumulators[bank] * twoPi);
                outputs[SIN_OUTPUT].setVoltageSimd(sinWave, baseChannel);
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
struct VCO3Widget : ModuleWidget {
    VCO3Widget(VCO3Module* module) {
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

        addInput(createInput<PJ301MPort>(Vec(x, inputY), module, VCO3Module::CV_INPUT));
        addParam(createParam<RoundBlackKnob>(Vec(x-4, knobY), module, VCO3Module::PITCH_PARAM));
        addOutput(createOutput<PJ301MPort>(Vec(x, sawY), module, VCO3Module::SAW_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, sinY), module, VCO3Module::SIN_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(x, paraY), module, VCO3Module::PARA_OUTPUT));
    
        // Add some quick hack labels to the panel.
        addLabel(Vec(20, headingY), "Demo VCO3");
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

Model* modelVCO3 = createModel<VCO3Module, VCO3Widget>("demo-vco3");