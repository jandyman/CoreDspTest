//
//  GenericDSP.cpp
//  Acoustify
//
//  Created by Andrew Voelkel on 5/30/18.
//  Copyright © 2018 Setpoint Medical. All rights reserved.
//

#include "GenericDsp.hpp"

void connect(DspInterface& from, uint fromPinIdx, DspInterface& to, uint toPinIdx) {

  if (fromPinIdx >= from.getNOutputPins()) {
    throw new DspError("connect: non existent from pin");
  }
  if (toPinIdx >= to.getNInputPins()) {
    throw new DspError("connect: non existent to pin");
  }

  to.connect(from.getOutputPin(fromPinIdx), toPinIdx);

}

void connect(DspInterface& from, DspInterface& to) {
  connect(from, 0, to, 0);
}
