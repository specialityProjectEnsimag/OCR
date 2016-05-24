/* 
 * File:   main.cpp
 * Author: clinm
 *
 * Created on 17 mai 2016, 10:32
 */
#define cimg_verbosity 3
#include <cstdlib>

#include <iostream>
#include <vector>
#include "Utils.h"
#include "clustering_data.h"
#include "CImg.h"

#include "Projection.h"

using namespace cimg_library;
using namespace std;


/*
 * 
 */
int main(int argc, char** argv) {
    // in order to remove unwanted error in output
    //cimg::exception_mode(0);
    
    cout << "Welcome !" << endl;
    kmeans("./dataset", "./result_dataset");
    if(argc < 2){
        cout << "Not enough arguments !" << endl;
        cout << "./main image.png" << endl;
        return 1;
    }
    
    int total = 0;
 
    CImg<> img = Utils::import(argv[1]);
    CImg<> crop = Projection::reduce(img);

    vector< CImg<>* > split;
    Projection::splitLines(crop, split);    
    cout << "Lines: " << split.size() << endl;
    for(int i = 0; i < split.size(); i++){
       vector< CImg<>* > lines;
       CImg<> line = Projection::reduce(*split.at(i));
       
       // in order to temporary remove unwanted little pixel
       // will be done with a preprocessing part later
       if(line._width <= 10 || line._height <= 10){
           cout << "\t Too small" << endl;
           continue;
       }
       Projection::splitCharacters(line, lines);
       cout << "\tCharacters: " << lines.size() << endl;
       total += lines.size();
       Forecast f("./result_dataset");
       for(int j = 0; j < lines.size(); j++){
            *lines.at(j) = Projection::reduce(*lines.at(j)).resize(SQUARE,SQUARE);
            Utils::displayImage(*lines.at(j));
            std::vector<forecast_type>  res;
            f.forecast(*lines.at(j), res);
            std::vector<forecast_type>::iterator i = res.begin();
            int nbr = 0;
            while (i != res.end() && nbr < 10) {
                std::cout << i->character << " : " << i->probability << endl;    
                nbr++;
                i++;
            }
       }
       Utils::delete_images(lines);
    }
    Utils::delete_images(split);
    
    cout << "Total: " << total << endl;

    return 0;
}

