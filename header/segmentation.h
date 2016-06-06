/**
 * File:   segmentation.h
 * Author: clinm
 * 
 * Algorithm using the more advanced implementation so far. By mixing both sliding_window and
 * overlapping segmentation we are able to split quicly both lines and characters.
 */
#include "CImg.h"
#include "text_line.h"
#include "text_character.h"
#include "projection.h"
#include "sliding_window.h"
#include "overlapping.h"
#ifndef SEGMENTATION_H
#define	SEGMENTATION_H


namespace segmentation{
    
    /**
     * Split the given text into lines.
     * @param text      The text to split, the image has to be reduced
     * @param lines     The line extracted with no information
     */
    void splitLines(const CImg<>& text, vector< text_line* >& lines);

    /**
     * Given a line split into characters. When wordThreshold is given, will add information about
     * whether the character is the last of a word of not.
     * @param line         The line to split
     * @param characters    List of characters found
     * @param wordThreshold Threshold value to detect word, default to -1 (don't look for words)
     */
    void splitCharacters(const CImg<>& line, vector< text_character* >& characters, double wordThreshold = -1);

}

#endif	/* SEGMENTATION_H */

