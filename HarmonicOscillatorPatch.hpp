#ifndef __HarmonicOscillatorPatch_hpp__
#define __HarmonicOscillatorPatch_hpp__

#include "StompBox.h"
#include "VoltsPerOctave.h"
#include "HarmonicOscillator.hpp"
#include "WavetableOscillator.h"
#include "BiquadFilter.h"

class HarmonicOscillatorPatch : public Patch {
private:
  static const int samples = 1024;
  static const int controls = 4;
  HarmonicOscillator<samples, controls> harms;
  WavetableOscillator* osc;
  FloatArray wavetable;
  VoltsPerOctave hz;
public:
HarmonicOscillatorPatch(){
    wavetable = FloatArray::create(samples);
    osc = new WavetableOscillator(getSampleRate(), wavetable);
    osc->setFrequency(440);
    // harms.setWavelength(64);
    for(int i=0; i<controls; ++i)
      harms.setControl(i, 0.25);
    harms.calculate(wavetable, samples);
  }
  void processAudio(AudioBuffer &buffer) {
    float tune = getParameterValue(PARAMETER_A)*8.0 - 6.0;
    // float a = getParameterValue(PARAMETER_A);
    float b = getParameterValue(PARAMETER_B);
    float c = getParameterValue(PARAMETER_C);
    float d = getParameterValue(PARAMETER_D);
    float e = getParameterValue(PARAMETER_E);
    harms.setTimeStep(b);
    // harms.setControl(0, b);
    harms.setControl(1, c);
    harms.setControl(2, d);
    harms.setControl(3, e);
    harms.normalizeAmplitudes();
    // harms.calculate(wavetable, samples);
    harms.calculateNormalized(wavetable, samples);
    harms.increment();
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    hz.setTune(tune);
    float freq = hz.getFrequency(left[0]);
    osc->setFrequency(freq);
    osc->getSamples(left);
    right.copyFrom(left);
    right.multiply(0.1);
  }
};

#endif   // __HarmonicOscillatorPatch_hpp__
