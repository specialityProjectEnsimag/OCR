/* 
 * File:   overlapping.h
 * Author: clinm
 *
 * Created on 26 mai 2016, 12:02
 * 
 * Implementation of the overlapping segmentation algorithm to detect characters.
 * This method is only used on characters and as no effect one line.
 * This method is effective when:
 *      - Characters are separated (same as sliding window)
 *      - Characters are overlapped (but at least one white pixel between two characters)
 * 
 */

#include "CImg.h"
#include "text_line.h"
#include "text_character.h"
#include "projection.h"
#include "sliding_window.h"
 
#ifndef OVERLAPPING_H
#define	OVERLAPPING_H


namespace overlapping{

    /**
     * Call sliding_window::splitLines
     * @param text      The text to split, the image has to be reduced
     * @param lines     The line extracted with no information
     */
    void splitLines(const CImg<>& text, vector< text_line* >& lines);

    /**
     * Given a line split into characters
     * @param line         The line to split
     * @param characters    List of characters found
     */
    void splitCharacters(const CImg<>& line, vector< text_character* >& characters);

}

#endif	/* OVERLAPPING_H */

