/* 
 * File:   main.cpp
 * Author: clinm
 *
 * Created on 17 mai 2016, 10:32
 */

#include <cstdlib>

#include <iostream>
#include <vector>
#include "Utils.h"
#include "clustering_data.h"
#include "CImg.h"


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
    return 0;
}

