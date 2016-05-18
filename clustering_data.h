/* 
 * File:   clustering_data.h
 * Author: jeanselv
 *
 * Created on 17 mai 2016, 14:51
 */

#ifndef _CLUSTERING_H
#define _CLUSTERING_H

#include <iostream> 
#include <stdlib.h> 
#include <string> 
#include <iterator>

#include "forecast.h"
#include "Utils.h"

#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
using namespace boost::filesystem;

/**
 * Computes the Kmeans algorithm on all the images from the directory into
 * k images in the average dataset
 * @param images            The images of the directory
 * @param label             The letter corresponding to the directory
 * @param k                 The number of needed average images
 * @param iteration         The number of iterations of the algorithm
 * @param average_dataset   The target directory
 */
void kmeans_directory(vector< CImg<>* >& images, char label, int k, int iteration, string average_dataset);

/**
 * Computes the kmeans on the dataset and creates average images in the 
 * average_dataset with k images for each subdirectory of dataset
 * @param dataset           The source directory which should contain only sub directories
 * @param k                 The number of avergae images to create for each sub directories
 * @param iteration         The number of iterations for the clustering using Kmeans algorithm
 * @param average_dataset   The target directory
 * @return                  True if it is a success, False otherwise
 */
bool kmeans(string dataset, string average_dataset, int k = 50, int iteration = 50);

/**
 * Gives the corresponding label
 * @param file  The name of the directory
 * @return      The corresponding label
 */
char corresponding_label(string dir);

#endif
