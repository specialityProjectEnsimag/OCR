/** 
 * File:   sliding_window.h
 * Author: clinm
 *
 * Created on 26 mai 2016, 09:11
 * 
 * Implementation of the sliding window algorithm to detect lines and characters
 * in an image. This method is effective when:
 *      - Lines are properly separated (at least one row full of white pixels
 *      - There is no imperfection in the image (one black pixel would result as 
 *          a new line which is not correct
 *      - Characters are properly separated(at least one column full of white pixels
 */

#include "CImg.h"
#include "text_line.h"
#include "text_character.h"
#include "projection.h"

#ifndef SLIDING_WINDOW_H
#define	SLIDING_WINDOW_H


namespace sliding_window{
    
    /**
     * Split the given text into lines.
     * @param text                  The text to split, the image has to be reduced
     * @param lines                 The line extracted with no information
     * @param paragraphThreshold    Threshold value to detect paragraphs
     */
    void splitLines(const CImg<>& text, vector< text_line* >& lines, double paragraphThreshold = -1);

    /**
     * Given a line split into characters. When wordThreshold is given, will add information about
     * whether the character is the last of a word of not.
     * @param line          The line to split
     * @param characters    List of characters found
     * @param wordThreshold Threshold value to detect words, default to -1 (don't look for words)
     */
    void splitCharacters(const CImg<>& line, vector< text_character* >& characters, double wordThreshold = -1);

}

#endif	/* SLIDING_WINDOW_H */

