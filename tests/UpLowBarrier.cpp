/* 
 * File:   UpLowBarrier.cpp
 * Author: jeanselv
 *
 * Created on 31 mai 2016, 09:52:10
 */


#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image_io.h"
#include "clustering_data.h"
#include "CImg.h"
#include "distance.h"
#include "projection.h"
#include "forecast.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "boost/filesystem.hpp"
#include "CImg.h"
#include <iomanip>
#include "text_character.h"
#include "overlapping.h"
#include "hmm.h"
using namespace overlapping;

using namespace boost::filesystem;
using namespace cimg_library;
using namespace std;

void analyse_with_up_and_low(const char* path){
    CImg<> img = image_io::import(path);
    CImg<> crop = projection::reduce(img); 

    img = preprocessing::otsu_binarization(img);
    
    vector< text_line* > split;
    splitLines(crop, split);    
    for(unsigned int i = 0; i < split.size(); i++){
       vector< text_character* > lines;
       CImg<> line = projection::reduce(split.at(i)->img);
       
       // in order to temporary remove unwanted little pixel
       // will be done with a preprocessing part later
       if(line._width <= 10 || line._height <= 10){
           continue;
       }
        vector<int> vertical(projection::horizontalHistogram(split.at(i)->img));
        vector<int> diff(projection::secondDifference(vertical));

        assert(vertical.size() == diff.size());
        cimg_forXY(split.at(i)->img, x, y){
            if (x < diff[y] && (y == split.at(i)->low_barrier || y == split.at(i)->up_barrier)) {
                (split.at(i)->img)(x,y) = 200;
            }
        }
        image_io::displayImage(split.at(i)->img); 

    }
    text_line::freeVector(split);
}

int main(int argc, char** argv) {
    kmeans("./dataset", "./result");
    cout << "Load Forecast" << endl;
    Forecast forecast("./result");
    for (int img = 1; img < argc; img++) {
        cout << "Test - " << img << endl;
        analyse_with_up_and_low(argv[img]);
    }

    return (EXIT_SUCCESS);
}


