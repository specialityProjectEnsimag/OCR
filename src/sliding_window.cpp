/* 
 * File:   sliding_window.cpp
 * Author: clinm
 *
 * Created on 26 mai 2016, 09:11
 * 
*/
#include "sliding_window.h"

namespace sliding_window{
    
    void splitLines(const CImg<>& text, vector< text_line* >& lines, double paragraphThreshold){
        vector<int> sep = projection::leftward(text);

        int state = 1; // 0: blank, 1: something
        int beg = 0;
        int space = 0;
        int full = text._width;
        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }else{
                    space ++;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    text_line* l = new text_line(CImg<>(text.get_crop(0, beg, full-1, i-1)));
                    if(space >= paragraphThreshold){
                        if(lines.size() > 0){
                            lines.back()->endOfParagraph = true;
                        }
                    }

                    lines.push_back(l);
                    space = 0;
                }
            }
        }  

        lines.back()->endOfParagraph = true;

    }
    
    void splitCharacters(const CImg<>& line, vector< text_character* >& characters, double wordThreshold){
        vector<int> sep = projection::upward(line);

        int state = 1; // 0: blank, 1: something
        int beg = 0;
        int space = 0;
        int full = line._height;
        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }else{
                    space ++;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    text_character* c = new text_character(CImg<>(line.get_crop(beg, 0, i-1, full-1)));
                    if(space >= wordThreshold){
                        if(characters.size() > 0){
                            characters.back()->endOfWords = true;
                        }
                    }


                    characters.push_back(c);
                    space = 0;
                }
            }
        }
        characters.back()->endOfWords = true;
    }
    
}