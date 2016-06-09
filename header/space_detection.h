/**
 * File:   space_detection.h
 * Author: clinm
 * 
 * Created on 03 june 2016 
 */

#include "CImg.h"
#include "projection.h"
#include <iostream>
#include <vector>
#include "text_line.h"
#include "image_io.h"

#ifndef SPACE_DETECTION_H
#define	SPACE_DETECTION_H


namespace space_detection{
    
    /**
     * Compute the threshold in order to detect words
     * Compute the means of space distance between characters
     * @param lines Text's line
     * @return Threshold, under this value it should be a space between character
     *                    above this value it should be a space between words
     */
    double getSpaceThreshold(const vector< text_line* >& lines);

    /**
     * Compute the threshold in order to detect paragraphs
     * @param document	The document to which we compute the threshold
     * @return Threshold, under this value it should be a space between lines
     * 					  above this value it should be a new paragraph
     */
	double getParagraphThreshold(const CImg<>& document);
}

#endif	/* SPACE_DETECTION_H */

