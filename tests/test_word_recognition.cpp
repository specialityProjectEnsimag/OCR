/* 
 * File:   testSegmentation.cpp
 * Author: clinm
 *
 * Created on 25 mai 2016, 08:56:15
 * 
 * This test file focuses on the segmentation algorithm itself.
 * Meaning that its purposes is to benchmark its accuraty.
 */

#include "base_test_segmentation.h"

#include "text_character.h"
#include "text_line.h"
#include "sliding_window.h"
#include "overlapping.h"
#include "space_detection.h"

void printHeader(){
    printColumns(30, ";");
    printColumns( 9, "Lines;");
    printColumns(16, "Error lines;");
    printColumns( 9, "Ratio;");
    printColumns(16, "Word expected;");
    printColumns(16, "Word found;");
    printColumns( 9, "Ratio;");
    printColumns(26, "Lines with - words;");
    printColumns(26, "Lines with + words;"<< endl);
}

/**
 * @param fileName              Path to the image
 * @param wordsPerLine     Expected number of characters per line
 */
void testSplitStat(const char* fileName, vector<int> wordsPerLine){
    CImg<> img = image_io::import(fileName);
    display(img);
    CImg<> crop = projection::reduce((img));
    crop = preprocessing::otsu_binarization(crop);
    int nLine = 0;
    int nLineIncorrect = 0;
    int nWords = 0;
    int nWordsFound = 0;
    int nLineNotEnough = 0;
    int nLineTooMuch = 0;
            
    vector< text_line* > split;
    sliding_window::splitLines(crop, split);    
    double res = space_detection::getSpaceThreshold(split);
    // cout << "Res " << res << endl;
    if(split.size() != wordsPerLine.size()){
        cout << "Number of lines incorrect !" << endl;
        cout << "split " << split.size() << " expected " << wordsPerLine.size() << endl;
    }

    nLine = std::min(split.size(), wordsPerLine.size());
    
    for(int i = 0; i < nLine; i++){
       nWords += wordsPerLine.at(i);
       vector< text_character* > lines;
       
       CImg<> line = projection::reduce(split.at(i)->img);
       
       sliding_window::splitCharacters(line, lines, res);

       int nWordsOnLine = 0;
       for(unsigned int j = 0; j < lines.size(); j++){
       		if(lines[j]->endOfWords){
       			nWordsOnLine ++;
       			// cout << "End of words !" << endl;
       		}
       		// image_io::displayImage(lines[j]->img);
       }
       nWordsFound += nWordsOnLine;

       
       if(nWordsOnLine != wordsPerLine.at(i)){
           nLineIncorrect++;
           if(nWordsOnLine > wordsPerLine.at(i)){
               nLineTooMuch++;
           }else{
               nLineNotEnough++;
           }

           debug(DEBUG_LOT, "Error on line " << i << endl);
           debug(DEBUG_LOT, "Split " << lines.size());
           debug(DEBUG_LOT, " expected " << wordsPerLine.at(i) << endl);
           
           display(line);
           for(unsigned int j = 0; j < lines.size(); j++){
               display(lines.at(j)->img);
           }
       }
       

       text_character::freeVector(lines);
    }

    text_line::freeVector(split);

    printColumns( 8, nLine << ";");
    printColumns(15, nLineIncorrect << ";");
    printColumns( 7, (float)((nLine - nLineIncorrect)*100/(float)nLine) << "%;");
    printColumns(15, nWords << ";");
    printColumns(15, nWordsFound << ";");
    printColumns( 7, (float)(nWordsFound*100/(float)nWords) << "%;");
    printColumns(25, nLineNotEnough << ";");
    printColumns(25, nLineTooMuch << ";" << endl);
}

void testAll(const char* baseFile, std::vector<int> wordsPerLine, string fontsName[], int size){
    printHeader();
    for(int i = 0; i < size; i++){
        std::stringstream output;
        printColumns(29, fontsName[i] << ";");
        output << FOLDER << baseFile << fontsName[i] <<".png" ;
        testSplitStat(output.str().c_str(), wordsPerLine);
    }
}

int main() {
    cout << "Test segmentation overlapping" << endl;
    cout << "DEFINE " << DEBUG_LEVEL << endl;
    // without dot
    int text1[] = {7, 9, 10, 8, 7, 9, 9, 9, 7};
    // with dot
    // int text1[] = {8, 10, 11, 9, 8, 10, 10, 10, 8};

    std::vector<int> wordsPerLine(text1, text1 + sizeof(text1) / sizeof(int) );

    cout << "## Normal" << endl;    
    testAll("text-1-", wordsPerLine, fontsNonCursive, NBFONTS_NON_CURSIVE);
    cout << "## Cursive" << endl;
    testAll("text-1-", wordsPerLine, fontsCursive, NBFONTS_CURSIVE);
   
    return (EXIT_SUCCESS);
}