/* 
 * File:   text_line.cpp
 * Author: clinm
 * 
 * Created on 26 mai 2016, 09:35
 */

#include "text_line.h"

text_line::text_line(CImg<> image) : img(image) {
    upAndLow(img, up_barrier, low_barrier);
}

text_line::text_line(const text_line& orig) : img(orig.img) {
    up_barrier = orig.up_barrier;
    low_barrier = orig.low_barrier;
}

text_line::~text_line() {
}

void text_line::freeVector(vector< text_line*>& vect){
    while (!vect.empty()) {
        delete vect.back();
        vect.pop_back();
    }
}

void text_line::upAndLow(const CImg<>& img, int& up, int& low) {
    vector<int> vertical(projection::horizontalHistogram(img));
    vector<int> diff(projection::secondDifference(vertical));

    up = 0;
    low = 0;
    for (unsigned int j = 0; j < diff.size(); j++) {
        if (diff[j] > diff[up]) {
            low = up;
            up = j;                    
        } else if (diff[j] > diff[low]) {
            low = j;
        }
    }
    if (low < up) {
        int tmp = low;
        low = up;
        up = tmp;
    }/*
    up = 0;
    for (unsigned int j = 0; j < diff.size(); j++) {
        if (diff[j] > diff[up]) {
            up = j;                    
        }
    }
    low = img.height() - up;
    */
}

