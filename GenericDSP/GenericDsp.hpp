//
//  GenericDsp.hpp
//  Acoustify
//
//  Created by Andrew Voelkel on 5/28/18.
//  Copyright © 2018 Setpoint Medical. All rights reserved.
//

#pragma once

#include <stdlib.h>
#define uint unsigned int

struct DspError {
  const char *msg;
  DspError(const char *msg) : msg(msg) {}
};

struct Pin {
  uint nChannels;
  float **buffers = NULL;
  uint bufSize;
  float sampleRate;

  Pin(uint nChannels) {
    this->nChannels = nChannels;
    buffers = new float*[nChannels];
    for (int i = 0; i < nChannels; i++) { buffers[i] = NULL; }
  }

  Pin() : Pin(0) {}

  void createBuffers() {
    if (buffers == NULL) { buffers = new float*[nChannels]; }
    for (int i = 0; i < nChannels; i++) { buffers[i] = new float[bufSize]; }
  }

  void freeBuffers() {
    if (buffers != NULL) {
      for (int i = 0; i < nChannels; i++) {
        if (buffers[i] != NULL) { delete buffers[i]; }
      }
    }
  }

  bool isCompatible(Pin pin) {
    return (nChannels == pin.nChannels && bufSize == pin.bufSize && sampleRate == pin.sampleRate);
  }

  // Don't delete actual buffer storage, since this may be an input pin and therefore
  // doesn't own the buffers

  ~Pin() {
    if (buffers != NULL) {
      delete buffers;
    }
  }

  // Don't copy intermediate pointers, they belong to each Pin

  Pin (const Pin &obj) {
    nChannels = obj.nChannels;
    sampleRate = obj.sampleRate;
    bufSize = obj.bufSize;
    buffers = new float*[nChannels];
    for (int i=0; i < nChannels; i++) { buffers[i] = obj.buffers[i]; }
  }

  Pin& operator = (const Pin &obj) {
    nChannels = obj.nChannels;
    sampleRate = obj.sampleRate;
    bufSize = obj.bufSize;
    for (int i=0; i < nChannels; i++) { buffers[i] = obj.buffers[i]; }
    return *this;
  }

};

struct DspInterface {
  virtual int getNInputPins() = 0;
  virtual int getNOutputPins() = 0;
  virtual Pin getInputPin(uint idx) = 0;
  virtual Pin getOutputPin(uint idx) = 0;
  virtual bool canConnect(Pin pin, uint inputPinIdx) = 0;
  virtual void connect(Pin outPin, uint inIdx) = 0;
  virtual void init() = 0;
  virtual void process() = 0;
};

struct DspBase : DspInterface {
  uint nInputPins = 0;
  uint nOutputPins = 0;
  Pin *inputPins = NULL;
  Pin *outputPins = NULL;

  int getNInputPins() override { return nInputPins; }
  int getNOutputPins() override { return nOutputPins; }

  void init() override {};
  void process() override {};

  DspBase(int nInputPins, int nOutputPins) : nInputPins(nInputPins), nOutputPins(nOutputPins) {
    inputPins = new Pin[nInputPins];
    outputPins = new Pin[nOutputPins];
  }

  DspBase(int nInputPins) : DspBase(nInputPins, 1) {}

  ~DspBase() {
    for (int i=0; i < nOutputPins; i++) {
      outputPins[i].freeBuffers();
    }
  }

  Pin getInputPin(uint idx) override {
    if (idx >= nInputPins) throw new DspError("get Input Pin: idx too high\n");
    return inputPins[idx];
  }

  Pin getOutputPin(uint idx) override {
    if (idx >= nOutputPins) throw new DspError("get Output Pin: idx too high\n");
    return outputPins[idx];
  }

  void checkPinIdx(uint idx) {
    if (idx >= nInputPins) throw new DspError("checkPinIdx: idx >= nInputPins");
  }

  bool canConnect(Pin pin, uint inputPinIdx) override {
    if (inputPinIdx >= nInputPins) { return false; }
    return true;
  }

  void connect(Pin outPin, uint inIdx) override {
    // assume 1 output pin for default functionality
    checkPinIdx(inIdx);
    inputPins[inIdx] = outPin;
    Pin& oPin = outputPins[0]; // my output pin
    if (oPin.nChannels == 0) { // this is the first pin connected
      oPin = outPin;              // copy bufSize, sampleRate, nChannels
      oPin.createBuffers();    // but create our own buffers
    } else if (!oPin.isCompatible(outPin)) {
      throw new DspError("connectPin: input pin incompatibility != 1");
    } else {
      inputPins[inIdx] = outPin;
    }
  }

};

void connect(DspInterface& from, uint fromPinIdx, DspInterface& to, uint toPinIdx);
void connect(DspInterface& from, DspInterface& to);












