#include "text_character.h"

text_character::text_character(CImg<> image) : img(image) {
    upLow = -1;
}

text_character::text_character(CImg<> image, int position) : img(image) {
    upLow = position;
}

text_character::text_character(const text_character& orig) : img(orig.img) {
    upLow = orig.upLow;
}

text_character::~text_character() {
}



void text_character::freeVector(vector< text_character*>& vect){
    while (!vect.empty()) {
        delete vect.back();
        vect.pop_back();
    }
}



