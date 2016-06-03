/* 
 * File:   UpLowCase.cpp
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
#include "sliding_window.h"
#include "hmm.h"
using namespace sliding_window;

using namespace boost::filesystem;
using namespace cimg_library;
using namespace std;

vector<char> analyse_with_up_and_low(const char* path, Forecast* f){
    vector<char> output;
    
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
        splitCharacters(line, lines);
        for(unsigned int j = 0; j < lines.size(); j++){
            text_character elt(lines.at(j)->img);
            elt = projection::reduce(elt,split.at(i)->up_barrier,split.at(i)->low_barrier);
            elt.img.resize(SQUARE,SQUARE,-100,-100,3);
            elt.img = preprocessing::otsu_binarization(elt.img);
            elt.img = preprocessing::skeletonization(elt.img);
            elt.img = chamfer::chamfer(elt.img);
            std::vector<forecast_type>  res;
            f->forecast(elt, res, MSE);
            output.push_back(res.begin()->character);
        }
        output.push_back('\n');
        text_character::freeVector(lines);
    }
    text_line::freeVector(split);
    
    return output;
}

vector<char> analyse(const char* path, Forecast* f){
    vector<char> output;
    
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
       splitCharacters(line, lines);

       for(unsigned int j = 0; j < lines.size(); j++){
            CImg<> elt = projection::reduce(lines.at(j)->img).resize(SQUARE,SQUARE,-100,-100,3);
            elt = preprocessing::otsu_binarization(elt);
            elt = preprocessing::skeletonization(elt);
            elt = chamfer::chamfer(elt);
            std::vector<forecast_type>  res;
            f->forecast(elt, res, MSE);
            output.push_back(res.begin()->character);
       }
       output.push_back('\n');
       text_character::freeVector(lines);
    }
    text_line::freeVector(split);
    
    return output;
}


int main(int argc, char** argv) {
    kmeans("./dataset", "./result");
    cout << "Load Forecast" << endl;
    Forecast forecast("./result");
    for (int img = 1; img < argc; img++) {
        cout << "Test - " << img << endl;
        vector<char> res(analyse(argv[img], &forecast));
        cout << "Analyse without correction : ";
        for (unsigned int i = 0; i < res.size(); i++) {
            cout << res.at(i);
        }
        cout << endl;
        res = analyse_with_up_and_low(argv[img], &forecast);
        cout << "Analyse with correction of case : ";
        for (unsigned int i = 0; i < res.size(); i++) {
            cout << res.at(i);
        }
        cout << endl;
    }

    return (EXIT_SUCCESS);
}

