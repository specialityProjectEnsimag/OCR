/* 
 * File:   sliding_window.cpp
 * Author: clinm
 *
 * Created on 26 mai 2016, 09:11
 * 
*/
#include "sliding_window.h"

namespace sliding_window{
    
    void splitLines(const CImg<>& text, vector< text_line* >& lines){
        vector<int> sep = projection::leftward(text);

        int state = 1; // 0: blank, 1: something
        int beg = 0;

        int full = text._width;
        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    text_line* l = new text_line(CImg<>(text.get_crop(0, beg, full-1, i-1)));
                    lines.push_back(l);
                }
            }
        }   
    }
    
    void splitCharacters(const CImg<>& line, vector< text_character* >& characters){
        vector<int> sep = projection::upward(line);

        int state = 1; // 0: blank, 1: something
        int beg = 0;

        int full = line._height;
        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                        
                    text_character* c = new text_character(CImg<>(line.get_crop(beg, 0, i-1, full-1)));

                    characters.push_back(c);
                }
            }
        }
    }
    
}