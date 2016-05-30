/* 
 * File:   hmm.cpp
 * Author: jeanselv
 *
 * Created on 27 mai 2016, 09:00:13
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

#include "overlapping.h"
#include "hmm.h"
using namespace overlapping;

using namespace boost::filesystem;
using namespace cimg_library;
using namespace std;

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
           cout << "\t Too small" << endl;
           continue;
       }
       splitCharacters(line, lines);


       for(unsigned int j = 0; j < lines.size(); j++){
            CImg<> elt = projection::reduce(lines.at(j)->img).resize(SQUARE,SQUARE,-100,-100,3);
            std::vector<forecast_type>  res;
            f->forecast(elt, res, MSE);
            std::vector<forecast_type>::iterator i = res.begin();
            output.push_back(i->character);
       }
       text_character::freeVector(lines);
    }
    text_line::freeVector(split);
    
    return output;
}

int main(int argc, char** argv) {
    kmeans("./dataset", "./result");
    cout << "Load Forecast" << endl;
    Forecast forecast("./result");
    HMM hmm = HMM();
    {
        cout << "Load hmm" << endl;
        std::ifstream ifs("hmm_save");
        boost::archive::text_iarchive ia(ifs);
        ia >> hmm;
    }    
    
    vector<char> res(analyse(argv[1], &forecast));
    cout << "Analyse without hmm : ";
    for (unsigned int i = 0; i < res.size(); i++) {
        cout << res.at(i);
    }
    cout << endl;
    res = hmm.viterbi(res);
    cout << "Analyse with hmm : ";
    for (unsigned int i = 0; i < res.size(); i++) {
        cout << res.at(i);
    }
    cout << endl;
    return (EXIT_SUCCESS);
}


