/* 
 * File:   distance.h
 * Author: jeanselv
 *
 * Created on 24 mai 2016, 15:39
 */

#ifndef DISTANCE_H
#define	DISTANCE_H

#include "CImg.h"
#include "chamfer.h"
using namespace cimg_library;
        
double Euclidean(CImg<> src, CImg<> average);

double Chamfer(CImg<> src, CImg<> average);

double MSE(CImg<> src, CImg<> average);

#endif	/* DISTANCE_H */

