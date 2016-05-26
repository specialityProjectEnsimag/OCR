/* 
 * File:   testOverlappingSegmentation.cpp
 * Author: clinm
 *
 * Created on 25 mai 2016, 15:49:59
 * 
 * This test file focuses on the overlapping segmentation.
 * The images were cleaned in order to have a perfect characters.
 * This way we can validate the correctness of the algorithm.
 * Specific tests were made in test_for_overlappingsegmentation
 * but this test is intended to validate the algorithm on a larger scale
 * (by providing hopefully a scenario where the algorithm goes wrong
 * (segfault ..)
 */

#include "base_test_segmentation.h"

#include "overlapping.h"
using namespace overlapping;

/**
 * @param fileName              Path to the image
 * @param charactersPerLine     Expected number of characters per line
 * @param preprocessing         If set to TRUE, a preprocessing is applied on the imae
 */
void testSplitAlgorithm(const char* fileName, vector<int> charactersPerLine, bool preprocessing){
    debug(DEBUG_FEW, "Test on " << fileName << endl);
    CImg<> img = image_io::import(fileName);
    display(img);
    CImg<> crop = projection::reduce(preprocessing::otsu_binarization(img));
    
    if(preprocessing){
        float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
        CImg<> mask(tab,3,3);
        crop = preprocessing::linear_filter(crop, mask);
        crop = preprocessing::otsu_binarization(crop);        
    }

    vector< text_line* > split;
    splitLines(crop, split);    

    if(split.size() != charactersPerLine.size()){
        cout << "Number of lines incorrect !" << endl;
        cout << "split " << split.size() << " expected " << charactersPerLine.size() << endl;
    }
    int end = std::min(split.size(), charactersPerLine.size());
    
    bool error = false;
    
    for(int i = 0; i < end; i++){
       vector< text_character* > lines;
       CImg<> line = projection::reduce(split.at(i)->img);
       
       splitCharacters(line, lines);
       if((int)lines.size() != charactersPerLine.at(i)){
           error = true;
           debug(DEBUG_LOT, "Error on line " << i << endl);
           debug(DEBUG_LOT, "Split " << lines.size());
           debug(DEBUG_LOT, " expected " << charactersPerLine.at(i) << endl);
           
           for(unsigned int j = 0; j < lines.size(); j++){
               display(lines.at(j)->img);
           }
       }
       
       
       text_character::freeVector(lines);       
    }

    text_line::freeVector(split);
    
    if(error){
        cout << "Test incorrect on " << fileName << endl;
    }

}

int main() {
    cout << "Test segmentation" << endl;
    // the array contains the number of expected characters per line
    // including all ponctuation marks
    
    int sol[] = {39};
    std::vector<int> charPerLine (sol, sol + sizeof(sol) / sizeof(int) );
    testSplitAlgorithm(FOLDER"simple_line.png", charPerLine, true);
    
    int sol2[] = {8, 41, 47, 48, 43, 45, 47, 44};
    charPerLine =std::vector<int> (sol2, sol2 + sizeof(sol2) / sizeof(int) );
    testSplitAlgorithm(FOLDER"paragraph-1.png", charPerLine, true);
    
    int sol3[] = {14, 41, 43, 46, 46, 34, 39, 43, 44, 44, 49, 14};
    charPerLine =std::vector<int> (sol3, sol3 + sizeof(sol3) / sizeof(int) );
    testSplitAlgorithm(FOLDER"paragraph-2.png", charPerLine, true);
    
    
    return (EXIT_SUCCESS);
}

