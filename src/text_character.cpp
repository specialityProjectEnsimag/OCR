#include "text_character.h"

text_character::text_character(CImg<> image) : img(image) {
	endOfWords = false;
}

text_character::~text_character() {
}



void text_character::freeVector(vector< text_character*>& vect){
    while (!vect.empty()) {
        delete vect.back();
        vect.pop_back();
    }
}



