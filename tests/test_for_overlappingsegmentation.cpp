/* 
 * File:   test_for_overlappingsegmentation.cpp
 * Author: clinm
 *
 * Created on 20 mai 2016, 09:37:18
 */

/**
 * This test file will provide more information if PRINT_DEBUG is defined
 */

#include <stdlib.h>
#include <iostream>
#include <assert.h> 
#include <iostream>
using namespace std;

#include "CImg.h"
using namespace cimg_library;

#include "overlappingSegmentation.h"
#include "Utils.h"
using namespace overlappingSegmentation;

#define FOLDER "./overlapping/"

bool areEquals(sol path, const sol expected){
    if(path.found == expected.found &&
            path.lx == expected.lx &&
            path.ly == expected.ly &&
            path.rx == expected.rx &&
            path.ry == expected.ry){
        return true;
    }else{
        cout << "(" << path.found << ", " ;
        cout << path.lx << ", " << path.ly; 
        cout << ", " << path.rx << ", " << path.ry << ")";
        cout << " expected (" << path.found << ", " ;
        cout << expected.lx << ", " << expected.ly ;
        cout << ", " << expected.rx << ", " << expected.ry << ")" << endl;
        return false;
    }
}

#ifdef PRINT_DEBUG
    #define displayIfDebug(img){                        \
            Utils::displayImage(img.resize(300, 300));  \
    }

    #define initTrace(img) colored = CImg<>(img);
#else
    #define displayIfDebug(img)  
    #define initTrace(img)
#endif    

