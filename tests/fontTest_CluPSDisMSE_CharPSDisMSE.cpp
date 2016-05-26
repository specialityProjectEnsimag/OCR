/* 
 * File:   fontsTest.cpp
 * Author: jeanselv
 *
 * Created on 24 mai 2016, 15:09:15
 * Statistic on the reconition of simple characters from various fonts 
 * With Clusterstering with preprocessing, skeletonization and distance MSE
 * And Recognition with preprocessing, skeletonization and distance MSE
 * 
 * MAKEFILE : -DSKELETONIZATION
 */

#include "fontTest.h"

string analyse(const char* path, Forecast* f){
    std::stringstream output;
    
    CImg<> img = image_io::import(path);
    CImg<> crop = projection::reduce(img); 

    img = preprocessing::otsu_binarization(img);
    img = preprocessing::skeletonization(img);
    
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
            elt = preprocessing::otsu_binarization( elt);
            elt = preprocessing::skeletonization( elt);
            float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
            CImg<> mask(tab,3,3);
            elt = preprocessing::linear_filter(elt, mask);
            elt = preprocessing::otsu_binarization(elt);
            std::vector<forecast_type>  res;
            f->forecast(elt, res, MSE);
            std::vector<forecast_type>::iterator i = res.begin();
            output << i->character;
       }
       text_character::freeVector(lines);
    }
    text_line::freeVector(split);
    
    return output.str();
}

int main() {
    cout << "Test Character recognition on fonts" << endl;
    
    cout << "Loading OCR ..." << endl;    
    Forecast* f = new Forecast("./result_dataset");
    cout << "End loading !" << endl;
    
    checkfont(f);

    return (EXIT_SUCCESS);
}

