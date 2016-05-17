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
    if(argc < 2){
        cout << "Not enough arguments !" << endl;
        cout << "./main image.png" << endl;
        return 1;
    }

    vector< CImg<>* > list;
    cout << "Importing images from directory.." << endl;
    for(int i = 1; i < argc; i++){
        if(i % (argc/20 + 1) == 1){
            cout << "\r" << (i * 100)/argc  << "%" << flush;
        }
        list.push_back(new CImg<>(Utils::import(argv[i])));
    }
    cout << "\r100%" << endl << "End import" << endl;
    Utils::exportAll("./tmp/", list);
    
    //CImg<> avg = Utils::average(list);
    //Utils::displayImage(avg);
    
    return 0;
}

