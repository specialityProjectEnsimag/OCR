/* 
 * File:   text_character.h
 * Author: clinm
 *
 * This class contains all information related to characters
 * 
 */
#include "CImg.h"
using namespace cimg_library;

#include <vector>
using namespace std;

#ifndef TEXT_CHARACTER
#define TEXT_CHARACTER

class text_character
{
public:
	/**
	*	The image that representes the current character
	*/
	CImg<> img;

	/**
	 * If set to true, means that it is the word's last character
	 */
	bool endOfWords;
        
        /** 
         *  Upper and lower case
         * -1 => Do not know
         * 0  => Upper
         * 1  => Middle
         * 2  => Lower
         * 3  => Ponctuation
         */
        int upLow;

	text_character(CImg<> image);
        text_character(CImg<> image, int position);
	text_character(const text_character& orig);
	~text_character();

	static void freeVector(vector< text_character*>& vect);

	
};

#endif