/* 
 * File:   fontsTest.cpp
 * Author: jeanselv
 *
 * Created on 19 mai 2016, 09:17:50
 * Statistic on the reconition of simple characters from various fonts
 * With Clusterstering with preprocessing and distance MSE
 * And Recognition with preprocessing and distance MSE
 * 
 */

#include "fontTest.h"

string analyse(const char* path, Forecast* f){
    std::stringstream output;
    
    CImg<> img = image_io::import(path);
    CImg<> crop = projection::reduce(img); 

    img = preprocessing::otsu_binarization(img);
    
    vector< CImg<>* > split;
    projection::splitLines(crop, split);    

    for(unsigned int i = 0; i < split.size(); i++){
       vector< CImg<>* > lines;
       CImg<> line = projection::reduce(*split.at(i));
       
       // in order to temporary remove unwanted little pixel
       // will be done with a preprocessing part later
       if(line._width <= 10 || line._height <= 10){
           cout << "\t Too small" << endl;
           continue;
       }
       projection::splitCharacters(line, lines);


       for(unsigned int j = 0; j < lines.size(); j++){
            *lines.at(j) = projection::reduce(*lines.at(j)).resize(SQUARE,SQUARE,-100,-100,3);
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

int main() {
    cout << "Test Character recognition on fonts" << endl;
    
    cout << "Loading OCR ..." << endl;    
    Forecast* f = new Forecast("./result_dataset");
    cout << "End loading !" << endl;
    
    checkfont(f);

    return (EXIT_SUCCESS);
}

