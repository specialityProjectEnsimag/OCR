/* 
 * File:   Utils.h
 * Author: clinm
 *
 * Useful methods to deal with images
 * Created on 17 mai 2016, 13:50
 */

#ifndef IMAGEIO_H
#define	IMAGEIO_H

#define SQUARE 64

#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "projection.h"

#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace boost::filesystem;
using namespace cimg_library;

#define BLACK_PIXEL 0
#define WHITE_PIXEL 255

namespace image_io 
{
    
    /**
     * Displays image and waits for being closed
     * @param img   The image to display
     * @param title Optionnal title for the window
     */
    void displayImage(CImg<>& img, const char* title = "Beautiful image");
    
    /**
     * Imports image stored in 'path' and returns the corresponding grayscale.
     * If an error occured during the import, quits immediatly (exit).
     * @param path  Path to the image
     * @return      Grayscale image
     */
    CImg<> import(const char* path);

    /**
     * Extract all the images from a directory and put them in images
     * The directory has to contain only ".png" files
     * @param path      Path of the directory
     * @param images    Result vector of images
     * @return          True if it is a success, False if it is not
     */
    bool extract_images(const char* path, std::vector< CImg<>* >& images);
    
    /**
     * Free pointers of the vector
     * @param images Vector of pointers
     */
    void delete_images(std::vector< CImg<>* > images);
    
    /**
     * Stores all images contained in list in path_'index'.png when index starts
     *  from 0 and increases
     * @param path Path where images will be written (can contains beginning of
     *             path's name (path/to/file/beginningFileName)
     * @param list  All images to be stored
     */
    void exportAll(const char* path, std::vector< CImg<>* > list);
    
    /**
     * Given a list of images, returns the average image. All images must have
     * the same dimensions.
     * @param list  Images
     * @return      Average images of all images
     */
    CImg<> average(std::vector< CImg<>* > list);
    
};

#endif	/* IMAGEIO_H */