void testMoveToTop() {
    CImg<>img (FOLDER "white.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1); 

    moveToTop(img, res, 50, 50, solution);
    displayIfDebug(colored);
    assert(res.found);
}


void testNoPathTopLeft(){
    CImg<>img (FOLDER "no_path_to_left.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1); 
    
    moveTopLeft(img, res, 50, 10, solution);
    displayIfDebug(colored);

    assert(areEquals(res, {false, 10, 10, -1, -1}));

    img = CImg<>(FOLDER "no_path_to_left_2.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1); 
    
    moveTopLeft(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 0, 10, -1, -1}));
    
}

void testMoveTopLeftAndTop(){
    CImg<>img (FOLDER "path_to_left_and_top.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1); 
    
    moveTopLeft(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(res.found);
    
    img = CImg<>(FOLDER "path_to_left_and_top_extrem.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1); 
    
    moveTopLeft(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(res.found);

}

void testMoveTopRightAndTop(){
    CImg<>img (FOLDER "path_to_top_and_right.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveTopRight(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testNoPathTopRight(){
    CImg<>img (FOLDER "no_path_to_right_left.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveTopRight(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 139, 10}));
    
    img =CImg<>(FOLDER "no_path_to_right_left_2.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1); 
    
    moveTopRight(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 149, 10}));
    
}

void testMoveTopTopAndRight(){
    CImg<>img (FOLDER "path_to_top_and_right.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToTop(img, res, 50, 20, solution);
    displayIfDebug(colored);
    assert(res.found); 
}


void testNoPathTopTopRightButToLeft(){
    CImg<>img = Utils::otsu_binarization(CImg<>(FOLDER "no_path_to_right_but_to_left.png"));
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToTop(img, res, 50, 20, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testMoveTopLeftUntil(){
    CImg<>img (FOLDER "stairs_left.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveTopLeftUntil(img, res, 50, 20, 1, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 4, 20, -1, -1}));
    
    img = CImg<>(FOLDER "stairs_left.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    
    moveTopLeftUntil(img, res, 50, 2, 2, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 3, 2, -1, -1}));
    
    //go after the column but blocked on the same line
    img = CImg<>(FOLDER "stairs_left_2.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    
    moveTopLeftUntil(img, res, 50, 30, 8, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 5, 30, -1, -1}));
    
    // go after the column and find the path
    img = CImg<>(FOLDER "path_stairs_left.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    
    moveTopLeftUntil(img, res, 50, 30, 17, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testMoveTopTopSided(){
    CImg<> img =CImg<>(FOLDER "path_to_left_with_object.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToTop(img, res, 100, 30, solution);
    displayIfDebug(colored);
    assert(res.found);

    
    img = Utils::otsu_binarization(CImg<>(FOLDER "no_path.png"));
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);    
    moveToTop(img, res, 120, 130, solution);
    displayIfDebug(colored);
    
    img = CImg<>(FOLDER "path_to_left_with_object_complex.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);    
    moveToTop(img, res, 120, 130, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testTopDoubleStairs(){
    CImg<> img = CImg<>(FOLDER "double_stairs.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToTop(img, res, 10, 145, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 10, 145, 89, 145}));
    
    
    img = CImg<>(FOLDER "double_stairs.png");
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);    
    moveToTop(img, res, 89, 145, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 10, 145, 89, 145}));
    
}


/**
 * Test for subroute bottom
 */
void testMoveBottom() {
    CImg<>img (FOLDER "white.png");
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToBottom(img, res, 50, 50, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testNoPathBottomRight(){
    CImg<>img (FOLDER "no_path_to_left.png");
    img.rotate(180.0);

    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveBottomRight(img, res, 99, 139, solution);
    displayIfDebug(colored);

    assert(areEquals(res, {false, -1, -1, 139, 139}));

    img = CImg<>(FOLDER "no_path_to_left_2.png");
    img.rotate(180.0);    
    initTrace(img);    
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomRight(img, res, 99, 139, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 149, 139}));
}

void testMoveBottomRightAndBottom(){
    CImg<>img (FOLDER "path_to_left_and_top.png");
    img.rotate(180.0);
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveBottomRight(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(res.found);
    
    img = CImg<>(FOLDER "path_to_left_and_top_extrem.png");
    img.rotate(180.0);
    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomRight(img, res, 50, 10, solution);
    displayIfDebug(colored);
    assert(res.found);

}

void testMoveBottomLeftAndBottom(){
    CImg<>img (FOLDER "path_to_top_and_right.png");
    img.rotate(180.0);    
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveBottomLeft(img, res, 99, 139, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testNoPathBottomLeft(){
    CImg<>img (FOLDER "no_path_to_right_left.png");
    img.rotate(180.0);
    
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveBottomLeft(img, res, 99, 139, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 10, 139, -1, -1}));
    
    img =CImg<>(FOLDER "no_path_to_right_left_2.png");
    img.rotate(180.0);
    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomLeft(img, res, 99, 139, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 0, 139, -1, -1}));
    
}

void testMoveBottomBottomAndLeft(){
    CImg<>img (FOLDER "path_to_top_and_right.png");
    img.rotate(180.0);
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveToBottom(img, res, 99, 129, solution);
    displayIfDebug(colored);
    assert(res.found); 
}


void testNoPathBottomBottomLeftButToRight(){
    CImg<>img = Utils::otsu_binarization(CImg<>(FOLDER "no_path_to_right_but_to_left.png"));
    img.rotate(180.0);
    initTrace(img);
    
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveToBottom(img, res, 99, 129, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testMoveBottomRightUntil(){
    CImg<>img (FOLDER "stairs_left.png");
    img.rotate(180.0);
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveBottomRightUntil(img, res, 99, 129, 148, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 145, 129}));
    
    img = CImg<>(FOLDER "stairs_left.png");
    img.rotate(180.0);    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomRightUntil(img, res, 99, 147, 147, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 146, 147}));
    
    //go after the column but blocked on the same line
    img = CImg<>(FOLDER "stairs_left_2.png");
    img.rotate(180.0);    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomRightUntil(img, res, 99, 119, 141, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, -1, -1, 144, 119}));
    
    // go after the column and find the path
    img = CImg<>(FOLDER "path_stairs_left.png");
    img.rotate(180.0);    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveBottomRightUntil(img, res, 99, 119, 132, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testMoveBottomBottomSided(){
    CImg<> img =CImg<>(FOLDER "path_to_left_with_object.png");
    img.rotate(180.0);    
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    moveToBottom(img, res, 49, 119, solution);
    displayIfDebug(colored);
    assert(res.found);

    
    img = Utils::otsu_binarization(CImg<>(FOLDER "no_path.png"));
    img.rotate(180.0);    
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveToBottom(img, res, 29, 19, solution);
    displayIfDebug(colored);
    
    img = CImg<>(FOLDER "path_to_left_with_object_complex.png");
    img.rotate(180.0);
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);
    moveToBottom(img, res, 29, 19, solution);
    displayIfDebug(colored);
    assert(res.found);
}

void testBottomDoubleStairs(){
    CImg<> img = CImg<>(FOLDER "double_stairs.png");
    img.rotate(180.0);
    initTrace(img);
    sol res = {false, -1, -1, -1, -1};                          
    vector<int> solution = vector<int>(img._height, -1);
    
    moveToBottom(img, res, 139, 4, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 60, 4, 139, 4}));
    
    
    img = CImg<>(FOLDER "double_stairs.png");
    img.rotate(180.0);
    initTrace(img);
    res = {false, -1, -1, -1, -1};                          
    solution = vector<int>(img._height, -1);    
    moveToBottom(img, res, 60, 4, solution);
    displayIfDebug(colored);
    assert(areEquals(res, {false, 60, 4, 139, 4}));
    
}



int main(int argc, char** argv) {
    cout << "Test overlapping segmentation" << endl;
    testMoveToTop();
    testNoPathTopLeft();
    testMoveTopLeftAndTop();
    testMoveTopRightAndTop();
    testNoPathTopRight();
    testMoveTopTopAndRight();
    testNoPathTopTopRightButToLeft();
    testMoveTopLeftUntil();
    testMoveTopTopSided();
    testTopDoubleStairs();
    
    testMoveBottom();
    testNoPathBottomRight();
    testMoveBottomRightAndBottom();
    testMoveBottomLeftAndBottom();
    testNoPathBottomLeft();
    testMoveBottomBottomAndLeft();
    testNoPathBottomBottomLeftButToRight();
    testMoveBottomRightUntil();
    testMoveBottomBottomSided();
    testBottomDoubleStairs();
    
    
    return (EXIT_SUCCESS);
}

