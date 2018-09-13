//
//  TestContainer.m
//  CoreDspTest
//
//  Created by Andrew Voelkel on 6/8/18.
//  Copyright © 2018 GronkWorks. All rights reserved.
//

#import "TestContainer.h"

@implementation TestContainer

Graph* graph;

-(id)initWithSampleRate:(float) sampleRate bufferSize:(uint) bufferSize {
  if (self = [super init]) {
    graph = new Graph(sampleRate, bufferSize);
  }
  return self;
}

-(void)process { graph->process(); }

-(float*)probe1 { return graph->sine1.outputPins[0].buffers[0]; }
-(float*)probe2 { return graph->sine2.outputPins[0].buffers[0]; }
-(float*)probe3 { return graph->inputMixer.outputPins[0].buffers[0]; }

-(void)setFrequency1:(float)frequency { graph->sine1.frequency = frequency; }
-(void)setFrequency2:(float)frequency { graph->sine2.frequency = frequency; }

@end
