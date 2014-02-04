/**
 * Copyright (C) 2014 The Motel On Jupiter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef WATERSURFACESIM_RAINGENERATOR_H_
#define WATERSURFACESIM_RAINGENERATOR_H_

#include <memory>

#include "generator/Generator.h"

class RainGenerator : public Generator {
public:
  struct OriginOption {
    // the area to generate
    int areawidth;
    int areaheight;
    // the range for the value to generate
    float minforce;
    float maxforce;
    // the number of the stimulus per execution
    float frequency;

    OriginOption(
        int areawidth,
        int areaheight,
        float minforce,
        float maxforce,
        float frequency
    )
    :
    areawidth(areawidth),
    areaheight(areaheight),
    minforce(minforce),
    maxforce(maxforce),
    frequency(frequency)
    {
    }
  };

  RainGenerator(const OriginOption &option, int ongeneration(int, int, float));
  virtual ~RainGenerator();

  virtual int Execute();

  // acccessor
  inline const OriginOption &option() const {return option_;}
  inline int areawidth() const {return option_.areawidth;}
  inline int areaheight() const {return option_.areaheight;}
  inline float frequency() const {return option_.frequency;}

  // mutator
  inline void set_option(const OriginOption &option) {memcpy(&option_, &option, sizeof(OriginOption));}
  inline void set_areawidth(int areawidth) {option_.areawidth = areawidth;}
  inline void set_areaheight(int areaheight) {option_.areaheight = areaheight;}
  inline void set_frequency(float frequency) {option_.frequency = frequency;}

private:
  OriginOption option_;
};

#endif /* WATERSURFACESIM_RAINGENERATOR_H_ */
