#include "demo-plugin.hpp"


Plugin* pluginInstance;

void init(rack::Plugin* p) {
	pluginInstance = p;

	p->addModel(modelVCO1);
	
}
