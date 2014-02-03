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

#include "ColorMap.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

const float ColorMap::WhiteColor[ColorMap::ColorNum] = { 1.0f, 1.0f, 1.0f };

ColorMap::ColorMap(int width, int height)
    : width_(width),
      height_(height),
      pixels_(),
      pixelbuf_(NULL),
      texturebuf_(NULL),
      texturebufflush_(true) {
  pixelbuf_ = new float[width * height * ColorNum]();
  pixels_ = new float**[width]();
  for (int i = 0; i < height; ++i) {
    pixels_[i] = new float*[height]();
    for (int j = 0; j < width; ++j) {
      pixels_[i][j] = pixelbuf_ + ColorNum * (height * i + j);
    }
  }
  texturebuf_ = new float[width * height * ColorMap::ColorNum]();
}

ColorMap::~ColorMap() {
  delete[] texturebuf_;
  for (int i = 0; i < height_; ++i) {
    delete pixels_[i];
  }
  delete pixels_;
  delete pixelbuf_;
}

void ColorMap::Initialize() {
  ClearAll();
}

void ColorMap::Finalize() {
}

void ColorMap::ClearAll() {
  memset(pixelbuf_, 0, width() * height() * ColorNum * sizeof(float));
}

const float *ColorMap::OutputTexture(
    const float (&basecolor)[ColorMap::ColorNum]) {
  if (texturebufflush_) {
    for (int i = 0; i < height(); ++i) {
      for (int j = 0; j < width(); ++j) {
        for (int k = 0; k < ColorMap::ColorNum; ++k) {
          set_texturebuf(j, i, k, basecolor[k] * pixel(j, i, k));
        }
      }
    }
    texturebufflush_ = false;
  }
  return texturebuf_;
}
