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
#include "CImg.h"

#include "projection.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
    // in order to remove unwanted error in output
    //cimg::exception_mode(0);
    
    cout << "Welcome !" << endl;
 
    CImg<> img1 = image_io::import(argv[1]);
    preprocessing::preprocessing(img1);
        CImgDisplay main_disp(img1, " ",0);
        while (!main_disp.is_closed()){
            main_disp.wait();
        }
}