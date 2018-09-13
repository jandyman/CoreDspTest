//
//  TestContainer.h
//  CoreDspTest
//
//  Created by Andrew Voelkel on 6/8/18.
//  Copyright © 2018 GronkWorks. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus

#include "GenericDsp.hpp"
#include "Sine.hpp"
#include "Mixers.hpp"

struct Graph  {
  SineGen sine1;
  SineGen sine2;
  TwoInputMixer inputMixer;

  Graph(int SR, int bufSize) : sine1(1000, SR, bufSize), sine2(2000, SR, bufSize) {
    connect(sine1, 0, inputMixer, 0);
    connect(sine2, 0, inputMixer, 1);
  }

  void process() {
    sine1.process();
    sine2.process();
    inputMixer.process();
  }

};

#endif

@interface TestContainer : NSObject

-(id)initWithSampleRate:(float) sampleRate bufferSize:(uint) count;
-(void)process;

@property (readonly) float* probe1;
@property (readonly) float* probe2;
@property (readonly) float* probe3;
@property (nonatomic) float frequency1;
@property (nonatomic) float frequency2;

@end
