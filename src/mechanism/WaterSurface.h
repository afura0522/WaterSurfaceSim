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

#ifndef WATERSURFACESIM_WATERSURFACE_H_
#define WATERSURFACESIM_WATERSURFACE_H_

#include <cstring>

class WaterSurface {
public:
  WaterSurface(int width, int height, float propagation, float attenuation);
  virtual ~WaterSurface();

  virtual void Initialize();
  virtual void Finalize();

  virtual void Execute();
  virtual void ClearAll();

  inline void set_currheight(int x, int y, float height) {currheights_[y][x] = height;}
  inline float currheight(int x, int y) const {return currheights_[y][x];}
  inline void set_prevheight(int x, int y, float height) {prevheights_[y][x] = height;}
  inline float prevheight(int x, int y) const {return prevheights_[y][x];}

private:
  int width_;
  int height_;
  float propagation_;
  float attenuation_;
  float *prevheightbuf_;
  float *currheightbuf_;
  float *nextheightbuf_;
  float **prevheights_;
  float **currheights_;
  float **nextheights_;
};

#endif /* WATERSURFACESIM_WATERSURFACE_H_ */
