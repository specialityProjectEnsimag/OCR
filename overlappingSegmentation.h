/* 
 * File:   overlappingSegmentation.h
 * Author: clinm
 *
 * Created on 20 mai 2016, 09:35
 */

/**
 *  This algorithm is an implementation of the publication : 
 *                  Separation of overlapping character
 *                    N. Papamarkos and T. Koutalianos
 *              Department of Electrical and Computer Engineering
 *            Democritus University of Thrace, 67100 Xanthi, Greece
 * 
 *  Explanations on parameters :
 *      - img       Representes the current image to cut
 *      - (x, y)    Representes the current position on the image. The
 *                  coordinates follows the same axis as in the CImg library,
 *                  meaning that (0, 0) will be on top left corner
 *      - solution  Representes the vector of x coordinates of the found path.
 *                  the y coordinate is kept through the index of the vector.
 *                  So solution(y) = x;         
 * 
 *  Explanations on PRINT_DEBUG.
 *      When defined, print_debug will alllow you to see the progression of the
 *      algorithm. You have to set colored as a copy of the image. Everytime the
 *      algorithm goes on a white pixel, it will make it a little bit darker so
 *      you can see if it goes several times on the same pixel.
 * 
 *      PRINT_DEBUG is also used in the associated test file to display the result
 */


#ifndef OVERLAPPINGSEGMENTATION_H
#define	OVERLAPPINGSEGMENTATION_H

#include "Utils.h"
#include <vector>

#include "CImg.h"
using namespace cimg_library;

#define WHITE_PIXEL 255

#include <iostream>
using namespace std;

namespace overlappingSegmentation
{
    
    #ifdef PRINT_DEBUG
        extern CImg<> colored;
    #endif
    
    
    /**
     *  Keep track of the path. When found is set to true, means a path has been
     * found and all other fields are set to -1. Otherwise the coordinates
     * (lx, ly) and (rx, ry) represents the left and right obstacles
     */
    struct sol{
        bool found;     /*!<  Whether a path to the top was found */
        int lx;         /*!<  x position where the left path is blocked */
        int ly;         /*!<  y position where the left path is blocked */
        int rx;         /*!<  x position where the right path is blocked */
        int ry;         /*!<  y position where the right path is blocked */       
    };
    

    /**
     * Try to find a way up.
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveToTop(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
    
    /**
     * Move along x axis on the given y to the left and always try to go up.
     * If blocked by a black pixel, fill (lx, ly) of path with the last white 
     * pixel (and also the current position). If it finds a way up, call moveToTop
     * on the TOP PIXEL available (important)
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveTopLeft(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
    
    /**
     * Move along x axis on the given y to the right and always try to go up.
     * If blocked by a black pixel, fill (rx, ry) of path with the last white
     * pixel (and also the current position). If it finds a way up, call moveToTop
     * on the TOP PIXEL available (important)
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveTopRight(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
    
    /**
     * Same as moveToLeft but it does not look for a way up before (destX, y), then calls
     * moveToLeft in order to restore the 'normal' behaviour. This functions allows to
     * bypass a range of columns that has been already checked.
     * @param img
     * @param path
     * @param x
     * @param y
     * @param destX
     * @param solution
     */
    void moveTopLeftUntil(const CImg<>& img, sol& path, int x, int y, int destX, const vector<int>& solution);
    
    /**
     * Same as moveToRight but it does not look for a way up before (destX, y), then calls
     * moveToRight in order to restore the 'normal' behaviour. This functions allows to
     * bypass a range of columns that has been already checked.
     * @param img
     * @param path
     * @param x
     * @param y
     * @param destX
     * @param solution
     */
    void moveTopRightUntil(const CImg<>& img, sol& path, int x, int y, int destX, const vector<int>& solution);
    
    /**
     * Try to find a way down. All functions to find a way down are similar to
     * the ones looking for a way up. The only difference is in moveToBottomInternal
     * where when we reach a black pixel, the order we have to look for a way down
     * is different. (more information in the code)
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveToBottom(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
        
    /**
     * Same as moveTopLeft execpt that it is looking for a way down
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveBottomLeft(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
    
    /**
     * Same as moveTopRight execpt that it is looking for a way down
     * @param img
     * @param path
     * @param x
     * @param y
     * @param solution
     */
    void moveBottomRight(const CImg<>& img, sol& path, int x, int y, const vector<int>& solution);
    
    /**
     * Same as moveTopLeftUntil
     * @param img
     * @param path
     * @param x
     * @param y
     * @param destX
     * @param solution
     */
    void moveBottomLeftUntil(const CImg<>& img, sol& path, int x, int y, int destX, const vector<int>& solution);
    
    /**
     * Same as moveTopRightUntil
     * @param img
     * @param path
     * @param x
     * @param y
     * @param destX
     * @param solution
     */
    void moveBottomRightUntil(const CImg<>& img, sol& path, int x, int y, int destX, const vector<int>& solution);
}

#endif	/* OVERLAPPINGSEGMENTATION_H */

