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

#include "map/mixin/ColorMapBitmapImportable.h"

#include <fstream>
#include <Windows.h>

#include "map/ColorMap.h"

ColorMapBitmapImportable::ColorMapBitmapImportable(ColorMap * const map)
    : map_(map) {
}

ColorMapBitmapImportable::~ColorMapBitmapImportable() {
}

/**
 * Import the bitmap image to the map pixels
 */
void ColorMapBitmapImportable::ImportBitmap(const char *path, char *bitmapbuf) {
  // TODO: fix to be only permitted the format exported by ColorMapBitmapExportable::ExportBitmap()
  BITMAPFILEHEADER bmpFile;
  BITMAPINFOHEADER bmpInfo;
  std::ifstream fs(std::string(path), std::ios::binary | std::ios::in);
  fs.read(reinterpret_cast<char *>(&bmpFile), sizeof(BITMAPFILEHEADER));
  fs.read(reinterpret_cast<char *>(&bmpInfo), sizeof(BITMAPINFOHEADER));

  fs.read(bitmapbuf, map_->width() * map_->height() * map_->ColorNum);

  float *buf = new float[map_->width() * map_->height() * ColorMap::ColorNum]();
  for (int i = 0; i < map_->width(); ++i) {
    for (int j = 0; j < map_->height(); ++j) {
      buf[(i * map_->width() + j) * 3] = static_cast<float>(bitmapbuf[(i
          + (map_->height() - j - 1) * map_->width()) * 3 + 2]) / 255.0f;
      buf[(i * map_->width() + j) * 3 + 1] = static_cast<float>(bitmapbuf[(i
          + (map_->height() - j - 1) * map_->width()) * 3 + 1]) / 255.0f;
      buf[(i * map_->width() + j) * 3 + 2] = static_cast<float>(bitmapbuf[(i
          + (map_->height() - j - 1) * map_->width()) * 3]) / 255.0f;
    }
  }

  map_->Import(buf);
}
