//
//  BassGraph.cpp
//  Acoustify
//
//  Created by Andrew Voelkel on 5/28/18.
//  Copyright © 2018 Setpoint Medical. All rights reserved.
//

#include "GenericDsp.hpp"
#include "Dynamics.hpp"
#include "BiquadChain.hpp"
#include "Sine.hpp"

extern "C" {
  float* out1;
  float* out2;
  float* out3;
}

const int bufSize = 128;
const int nProcCalls = 3;
const int SR = 44100;

struct TestContainer  {
  SineGen sine1;
  SineGen sine2;
  TwoInputMixer inputMixer;

  TestContainer() : sine1(1000, SR, bufSize), sine2(2000, SR, bufSize) {
    connect(sine1, 0, inputMixer, 0);
    connect(sine2, 0, inputMixer, 1);
    out1 = new float[nProcCalls * bufSize];
    out2 = new float[nProcCalls * bufSize];
    out3 = new float[nProcCalls * bufSize];
  }

  void process() {
    sine1.process();
    sine2.process();
    inputMixer.process();
  }

};

extern "C" void runTest() {
  int sampCnt = 0;
  TestContainer tc;
  for (int i=0; i < nProcCalls; i++) {
    tc.process();
    memcpy(&out1[sampCnt], tc.sine1.outputPins[0].buffers[0], bufSize * sizeof(float));
    memcpy(&out2[sampCnt], tc.sine2.outputPins[0].buffers[0], bufSize * sizeof(float));
    memcpy(&out3[sampCnt], tc.inputMixer.outputPins[0].buffers[0], bufSize * sizeof(float));
    sampCnt += bufSize;
  }

}
