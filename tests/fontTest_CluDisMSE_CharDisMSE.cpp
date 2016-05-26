/* 
 * File:   fontsTest.cpp
 * Author: jeanselv
 *
 * Created on 24 mai 2016, 15:09:15
 * Statistic on the reconition of simple characters from various fonts
 * With Clusterstering with distance MSE
 * And Recognition with distance MSE
 */

#include "fontTest.h"

string analyse(const char* path, Forecast* f){
    std::stringstream output;
    
    CImg<> img = image_io::import(path);
    CImg<> crop = projection::reduce(img); 

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
            *lines.at(j) = projection::reduce(*lines.at(j)).resize(SQUARE,SQUARE);
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

