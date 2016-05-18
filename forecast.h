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

#include "Utils.h"
#include "clustering_data.h"

#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
using namespace boost::filesystem;

struct forecast_type {
  double probability;
  char character;
  bool operator>(const forecast_type& other) const { return other.probability < probability; }
};

/**
 * Merge all contigus letter from the given list
 * @param list
 */
static void mergeContiguous(vector<forecast_type>& list);

class Forecast {
    vector< CImg<>* > images;
    vector< char > labels;
    
    public:
        Forecast(string average_dataset);
        Forecast(const Forecast& orig);
        virtual ~Forecast(){};
        
        /**
         * Computes the probability of each images given in average for the image
         * @param image     The image that we study
         * @param average   The different average images to compare
         * @param res       The result of the computing
         * @param length    The length of average and res
         * @return          The index of the most probable avergae image
         */
        static int forecast(CImg<> image, vector<forecast_type>& res, vector< CImg<>* > average, vector<char> labels);
        
        /**
         * Computes the same vector but uses images and labels of the object
         */
        int forecast(CImg<> image, vector<forecast_type>& res);
};
#endif	/* FORECAST_H */

