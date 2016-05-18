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
            while(img(j,h) == 255 && j < img._width){
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
            while(img(w, j) == 255 && j < img._height){
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
    
private:

};

#endif	/* PROJECTION_H */

