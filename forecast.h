/* 
 * File:   forecast.h
 * Author: jeanselv
 *
 * Created on 17 mai 2016, 14:51
 */

#ifndef _FORECAST_H
#define _FORECAST_H

#include <stdlib.h> 
#include <iostream> 
#include <vector> 
#include <math.h> 

#include "CImg.h"
using namespace cimg_library;

struct forecast_type {
  double probability;
  char character;
};

/**
 * Computes the probability of each images given in average for the image
 * @param image     The image that we study
 * @param average   The different average images to compare
 * @param res       The result of the computing
 * @param length    The length of average and res
 * @return          The index of the most probable avergae image
 */
int forecast(CImg<> image, CImg<>* average, forecast_type* res, int length);

#endif	/* FORECAST_H */

