
#pragma once

#include "GenericDsp.hpp"
#include <Accelerate/Accelerate.h>


struct TwoInputMixer : DspBase {

  TwoInputMixer() : DspBase(2,1) { }

  void process() override {
    Pin& op = outputPins[0];
    Pin& ip1 = inputPins[0];
    Pin& ip2 = inputPins[1];
    for (int i=0; i < op.nChannels; i++) {
      vDSP_vadd(ip1.buffers[i], 1, ip2.buffers[i], 1, op.buffers[i], 1, op.bufSize);
    }
  }

};
