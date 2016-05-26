/* 
 * File:   text_line.cpp
 * Author: clinm
 * 
 * Created on 26 mai 2016, 09:35
 */

#include "text_line.h"

text_line::text_line(CImg<> image) : img(image) {
}

text_line::text_line(const text_line& orig) : img(orig.img) {
}

text_line::~text_line() {
}

void text_line::freeVector(vector< text_line*>& vect){
    while (!vect.empty()) {
        delete vect.back();
        vect.pop_back();
    }
}

