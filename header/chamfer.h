/* 
 * File:   chamfer.h
 * Author: jeanselv
 *
 * Created on 24 mai 2016, 10:04
 */

#ifndef CHAMFER_H
#define	CHAMFER_H

#include "CImg.h"
#include <stdlib.h>
#include <iostream>

using namespace cimg_library;
using namespace std;

#define BLACK_PIXEL 0
#define WHITE_PIXEL 255

namespace chamfer 
{
    /**
     * Given an image, we compute the chamfer distance
     * @param image  Image binarized
     * @return       Chamfer distance
     */
    CImg<> chamfer(CImg<> image);
    
    CImg<> bin(CImg<> image);
}
#endif	/* CHAMFER_H */

