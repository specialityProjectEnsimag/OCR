/* 
 * File:   dataset.h
 * Author: jeanselv
 *
 * Functions linked to the dataset
 * 
 * Created on 26 mai 2016, 10:12
 */

#ifndef DATASET_H
#define	DATASET_H
#include <iostream>
#include <string>
#include <stdlib.h> 
#include "boost/filesystem.hpp"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
using namespace boost::filesystem;

#define NUMBER_LETTER 87

/**
 * Gives the corresponding label
 * @param file  The name of the directory
 * @return      The corresponding label
 */
char corresponding_label(string dir);

/**
 * Bijection which gives a number between 0 and NUMBER_LETTER - 1
 * @param c     The researched char
 * @return      The corresponding integer
 */
int corresponding_int(char c);

/**
 * Bijection which gives a character corresponding to the int between 0 and NUMBER_LETTER - 1
 * @param i     The researched int
 * @return      The corresponding char
 */
char corresponding_char(int i);

#endif	/* DATASET_H */

