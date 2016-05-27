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

#include "dataset.h"
#include "image_io.h"
#include "clustering_data.h"

#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
using namespace boost::filesystem;

// Best experimental value
#define NUMBERNEIGHT 2

struct forecast_type {
  double probability;
  char character;
  bool operator>(const forecast_type& other) const { return other.probability < probability; }
  bool operator<(const forecast_type& other) const { return other.probability > probability; }
};

class Forecast {
    vector< CImg<>* > images;
    vector< char > labels;
    
    public:
        Forecast(string average_dataset);
        Forecast(const Forecast& orig);
        virtual ~Forecast(){image_io::delete_images(images);};
        
        /**
         * Computes the probability of each images given in average for the image
         * @param image     The image that we study
         * @param average   The different average images to compare
         * @param labels   The labels of the different average
         * @param res       The result of the computing
         * @param dist      The ditance to use
         */
        static void forecast(const CImg<>& image, vector<forecast_type>& res, const vector< CImg<>* >& average, const vector<char>& labels,double (*dist)(const CImg<>&, const CImg<>&));

        /**
         * Computes the same vector but uses images and labels of the object
         */
        void forecast(const CImg<>& image, vector<forecast_type>& res, double (*dist)(const CImg<>&, const CImg<>&)){
            forecast(image, res, this->images, this->labels, dist);
        }
        
        /**
         * Computes the most probable image 
         * @param image     The image that we study
         * @param average   The different average images to compare
         * @param dist      The ditance to use
         * @return          The index of the closest average image
         */
        static int indexOfClosest(const CImg<>& image, const vector< CImg<>* >& average, double (*dist)(const CImg<>&, const CImg<>&));
        
};
#endif	/* FORECAST_H */

