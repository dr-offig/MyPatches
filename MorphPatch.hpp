#ifndef __MorphPatch_hpp__
#define __MorphPatch_hpp__

#include "StompBox.h"

// #include "SimpleSinePatch.hpp"
// #include "SineOscPatch.hpp"
// #define GREEN_PATCH SimpleSinePatch
// #define RED_PATCH SineOscPatch
#include "TrickySineOscPatch.hpp"
#include "SineOscPatch.hpp"
#define GREEN_PATCH SineOscPatch
#define RED_PATCH TrickySineOscPatch

#define MORPH_PARAMETER PARAMETER_C

/**
 */
class MorphPatch : public Patch {
private:
  GREEN_PATCH green;
  RED_PATCH red;
  AudioBuffer* buf;
public:
  MorphPatch(){
    registerParameter(MORPH_PARAMETER, "Morph");
    buf = AudioBuffer::create(2, getBlockSize());
  }
  void processAudio(AudioBuffer &buffer){
    FloatArray l1 = buffer.getSamples(LEFT_CHANNEL);
    FloatArray r1 = buffer.getSamples(RIGHT_CHANNEL);
    FloatArray l2 = buf->getSamples(LEFT_CHANNEL);
    FloatArray r2 = buf->getSamples(RIGHT_CHANNEL);
    float morph = getParameterValue(MORPH_PARAMETER);
    l2.copyFrom(l1);
    r2.copyFrom(r1);
    green.processAudio(*buf);
    red.processAudio(buffer);
    int size = buffer.getSize();
    for(int i=0; i<size; ++i){
      l1[i] = l1[i]*(1-morph) + l2[i]*morph;
      r1[i] = r1[i]*(1-morph) + r2[i]*morph;
    }
  }
};

#endif   // __MorphPatch_hpp__
