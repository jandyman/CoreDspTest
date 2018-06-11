
#pragma once

#include "GenericDsp.hpp"
#include <math.h>

struct SineGen : DspBase {
  float frequency;
  float amplitude = 1.0;
  float phase = 0;

  SineGen(float frequency, float sampleRate, uint bufSize) : DspBase(0,1) {
    this->frequency = frequency;
    Pin& pin = outputPins[0];
    pin.nChannels = 1;
    pin.sampleRate = sampleRate;
    pin.bufSize = bufSize;
    pin.createBuffers();
  }

  void process() override {
    Pin& op = outputPins[0];
    for (int samp=0; samp < op.bufSize; samp++) {
      op.buffers[0][samp] = sin(phase * 2 * M_PI) * amplitude;
      phase += frequency / op.sampleRate;
    }
  }

};
