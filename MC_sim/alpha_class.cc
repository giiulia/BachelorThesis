#include "alpha_class.h"
#include <math.h>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES

alpha::alpha(double theta, double phi):
  ang_theta(theta),
  ang_phi(phi)
  {}

alpha::~alpha(){
}

double alpha::get_theta(){
  return ang_theta;
}

double alpha::get_phi(){
  return ang_phi;
}

double alpha::coeff_x(){
  return tan(ang_theta)*cos(ang_phi);
}

double alpha::coeff_y(){
  return tan(ang_theta)*sin(ang_phi);
}
