/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef WATERSURFACESIM_RAINSTIMULATOR_H_
#define WATERSURFACESIM_RAINSTIMULATOR_H_

#include <memory>

class RectangleRainStimulator {
public:
  struct StimulusOption {
    // the area to stimulate
    int areawidth;
    int areaheight;
    // the area to stimulate
    float minforce;
    float maxforce;
    // the number of the stimulus per execution
    float frequency;

    StimulusOption(
        int areawidth,
        int areaheight,
        float minforce,
        float maxforce,
        float frequency
    )
    :
    areawidth(areawidth),
    areaheight(areaheight),
    minforce(minforce),
    maxforce(maxforce),
    frequency(frequency)
    {
    }
  };

  RectangleRainStimulator(const StimulusOption &option, int onstimulate(int, int, float));
  virtual ~RectangleRainStimulator();

  int Execute();

  // acccessor
  inline const StimulusOption &option() const {return option_;}
  inline int areawidth() const {return option_.areawidth;}
  inline int areaheight() const {return option_.areaheight;}
  inline float frequency() const {return option_.frequency;}

  // mutator
  inline void set_option(const StimulusOption &option) {memcpy(&option_, &option, sizeof(StimulusOption));}
  inline void set_areawidth(int areawidth) {option_.areawidth = areawidth;}
  inline void set_areaheight(int areaheight) {option_.areaheight = areaheight;}
  inline void set_frequency(float frequency) {option_.frequency = frequency;}

private:
  StimulusOption option_;
  int (*onstimulate_)(int, int, float);
};

#endif /* WATERSURFACESIM_RAINSTIMULATOR_H_ */
