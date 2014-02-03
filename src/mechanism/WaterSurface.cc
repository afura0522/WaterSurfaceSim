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

#include "WaterSurface.h"

#include <Windows.h>

WaterSurface::WaterSurface(int width, int height, float propagation,
                           float attenuation)
    : width_(width),
      height_(height),
      propagation_(propagation),
      attenuation_(attenuation),
      prevheightbuf_(NULL),
      currheightbuf_(NULL),
      nextheightbuf_(NULL),
      prevheights_(NULL),
      currheights_(NULL),
      nextheights_(NULL) {
  prevheightbuf_ = new float[width_ * height_]();
  prevheights_ = new float*[height_]();
  for (int i = 0; i < height_; ++i) {
    prevheights_[i] = prevheightbuf_ + width_ * i;
  }
  currheightbuf_ = new float[width_ * height_]();
  currheights_ = new float*[height_]();
  for (int i = 0; i < height_; ++i) {
    currheights_[i] = currheightbuf_ + width_ * i;
  }
  nextheightbuf_ = new float[width_ * height_]();
  nextheights_ = new float*[height_]();
  for (int i = 0; i < height_; ++i) {
    nextheights_[i] = nextheightbuf_ + width_ * i;
  }
}

WaterSurface::~WaterSurface() {
  delete nextheights_;
  delete nextheightbuf_;
  delete currheights_;
  delete currheightbuf_;
  delete prevheights_;
  delete prevheightbuf_;
}

void WaterSurface::Initialize() {
  ClearAll();
}

void WaterSurface::Finalize() {
}

void WaterSurface::Execute() {
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      float neighborSum = 0.0f;

      // plus the neightbor point values
      if (0 != j) {
        neighborSum += currheights_[j - 1][i] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if (width_ - 1 != j) {
        neighborSum += currheights_[j + 1][i] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if (0 != i) {
        neighborSum += currheights_[j][i - 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if (height_ - 1 != i) {
        neighborSum += currheights_[j][i + 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if ((0 != i) && (0 != j)) {
        neighborSum += currheights_[j - 1][i - 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if ((height_ - 1 != i) && (0 != j)) {
        neighborSum += currheights_[j - 1][i + 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if ((0 != i) && (width_ - 1 != j)) {
        neighborSum += currheights_[j + 1][i - 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }
      if ((height_ - 1 != i) && (width_ - 1 != j)) {
        neighborSum += currheights_[j + 1][i + 1] - currheights_[j][i];
      } else {
        neighborSum += currheights_[j][i] - currheights_[j][i];
      }

      float neighborEffect = (neighborSum / 8.0f) * propagation_;
      float prevEffect = currheights_[j][i] - prevheights_[j][i];

      nextheights_[j][i] = (currheights_[j][i] + neighborEffect + prevEffect)
          * attenuation_;
    }
  }

  // Fî•ñXV
  memcpy(prevheightbuf_, currheightbuf_, sizeof(float) * width_ * height_);
  memcpy(currheightbuf_, nextheightbuf_, sizeof(float) * width_ * height_);
}

void WaterSurface::ClearAll() {
  memset(prevheightbuf_, 0, sizeof(float) * width_ * height_);
  memset(currheightbuf_, 0, sizeof(float) * width_ * height_);
  memset(nextheightbuf_, 0, sizeof(float) * width_ * height_);
}
