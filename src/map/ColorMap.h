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

#ifndef WATERSURFACESIM_COLORMAP_H_
#define WATERSURFACESIM_COLORMAP_H_

#include <cstring>

class ColorMap {
public:
  enum {ColorNum = 3};
  static const float WhiteColor[ColorNum];

  ColorMap(int width, int height);
  virtual ~ColorMap();

  virtual void Initialize();
  virtual void Finalize();

  virtual void Execute() = 0;

  virtual void ClearAll();
  virtual const float *OutputTexture(const float (&basecolor)[ColorNum] = WhiteColor);

  // accessor
  inline int width() const {
    return width_;
  }
  inline int height() const {
    return height_;
  }
  inline float pixel(int x, int y, int rgb) const {
    return pixels_[y][x][rgb];
  }
  inline void pixel(int x, int y, float (&color)[ColorMap::ColorNum]) const {
    memcpy(color, pixels_[y][x], sizeof(color));
  }
  inline float texturebuf(int x, int y, int rgb) const {
    return texturebuf_[(x * height_ + y) * ColorNum + rgb];
  }
  inline void texturebuf(int x, int y, float (&color)[ColorMap::ColorNum]) const {
    memcpy(color, &(texturebuf_[(x * height_ + y) * ColorNum]), sizeof(color));
  }

protected:
  // mutator
  inline void set_pixel(int x, int y, int rgb, float val) {
    pixels_[y][x][rgb] = val;
  }
  inline void set_pixel(int x, int y, const float (&color)[ColorMap::ColorNum]) {
    memcpy(pixels_[y][x], color, sizeof(float) * ColorMap::ColorNum);
  }
  inline void set_texturebuf(int x, int y, int rgb, float val) {
    texturebuf_[(x * height_ + y) * ColorNum + rgb] = val;
  }
  inline void set_texturebuf(int x, int y, const float (&color)[ColorMap::ColorNum]) {
    memcpy(&(texturebuf_[(x * height_ + y) * ColorNum]), color, sizeof(color));
  }
  inline void set_texturebufflush(bool val) {
    texturebufflush_ = val;
  }

private:
  int width_;
  int height_;
  float ***pixels_;
  float *pixelbuf_;
  float *texturebuf_;
  bool texturebufflush_;
};

#endif /* WATERSURFACESIM_COLORMAP_H_ */
