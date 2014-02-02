/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef WATERSURFACESIM_WALKERSTIMULATOR_H_
#define WATERSURFACESIM_WALKERSTIMULATOR_H_

#include "stimulator/Stimulator.h"

/**
 * Stimulate as like the footprint made by human walk
 */
class WalkerStimulator : public Stimulator {
public:
  /**
   * defaultposx, defaultposy: The start position of walker
   * speed: The walk speed of walk
   * turnspeed: The turn speed of walk
   * force: The force to stimulate
   * footwidth: The width of the footprints of the both walker feet
   * footwidthfluct: The fluctuation of the foot width on every update
   */
  struct StimulusOption {
    float defaultposx;
    float defaultposy;
    float speed;
    float turnspeed;
    float force;
    float footwidth;
    float footwidthfluct;

    StimulusOption(float defaultposx, float defaultposy, float speed, float turnspeed, float force, float footwidth, float footwidthfluct)
    :
    defaultposx(defaultposx),
    defaultposy(defaultposy),
    speed(speed),
    turnspeed(turnspeed),
    force(force),
    footwidth(footwidth),
    footwidthfluct(footwidthfluct)
    {
    }
  };

  /**
   * Constructor
   */
  WalkerStimulator(const StimulusOption& option, int (*onstimulatepoint)(float, float, float));

  /**
   * Destructor
   */
  virtual ~WalkerStimulator();

  /**
   * Set the walker behavior on the next execution.
   * If you don't call, the walker is no move on the next execution.
   */
  void Walk(int go, int turn);

  /**
   * Update the walker motion.
   * You need to call every frame as long as you don't want to pause the execution.
   */
  virtual int Execute();

private:
  StimulusOption option_;
  int go_;
  int turn_;
  float posx_;
  float posy_;
  float dir_;
  bool foot_;
};

#endif /* WATERSURFACESIM_WALKERSTIMULATOR_H_ */
