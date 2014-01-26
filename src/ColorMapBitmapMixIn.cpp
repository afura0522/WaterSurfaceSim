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

#include "ColorMapBitmapMixIn.h"

#include <fstream>

#include <Windows.h>

#include "ColorMap.h"

ColorMapBitmapMixIn::ColorMapBitmapMixIn(ColorMap * const map)
    : map_(map),
      bitmapbuf_(NULL) {
}

ColorMapBitmapMixIn::~ColorMapBitmapMixIn() {
  FinalizeBitmapMixIn();
}

void ColorMapBitmapMixIn::InitializeBitmapMixIn() {
  bitmapbuf_ =
      new char[map_->width() * map_->height() * map_->ColorNum]();
}

void ColorMapBitmapMixIn::FinalizeBitmapMixIn() {
  if (NULL != bitmapbuf_) {
    delete[] bitmapbuf_;
    bitmapbuf_ = NULL;
  }
}

void ColorMapBitmapMixIn::OutputBitmap(const char *path) {
  BITMAPFILEHEADER bmpFile;
  bmpFile.bfType = 'MB';
  bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
      + map_->width() * map_->height() * map_->ColorNum;
  bmpFile.bfReserved1 = 0;
  bmpFile.bfReserved2 = 0;
  bmpFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  BITMAPINFOHEADER bmpInfo;
  bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.biWidth = map_->width();
  bmpInfo.biHeight = map_->height();
  bmpInfo.biPlanes = 1;
  bmpInfo.biBitCount = 24;
  bmpInfo.biSizeImage = 0;
  bmpInfo.biCompression = BI_RGB;
  bmpInfo.biXPelsPerMeter = 0;
  bmpInfo.biYPelsPerMeter = 0;
  bmpInfo.biClrUsed = 0;
  bmpInfo.biClrImportant = 0;

  std::ofstream fs(std::string(path), std::ios::binary | std::ios::trunc);
  fs.write(reinterpret_cast<const char *>(&bmpFile), sizeof(BITMAPFILEHEADER));
  fs.write(reinterpret_cast<const char *>(&bmpInfo), sizeof(BITMAPINFOHEADER));
  const float *texture = map_->OutputTexture();
  for (int i = 0; i < map_->width(); ++i) {
    for (int j = 0; j < map_->height(); ++j) {
      bitmapbuf_[(i + (map_->height() - j - 1) * map_->width()) * 3 + 2] =
          static_cast<char>(texture[(i * map_->width() + j) * 3] * 255.0f);
      bitmapbuf_[(i + (map_->height() - j - 1) * map_->width()) * 3 + 1] =
          static_cast<char>(texture[(i * map_->width() + j) * 3 + 1] * 255.0f);
      bitmapbuf_[(i + (map_->height() - j - 1) * map_->width()) * 3] =
          static_cast<char>(texture[(i * map_->width() + j) * 3 + 2] * 255.0f);
    }
  }
  fs.write(bitmapbuf_, map_->width() * map_->height() * map_->ColorNum);
}
