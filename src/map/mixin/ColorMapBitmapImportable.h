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

#ifndef __COLORMAPBITMAPIMPORTABLE_H__
#define __COLORMAPBITMAPIMPORTABLE_H__

class ColorMap;

class ColorMapBitmapImportable {
public:
  ColorMapBitmapImportable(ColorMap *const map);
  virtual ~ColorMapBitmapImportable();

  ColorMapBitmapImportable &operator=(const ColorMapBitmapImportable &obj);

  void ImportBitmap(const char *path, char *bitmapbuf);

private:
  ColorMap *const map_;
};

#endif /* __COLORMAPBITMAPIMPORTABLE_H__ */
