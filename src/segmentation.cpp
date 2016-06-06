/* 
 * File:   overlapping.cpp
 * Author: clinm
 *
 * Created on 26 mai 2016, 12:05
 * 
*/
#include "segmentation.h"

namespace segmentation{
    
    void splitLines(const CImg<>& text, vector< text_line* >& lines){
        sliding_window::splitLines(text, lines);
    }

    void splitCharacters(const CImg<>& line, vector< text_character* >& characters, double threshold){
        vector< text_character* > firstCut;
        sliding_window::splitCharacters(line, firstCut, threshold);

        for(unsigned int ch = 0; ch < firstCut.size(); ch++){
            overlapping::splitCharacters(firstCut[ch]->img, characters);

            // keeping information about end of words
            characters.back()->endOfWords = firstCut[ch]->endOfWords;
        }

        text_character::freeVector(firstCut);
    }
    
}