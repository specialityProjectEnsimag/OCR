/* 
 * File:   overlapping.cpp
 * Author: clinm
 *
 * Created on 26 mai 2016, 12:05
 * 
*/
#include "overlapping.h"

namespace overlapping{
    
    void splitLines(const CImg<>& text, vector< text_line* >& lines){
        sliding_window::splitLines(text, lines);
    }

    void splitCharacters(const CImg<>& line, vector< text_character* >& characters){
        CImg<> img(line);
        CImg<> left, right;
        
        do{
            // adding white column to make sure the splitting always work
            img = img.get_resize(img._width + 1, img._height, -100, -100, 0);
            cimg_forY(img, y){
                img(img._width - 1, y) = 255;
            }

            overlappingSegmentation::splitChar(img, left, right);
            
            CImg<> res(left);
            img = CImg<>(right);
            text_character* c = new text_character(res);

            characters.push_back(c);
        }while(right.min() != WHITE_PIXEL);
    }
    
}