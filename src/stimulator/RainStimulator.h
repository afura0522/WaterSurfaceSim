/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef WATERSURFACESIM_RAINSTIMULATOR_H_
#define WATERSURFACESIM_RAINSTIMULATOR_H_

#include <memory>

#include "stimulator/Stimulator.h"

class RectangleRainStimulator : public Stimulator {
public:
  struct StimulusOption {
    // the area to stimulate
    float areawidth;
    float areaheight;
    // the area to stimulate
    float minforce;
    float maxforce;
    // the number of the stimulus per execution
    float frequency;

    StimulusOption(
        float areawidth,
        float areaheight,
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

  RectangleRainStimulator(const StimulusOption &option, int onstimulatepoint(float, float, float));
  virtual ~RectangleRainStimulator();

  virtual int Execute();

  // acccessor
  inline const StimulusOption &option() const {return option_;}
  inline float areawidth() const {return option_.areawidth;}
  inline float areaheight() const {return option_.areaheight;}
  inline float frequency() const {return option_.frequency;}

  // mutator
  inline void set_option(const StimulusOption &option) {memcpy(&option_, &option, sizeof(StimulusOption));}
  inline void set_areawidth(float areawidth) {option_.areawidth = areawidth;}
  inline void set_areaheight(float areaheight) {option_.areaheight = areaheight;}
  inline void set_frequency(float frequency) {option_.frequency = frequency;}

private:
  StimulusOption option_;
};

#endif /* WATERSURFACESIM_RAINSTIMULATOR_H_ */
