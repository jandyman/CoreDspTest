//
//  ViewController.swift
//  CoreDspTest
//
//  Created by Andrew Voelkel on 6/4/18.
//  Copyright © 2018 GronkWorks. All rights reserved.
//

import Cocoa
import CorePlot

class ProbeGetter {
  let source: UnsafeMutablePointer<Float>!
  let bufSize: Int
  var data = [Float]()
  
  init(source: UnsafeMutablePointer<Float>, bufSize: uint) {
    self.source = source; self.bufSize = Int(bufSize)
  }
  
  func addBuffer() {
    data += Array(UnsafeBufferPointer(start: source, count: bufSize))
  }
  
  func toDouble() -> [Double] { return data.map { Double($0) }}
}

class ViewController: NSViewController {
  
  @IBOutlet weak var graph: Graph!
  
  let SR: Float = 44100.0
  let bufSize: uint = 128
  let nProcCalls = 3
  
  override func viewDidLoad() {
    super.viewDidLoad()
    
    let tc = TestContainer(sampleRate: SR, bufferSize: bufSize)!
    tc.frequency2 = 500.0; // show how to change a parameter
    
    let probeSources = [tc.probe1!, tc.probe2!, tc.probe3!]
    let probeGetters = probeSources.map {
      return ProbeGetter(source: $0, bufSize: bufSize)
    }
    
    for _ in 0..<nProcCalls {
      tc.process()
      _ = probeGetters.map { $0.addBuffer() }
    }
    
    setupGraph(getters: probeGetters)
  }
  
  var dataSet: YValuesDataSet!
  
  func setupGraph(getters: [ProbeGetter])  {
    let y = getters.map { YValuesDataSet(yValues: $0.toDouble()) }
    _ = y.map { graph.addTrace(dataSource: $0) }
  }
  
}


