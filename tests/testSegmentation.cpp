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

void printHeader(){
    printColumns(25, ";");
    printColumns( 9, "Lines;");
    printColumns(16, "Error lines;");
    printColumns( 9, "Ratio;");
    printColumns(16, "Char expected;");
    printColumns(16, "Char found;");
    printColumns( 9, "Ratio;");
    printColumns(26, "Lines with - character;");
    printColumns(26, "Lines with + character;"<< endl); 
}

/**
 * @param fileName              Path to the image
 * @param charactersPerLine     Expected number of characters per line
 * @param preprocessing         If set to TRUE, a preprocessing is applied on the imae
 */
void testSplitStat(const char* fileName, vector<int> charactersPerLine,
                          void (*sLines)(const CImg<>& text, vector< text_line* >& lines),
                          void (*sCharacters)(const CImg<>& line, vector< text_character* >& characters, double)){
    CImg<> img = image_io::import(fileName);
    display(img);
    CImg<> crop = projection::reduce((img));
    crop = preprocessing::otsu_binarization(crop);
    int nLine = 0;
    int nLineIncorrect = 0;
    int nCharacter = 0;
    int nCharacterFound = 0;
    int nLineNotEnough = 0;
    int nLineTooMuch = 0;
            
    vector< text_line* > split;
    sLines(crop, split);    

    if(split.size() != charactersPerLine.size()){
        cout << "Number of lines incorrect !" << endl;
        cout << "split " << split.size() << " expected " << charactersPerLine.size() << endl;
    }

    nLine = std::min(split.size(), charactersPerLine.size());
    
    for(int i = 0; i < nLine; i++){
       nCharacter += charactersPerLine.at(i);
       vector< text_character* > lines;
       
       CImg<> line = projection::reduce(split.at(i)->img);
       sCharacters(line, lines, -1);
       nCharacterFound += lines.size();
       
       if((int)lines.size() != charactersPerLine.at(i)){
           nLineIncorrect++;
           if((int)lines.size() > charactersPerLine.at(i)){
               nLineTooMuch++;
           }else{
               nLineNotEnough++;
           }

           debug(DEBUG_LOT, "Error on line " << i << endl);
           debug(DEBUG_LOT, "Split " << lines.size());
           debug(DEBUG_LOT, " expected " << charactersPerLine.at(i) << endl);
           
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
    printColumns(15, nCharacter << ";");
    printColumns(15, nCharacterFound << ";");
    printColumns( 7, (float)(nCharacterFound*100/(float)nCharacter) << "%;");
    printColumns(25, nLineNotEnough << ";");
    printColumns(25, nLineTooMuch << ";" << endl);
}

void testAll(const char* baseFile, const char* title, std::vector<int> charPerLine,
                      void (*sLines)(const CImg<>& text, vector< text_line* >& lines),
                      void (*sCharacters)(const CImg<>& line, vector< text_character* >& characters, double)){
    cout << title << endl;
    printHeader();
    for(int i = 0; i < NBFONTS; i++){
        std::stringstream output;
        printColumns(24, fonts[i] << ";");
        output << FOLDER << baseFile << fonts[i] <<".png" ;
        testSplitStat(output.str().c_str(), charPerLine, sLines, sCharacters);
    }
}

int main() {
    cout << "Test segmentation overlapping" << endl;
    cout << "DEFINE " << DEBUG_LEVEL << endl;
    int text1[] = {36, 46, 57, 55, 43, 34, 52, 43, 42};
    std::vector<int> charPerLine(text1, text1 + sizeof(text1) / sizeof(int) );
    
    testAll("text-1-", "Sliding window", charPerLine, sliding_window::splitLines, sliding_window::splitCharacters);
    testAll("text-1-", "Overlapping segmentation", charPerLine, sliding_window::splitLines, overlapping::splitCharacters);
    
       
   
    return (EXIT_SUCCESS);
}