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

#include "FPSCounter.h"

FPSCounter::FPSCounter(int intervaltime, int (&gettimefunc)())
    : intervaltime_(intervaltime),
      gettimefunc_(gettimefunc),
      count_(0),
      countstarttime_(0),
      fps_(0) {
}

FPSCounter::~FPSCounter() {
}

int FPSCounter::Update() {
  // intervaltime_が0以下の時は更新しない
  if (0 >= intervaltime_) {
    return 1;
  }

  // フレーム数をカウント
  ++count_;

  //
  int nowtime = gettimefunc_();
  int elapsedtime = nowtime - countstarttime_;
  if (elapsedtime > intervaltime_) {
    //計測
    fps_ = (intervaltime_ * count_ / elapsedtime) / (intervaltime_ / 1000);

    //初期化
    countstarttime_ = nowtime;
    count_ = 0;
  }

  return 0;
}

