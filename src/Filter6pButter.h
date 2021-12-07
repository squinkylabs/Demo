#pragma once

#include <assert.h>

class Filter6PButter {
public:
    Filter6PButter() {

    }

    void setCutoffFreq(float x) {
        assert(x > 0 && x < .5f);
    }

    float process(float x) {
        return x;
    }

private:
};