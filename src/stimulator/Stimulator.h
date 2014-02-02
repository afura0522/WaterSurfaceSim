/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef WATERSURFACESIM_STIMULATOR_H_
#define WATERSURFACESIM_STIMULATOR_H_

class Stimulator {
public:
  virtual int Execute() = 0;

protected:
  Stimulator(int (*onstimulatepoint)(float, float, float));
  virtual ~Stimulator();

  inline int StimulatePoint(float x, float y, float force) {
    return onstimulatepoint_(x, y, force);
  }

private:
  int (*onstimulatepoint_)(float, float, float);
};

#endif /* WATERSURFACESIM_STIMULATOR_H_ */
