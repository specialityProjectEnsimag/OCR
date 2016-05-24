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
    
    vector< CImg<>* > split;
    projection::splitLines(crop, split);    

    for(int i = 0; i < split.size(); i++){
       vector< CImg<>* > lines;
       CImg<> line = projection::reduce(*split.at(i));
       
       // in order to temporary remove unwanted little pixel
       // will be done with a preprocessing part later
       if(line._width <= 10 || line._height <= 10){
           cout << "\t Too small" << endl;
           continue;
       }
       projection::splitCharacters(line, lines);


       for(int j = 0; j < lines.size(); j++){
            *lines.at(j) = projection::reduce(*lines.at(j)).resize(SQUARE,SQUARE,-100,-100,3);
            *lines.at(j) = preprocessing::otsu_binarization( *lines.at(j));
            *lines.at(j) = preprocessing::skeletonization( *lines.at(j));
            float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
            CImg<> mask(tab,3,3);
            *lines.at(j) = preprocessing::linear_filter(*lines.at(j), mask);
            *lines.at(j) = preprocessing::otsu_binarization(*lines.at(j));
            std::vector<forecast_type>  res;
            f->forecast(*lines.at(j), res, MSE);
            std::vector<forecast_type>::iterator i = res.begin();
            output << i->character;
       }
       image_io::delete_images(lines);
    }
    image_io::delete_images(split);
    
    return output.str();
}

int main(int argc, char** argv) {
    cout << "Test Character recognition on fonts" << endl;
    
    cout << "Loading OCR ..." << endl;    
    Forecast* f = new Forecast("./result_dataset");
    cout << "End loading !" << endl;
    
    checkfont(f);

    return (EXIT_SUCCESS);
}

