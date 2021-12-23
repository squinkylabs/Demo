#include "demo-plugin.hpp"

Plugin* pluginInstance;

void init(rack::Plugin* p) {
	pluginInstance = p;

	// Here you must add all the models that your plugin implements.
	// There must be at least one. Often there are many more.
	p->addModel(modelVCO1);
	p->addModel(modelVCO2);
	p->addModel(modelVCO3);
	p->addModel(modelFILTER1);
	p->addModel(modelSHAPER1);
	p->addModel(modelVCOB1);
}
