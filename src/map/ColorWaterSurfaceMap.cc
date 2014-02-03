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

#include "map/ColorWaterSurfaceMap.h"

#include <Windows.h>

#include "mechanism/WaterSurface.h"

ColorWaterSurfaceMap::ColorWaterSurfaceMap(int width, int height,
                                           float propagation, float attenuation)
    : ColorMap(width, height),
      ColorMapBitmapMixIn(this),
      wss_() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i] = new WaterSurface(width, height, propagation, attenuation);
  }
}

ColorWaterSurfaceMap::~ColorWaterSurfaceMap() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    delete wss_[i];
  }
}

void ColorWaterSurfaceMap::Initialize() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i]->Initialize();
  }
  InitializeBitmapMixIn();
}

void ColorWaterSurfaceMap::Finalize() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i]->Finalize();
  }
  FinalizeBitmapMixIn();
}

void ColorWaterSurfaceMap::Execute() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i]->Execute();
  }

  // êFèÓïÒçXêV
  for (int i = 0; i < height(); ++i) {
    for (int j = 0; j < width(); ++j) {
      for (int k = 0; k < ColorMap::ColorNum; ++k) {
        set_pixel(j, i, k, wss_[k]->GetHeight(j, i));
      }
    }
  }

  set_texturebufflush(true);
}

void ColorWaterSurfaceMap::ClearAll() {
  ColorMap::ClearAll();
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i]->ClearAll();
  }
}

void ColorWaterSurfaceMap::SetHeight(int x, int y,
                                     float (&color)[ColorMap::ColorNum]) {
  // must validate
  int x_validate = min(max(x, 0), width() - 1);
  int y_validate = min(max(y, 0), height() - 1);
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    wss_[i]->SetHeight(x_validate, y_validate, color[i]);
  }
}
