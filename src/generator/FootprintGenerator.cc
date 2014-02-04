/**
 * Copyright (c) 2014 The Motel On Jupiter
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

/**
 *  Handle the angle on the unit of radian.
 */

#include "generator/FootprintGenerator.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#define ELEMENTSOF(_array) (sizeof(_array) / sizeof(_array[0]))

static const float PI = static_cast<float>(M_PI);

/**
 * Constructor
 */
FootprintGenerator::FootprintGenerator(const OriginOption& option,
                                   int (*ongeneration)(int, int, float))
    : Generator(ongeneration),
      option_(option),
      go_(0),
      turn_(0),
      posx_(option.defaultposx),
      posy_(option.defaultposy),
      dir_(0.0f),
      foot_(false) {
}

/**
 * Destructer
 */
FootprintGenerator::~FootprintGenerator() {
}

/**
 * Record the walk request on the frame.
 * If go is set as positive integer the walker go ahead,
 * and if go is set as negative integer the walker be back,
 * and if go is set as zero the walker don't move.
 * If turn is set as positive integer the walker turn left,
 * and if turn is set as negative integer the walker turn right,
 * and if turn is set as zero the walker don't turn.
 */
void FootprintGenerator::Walk(int go, int turn) {
  go_ = go;
  turn_ = turn;
}

/**
 * Update the walker status,
 * and stimulate the point for the one side foot to earth.
 */
int FootprintGenerator::Execute() {
  // update the direction
  if (0 < turn_) {
    dir_ += option_.turnspeed;
  } else if (0 > turn_) {
    dir_ -= option_.turnspeed;
  }

  // normalize the direction value
  while (0.0f > dir_) {
    dir_ += 2.0f;
  }
  while (2.0f <= dir_) {
    dir_ -= 2.0f;
  }

  // update the position
  if (0 < go_) {
    posx_ += option_.speed * std::cos(dir_ * PI);
    posy_ += option_.speed * std::sin(dir_ * PI);
  } else if (0 > go_) {
    posx_ -= option_.speed * std::cos(dir_ * PI);
    posy_ -= option_.speed * std::sin(dir_ * PI);
  }

  // stimulate the point of foot earthing
  if (0 != go_) {
    float footoffsetfluct = option_.footwidthfluct
        * (static_cast<float>(rand()) * 2.0f / RAND_MAX - 1.0f);
    float footoffset = (option_.footwidth + footoffsetfluct)
        * (foot_ ? 1.0f : -1.0f);
    float footoffsetx = footoffset * std::cos((dir_ + 0.5f) * PI);
    float footoffsety = footoffset * std::sin((dir_ + 0.5f) * PI);
    int x = static_cast<int>(posx_ + footoffsetx);
    int y = static_cast<int>(posy_ + footoffsety);
    int ret = Generate(x, y, option_.force);
    if (0 > ret) {
      return -1;
    }

    // switch the side of foot
    foot_ = !foot_;
  }

  // clear the settings
  go_ = 0;
  turn_ = 0;

  return 0;
}
