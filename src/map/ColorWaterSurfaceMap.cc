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
                                           float propagation, float attenuation,
                                           const float (&heightrange)[2])
    : ColorMap(width, height),
      ws_(),
      heightrange_() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i] = new WaterSurface(width, height, propagation, attenuation);
  }
  memcpy(heightrange_, heightrange, sizeof(heightrange));
}

ColorWaterSurfaceMap::~ColorWaterSurfaceMap() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    delete ws_[i];
  }
}

void ColorWaterSurfaceMap::Initialize() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i]->Initialize();
  }
}

void ColorWaterSurfaceMap::Finalize() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i]->Finalize();
  }
}

void ColorWaterSurfaceMap::Execute() {
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i]->Execute();
  }

  // Update the pixel colors
  for (int i = 0; i < height(); ++i) {
    for (int j = 0; j < width(); ++j) {
      for (int k = 0; k < ColorMap::ColorNum; ++k) {
        float color = (ws_[k]->GetHeight(j, i) - heightrange_[0])
            / (heightrange_[1] - heightrange_[0]);
        color = (color > 1.0f) ? 1.0f : (color < 0.0f) ? 0.0f : color;
        set_pixel(j, i, k, color);
      }
    }
  }

  set_texturebufflush(true);
}

void ColorWaterSurfaceMap::ClearAll() {
  ColorMap::ClearAll();
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i]->ClearAll();
  }
}

void ColorWaterSurfaceMap::SetHeight(int x, int y,
                                     const float (&color)[ColorMap::ColorNum]) {
  // must validate
  int x_validate = min(max(x, 0), width() - 1);
  int y_validate = min(max(y, 0), height() - 1);
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    ws_[i]->SetHeight(x_validate, y_validate, color[i]);
  }
}

void ColorWaterSurfaceMap::GetHeight(int x, int y,
                                     float (&color)[ColorMap::ColorNum]) const {
  // must validate
  int x_validate = min(max(x, 0), width() - 1);
  int y_validate = min(max(y, 0), height() - 1);
  for (int i = 0; i < ColorMap::ColorNum; ++i) {
    color[i] = ws_[i]->GetHeight(x_validate, y_validate);
  }
}
