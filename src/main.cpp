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
#include "image_io.h"
#include "preprocessing.h"
#include "clustering_data.h"
#include "distance.h"
#include "CImg.h"

#include "projection.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
    // in order to remove unwanted error in output
    //cimg::exception_mode(0);
    
    cout << "Welcome !" << endl;
    
    CImg<> img = image_io::import(argv[1]);

    float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
    CImg<> mask(tab,3,3);
    img = preprocessing::linear_filter(img, mask);
    //img = preprocessing::median_filter(img);
    img = preprocessing::otsu_binarization(img);
    img.save("lineaire.png");
    
}