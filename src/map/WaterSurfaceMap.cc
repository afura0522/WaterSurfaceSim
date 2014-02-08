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

#include "map/WaterSurfaceMap.h"

#include <algorithm>

#include "ColorMap.h"

WaterSurfaceMap::WaterSurfaceMap(int width, int height, float propagation,
                                 float attenuation)
    : ColorMap(width, height),
      ColorMapBitmapMixIn(this),
      ws(width, height, propagation, attenuation) {
}

WaterSurfaceMap::~WaterSurfaceMap() {
}

void WaterSurfaceMap::Initialize() {
  ws.Initialize();
  InitializeBitmapMixIn();
}

void WaterSurfaceMap::Finalize() {
  FinalizeBitmapMixIn();
  ws.Finalize();
}

void WaterSurfaceMap::Execute() {
  // êFèÓïÒçXêV
  ws.Execute();

  for (int i = 0; i < height(); ++i) {
    for (int j = 0; j < width(); ++j) {
      for (int k = 0; k < ColorMap::ColorNum; ++k) {
        set_pixel(j, i, k, ws.GetHeight(j, i));
      }
    }
  }

  set_texturebufflush(true);
}

void WaterSurfaceMap::ClearAll() {
  ColorMap::ClearAll();
  ws.ClearAll();
}

void WaterSurfaceMap::SetHeight(int x, int y, float height) {
  // must validate
  int x_validate = std::min(std::max(x, 0), width() - 1);
  int y_validate = std::min(std::max(y, 0), this->height() - 1);
  ws.SetHeight(x_validate, y_validate, height);
}

float WaterSurfaceMap::GetHeight(int x, int y) const {
  // must validate
  int x_validate = std::min(std::max(x, 0), width() - 1);
  int y_validate = std::min(std::max(y, 0), this->height() - 1);
  return ws.GetHeight(x_validate, y_validate);
}
