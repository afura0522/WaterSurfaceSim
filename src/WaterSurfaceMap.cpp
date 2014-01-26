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

#include "WaterSurfaceMap.h"

#include <Windows.h>

#include "ColorMap.h"

template<class T> static T **new_continuouse_2darray(int len0, int len1) {
  float **buf = new T*[len0]();
  buf[0] = new T[len0 * len1]();
  for (int i = 1; i < len0; i++) {
    buf[i] = buf[0] + len1 * i;
  }
  return buf;
}

template<class T> static void delete_continuouse_2darray(T **buf) {
  delete[] buf[0];
  delete[] buf;
}

WaterSurfaceMap::WaterSurfaceMap(int mapwidth, int mapheight, float propagation,
                                 float attenuation)
    : ColorMap(mapwidth, mapheight),
      ColorMapBitmapMixIn(this),
      propagation_(propagation),
      attenuation_(attenuation),
      prevtexs_(NULL),
      currtexs_(NULL),
      nexttexs_(NULL),
      texturebuf_(NULL),
      texturebufflush_(true) {
  prevtexs_ = new_continuouse_2darray<float>(mapwidth, mapheight);
  currtexs_ = new_continuouse_2darray<float>(mapwidth, mapheight);
  nexttexs_ = new_continuouse_2darray<float>(mapwidth, mapheight);
  texturebuf_ = new float[mapwidth * mapheight * ColorMap::ColorNum]();
}

WaterSurfaceMap::~WaterSurfaceMap() {
  delete[] texturebuf_;
  delete_continuouse_2darray<float>(nexttexs_);
  delete_continuouse_2darray<float>(currtexs_);
  delete_continuouse_2darray<float>(prevtexs_);
}

void WaterSurfaceMap::Initialize() {
  InitializeBitmapMixIn();
}

void WaterSurfaceMap::Finalize() {
  FinalizeBitmapMixIn();
}

void WaterSurfaceMap::Execute() {
  for (int i = 0; i < height(); ++i) {
    for (int j = 0; j < width(); ++j) {
      float neighborSum = 0.0f;

      // plus the neightbor point values
      if (0 != j) {
        neighborSum += currtexs_[j - 1][i] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if (width() - 1 != j) {
        neighborSum += currtexs_[j + 1][i] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if (0 != i) {
        neighborSum += currtexs_[j][i - 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if (height() - 1 != i) {
        neighborSum += currtexs_[j][i + 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if ((0 != i) && (0 != j)) {
        neighborSum += currtexs_[j - 1][i - 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if ((height() - 1 != i) && (0 != j)) {
        neighborSum += currtexs_[j - 1][i + 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if ((0 != i) && (width() - 1 != j)) {
        neighborSum += currtexs_[j + 1][i - 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }
      if ((height() - 1 != i) && (width() - 1 != j)) {
        neighborSum += currtexs_[j + 1][i + 1] - currtexs_[j][i];
      } else {
        neighborSum += currtexs_[j][i] - currtexs_[j][i];
      }

      float neighborEffect = (neighborSum / 8.0f) * propagation_;
      float prevEffect = currtexs_[j][i] - prevtexs_[j][i];

      nexttexs_[j][i] = (currtexs_[j][i] + neighborEffect + prevEffect) * attenuation_;
    }
  }

  // Fî•ñXV
  memcpy(*prevtexs_, *currtexs_, sizeof(float) * width() * height());
  memcpy(*currtexs_, *nexttexs_, sizeof(float) * width() * height());

  texturebufflush_ = true;
}

void WaterSurfaceMap::ClearAll() {
  memset(*prevtexs_, 0, sizeof(float) * width() * height());
  memset(*currtexs_, 0, sizeof(float) * width() * height());
  memset(*nexttexs_, 0, sizeof(float) * width() * height());
}

const float *WaterSurfaceMap::OutputTexture() {
  if (texturebufflush_) {
    for (int i = 0; i < height(); ++i) {
      for (int j = 0; j < width(); ++j) {
        texturebuf_[(i * width() + j) * ColorNum + 0] = 0.0f;
        texturebuf_[(i * width() + j) * ColorNum + 1] = max(
            min(1.0f * currtexs_[j][i], 1.0f), 0.0f);
        texturebuf_[(i * width() + j) * ColorNum + 2] = max(
            min(1.0f * currtexs_[j][i], 1.0f), 0.0f);
      }
    }
    texturebufflush_ = false;
  }
  return texturebuf_;
}

void WaterSurfaceMap::Set(int x, int y, float val) {
  // must validate
  int x_validate = min(max(x, 0), width() - 1);
  int y_validate = min(max(y, 0), height() - 1);
  currtexs_[y_validate][x_validate] = val;
}
