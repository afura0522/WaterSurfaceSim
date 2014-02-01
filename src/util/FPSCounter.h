/**
 * Copyright (C) 2014 The Motel On Jupiter
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

#ifndef WATERSURFACESIM_FPSCOUNTER_H_
#define WATERSURFACESIM_FPSCOUNTER_H_

class FPSCounter {
public:
  FPSCounter(int intervaltime, int (&getTime)());
  virtual ~FPSCounter();

  int Update();

  // accessor
  inline int fps() const {return fps_;}

  // mutator
  inline void set_intervaltime(int time) {intervaltime_ = time;}

private:
  int intervaltime_;
  int (&gettimefunc_)();
  int count_;
  int countstarttime_;
  int fps_;
};

#endif /* WATERSURFACESIM_FPSCOUNTER_H_ */
