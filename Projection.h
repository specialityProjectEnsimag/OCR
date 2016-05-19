/* 
 * File:   Projection.h
 * Author: clinm
 *
 * Created on 18 mai 2016, 10:06
 */

#ifndef PROJECTION_H
#define	PROJECTION_H

#include <iostream>
#include <vector>
#include "CImg.h"
#include "Utils.h"
#include <stdlib.h>

using namespace cimg_library;

class Projection {
public:
    Projection();
    Projection(const Projection& orig);
    virtual ~Projection();
    
    /**
     * Projects from left to right. Counts the number of white pixels starting
     * from the left
     * @param img The image to project
     * @return A vector where each entry corresponds to the number of white pixel
     *         before a non-white one.
     */
    static std::vector<int> leftward(CImg<> img){
        std::vector<int> sum(img._height, 0);
        
        for(int h = 0; h < img._height; h++){
            int j = 0;
            while(j < img._width && img(j,h) == 255){
                sum.at(h) = sum.at(h) + 1 ;
                j++;
            }
        }
        return sum;
    }
    
    /**
     * Displays the projection as an image. The number of white pixels are displayed
     * in black and the 'shadow' of the form remains in white
     * @param left      The left projection of the image
     * @param width     Its width (in order to keep the image's dimension
     * @return          An image representing the shadow
     */
    static CImg<> leftwardToImg(std::vector<int> left, int width){
        CImg<> img(width, left.size(), 1, 1, 255);
        for(int h = 0; h < left.size(); h++){
            for(int w = 0; w < left.at(h); w++){
                img(w, h) = 0;
            }
        }
        return img;
    }
    
    /**
     * Projects from up to down. Counts the number of white pixels starting
     * from the top
     * @param img The image to project
     * @return A vector where each entry corresponds to the number of white pixel
     *         before a non-white one.
     */   
    static std::vector<int> upward(CImg<> img){
        std::vector<int> sum(img._width, 0);
        
        for(int w = 0; w < img._width; w++){
            int j = 0;
            while(j < img._height && img(w, j) == 255){
                sum.at(w) = sum.at(w) + 1 ;
                j++;
            }
        }
        return sum;
    }
  
    /**
     * Displays the projection as an image. The number of white pixels are displayed
     * in black and the 'shadow' of the form remains in white
     * @param left      The up projection of the image
     * @param width     Its width (in order to keep the image's dimension
     * @return          An image representing the shadow
     */
    static CImg<> upwardToImg(std::vector<int> top, int height){
        CImg<> img(top.size(), height, 1, 1, 255);
        for(int w = 0; w < top.size(); w++){
            for(int h = 0; h < top.at(w); h++){
                img(w, h) = 0;
            }
        }
        return img;
    }
 
    /**
     * Give the first index of the vector which is not equal to 'value'. 
     * If beginning is set to 'TRUE', index is computed from the beginning.
     * If beginning is set to 'FALSE', index is computed from the end
     * @param projection    Result of a projection on one axis
     * @param value         Value
     * @param beginning     TRUE = beginning, FALSE = end
     * @return  The index of the first value non equal to 'value'
     */
    static int firstNonEqual(std::vector<int> projection, int value, bool beginning = true){
        int index;
        if(beginning){
            index = -1;
            do{
                index++;
            }while(index < projection.size() && projection.at(index) == value);
        }else{
            index = projection.size();
            do{
                index--;
            }while(index >= 0 && projection.at(index) == value);

        }
        
        return index;
    }
    
    /**
     * Reduces the image to its core content. Remove all unecessary white borders
     * @param img The image to reduce
     * @return The same image without the white borders
     */
    static CImg<> reduce(CImg<> img){
        vector<int> up = Projection::upward(img);
        int upB = Projection::firstNonEqual(up, img._height);
        int upE = Projection::firstNonEqual(up, img._height, false);

        vector<int> left = Projection::leftward(img);
        int leftB = Projection::firstNonEqual(left, img._width);
        int leftE = Projection::firstNonEqual(left, img._width, false);

        return img.crop(upB, leftB, upE, leftE);
    }
    
    /**
     *  Split the given text into lines 
     * @param text      The text to split
     * @param lines     The line extracted
     */
    static void splitLines(const CImg<>& text, std::vector< CImg<>* >& lines){
        vector<int> sep = Projection::leftward(text);
        
        int state = 1; // 0: blank, 1: something
        int beg = 0;
        
        int full = text._width;
        sep.push_back(full);
        for(int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    CImg<>* elt = new CImg<>(text.get_crop(0, beg, full-1, i-1));
                    lines.push_back(elt);
                }
            }
        }
    }
    
    /**
     * Given a line split into characters
     * @param line         The line to split
     * @param characters    List of characters found
     */
    static void splitCharacters(const CImg<>& line, std::vector< CImg<>* >& characters){
        vector<int> sep = Projection::upward(line);

        int state = 1; // 0: blank, 1: something
        int beg = 0;

        int full = line._height;
        sep.push_back(full);
        for(int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;

                    CImg<>* elt = new CImg<>(line.get_crop(beg, 0, i-1, full-1));
                    //.resize(128, 128, 1, 1))
                    characters.push_back(elt);
                }
            }
        }
    }
    
private:

};

#endif	/* PROJECTION_H */

