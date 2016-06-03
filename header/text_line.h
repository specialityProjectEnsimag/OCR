/* 
 * File:   text_line.h
 * Author: clinm
 *
 * Created on 26 mai 2016, 09:35
 * 
 * This class contains all information related to lines
 * 
 */

#include "projection.h"

#include "CImg.h"
using namespace cimg_library;

#include <vector>
using namespace std;

#ifndef TEXT_LINE_H
#define	TEXT_LINE_H


class text_line {
public:
    CImg<> img;
    
    int up_barrier;
    int low_barrier;

    text_line(CImg<> image);
    text_line(const text_line& orig);
    virtual ~text_line();

    static void freeVector(vector< text_line*>& vect);

private:
    /**
     * Computes the up and low barriers of the line
     * 
     * @param img   The line in black and white
     * @param up    The result of up barrier
     * @param low   The reuslt of low barrier
     */
    void upAndLow(const CImg<>& img, int& up, int& low);

};

#endif	/* TEXT_LINE_H */

