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

	text_character(CImg<> image);
	text_character(const text_character& orig);
	~text_character();

	static void freeVector(vector< text_character*>& vect);

	
};

#endif