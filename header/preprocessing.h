/* 
 * File:   Preprocessing.h
 * Author: jeanselv
 *
 * Created on 24 mai 2016, 10:02
 */

#ifndef PREPROCESSING_H
#define	PREPROCESSING_H

#include "CImg.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "chamfer.h"

using namespace cimg_library;
using namespace std;

#define BLACK_PIXEL 0
#define WHITE_PIXEL 255

namespace preprocessing
{  
 
    /**
     * Computes the binarization of otsu 
     * @param src   Image in grayscale
     * @return      Binarized image
     */
    CImg<> otsu_binarization(const CImg<>& src);
    
    /**
     * Apply a filter on each neighborhood
     * @param src   Image in grayscale
     * @param mask  Mask to apply on the neighborhood (must be a 3x3 matrix)
     *              Example :
     *                  float prewit_x[9] = { 1, 0, -1, 1, 0, -1, 1, 0, -1};
     *                  float sobel_x[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1};
     *                  float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
     *                  CImg<> mask(tab,3,3);
     * @return      Image with the applied mask
     */
    CImg<> linear_filter(const CImg<>& src, const CImg<>& mask);
    
    /**
     * Apply the median filter on each neighborhood
     * @param src   Image in grayscale
     * @return      Image with the applied filter
     */
    CImg<> median_filter(const CImg<>& src);
    
    /**
     * Fill blanks and reduce noisy black and white image
     * @param src   Image in black and white
     * @return      Cleaner image
     */
    CImg<> noise(const CImg<>& src);
    
    /**
     * Create the squeleton of the image using the Hilditch's Algorithm
     * @param src   Image in black and white
     * @return      Skeletonized image
     */
    CImg<> skeletonization(const CImg<>& src);
    
    /**
     * Modifies the image with noise correction, squeletization and filters
     * @param image Image to modify
     */
    void preprocessing(CImg<>& image);
    
    /**
     * Modifies all images with filters and skeletonisation
     * @param images    Vector of images to modify
     */
    void preprocessing(vector< CImg<>* >& images);
}

#endif	/* PREPROCESSING_H */

