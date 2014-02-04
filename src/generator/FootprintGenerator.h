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

#ifndef WATERSURFACESIM_WALKERGENERATOR_H_
#define WATERSURFACESIM_WALKERGENERATOR_H_

#include "generator/Generator.h"

/**
 * Stimulate as like the footprint made by human walk
 */
class FootprintGenerator : public Generator {
public:
  /**
   * defaultposx, defaultposy: The start position of walker
   * speed: The walk speed of walk
   * turnspeed: The turn speed of walk
   * force: The force to stimulate
   * footwidth: The width of the footprints of the both walker feet
   * footwidthfluct: The fluctuation of the foot width on every update
   */
  struct OriginOption {
    float defaultposx;
    float defaultposy;
    float speed;
    float turnspeed;
    float force;
    float footwidth;
    float footwidthfluct;

    OriginOption(float defaultposx, float defaultposy, float speed, float turnspeed, float force, float footwidth, float footwidthfluct)
    :
    defaultposx(defaultposx),
    defaultposy(defaultposy),
    speed(speed),
    turnspeed(turnspeed),
    force(force),
    footwidth(footwidth),
    footwidthfluct(footwidthfluct)
    {
    }
  };

  /**
   * Constructor
   */
  FootprintGenerator(const OriginOption& option, int (*ongeneration)(int, int, float));

  /**
   * Destructor
   */
  virtual ~FootprintGenerator();

  /**
   * Set the walker behavior on the next execution.
   * If you don't call, the walker is no move on the next execution.
   */
  void Walk(int go, int turn);

  /**
   * Update the walker motion.
   * You need to call every frame as long as you don't want to pause the execution.
   */
  virtual int Execute();

private:
  OriginOption option_;
  int go_;
  int turn_;
  float posx_;
  float posy_;
  float dir_;
  bool foot_;
};

#endif /* WATERSURFACESIM_WALKERGENERATOR_H_ */
