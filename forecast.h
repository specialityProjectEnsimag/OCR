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

#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
using namespace boost::filesystem;

struct forecast_type {
  double probability;
  char character;
  bool operator<(const forecast_type& other) const { return other.probability < probability; }
};

class Forecast {
    vector< CImg<>* > images;
    vector< char > labels;
    
    public:
        Forecast() {};
        Forecast(const Forecast& orig){};
        virtual ~Forecast(){};
        
        /**
         * Test the image and compute the most probable image
         * @param average_dataset   The name of the dataset 
         * @return                  The forecast sorted by probability
         */
        vector<forecast_type>* testLetter(CImg<> toTest, string average_dataset);
        
        /**
         * Computes the probability of each images given in average for the image
         * @param image     The image that we study
         * @param average   The different average images to compare
         * @param res       The result of the computing
         * @param length    The length of average and res
         * @return          The index of the most probable avergae image
         */
        static int forecast(CImg<> image, vector< CImg<>* > average, vector<char> labels, vector<forecast_type>& res);

        /**
         * Merge all contigus letter from the given list
         * @param list
         */
        static void mergeContiguous(vector<forecast_type>& list);

};
#endif	/* FORECAST_H */

