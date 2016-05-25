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
        
double Euclidean(const CImg<>& src, const CImg<>& average);

double Chamfer(const CImg<>& src, const CImg<>& average);

double MSE(const CImg<>& src, const CImg<>& average);

#endif	/* DISTANCE_H */

