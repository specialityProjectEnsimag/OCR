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

/**
 *  Fonts' name. When adding a font, you have to make sure to provide one image per serie
 */
string fonts[] = {"arial", "calibri", "comicSansMS", "corsiva", "droidSerif", "roboto", "ubuntu"};
#define NBFONTS 7

void printHeader(){
    printColumns(15, ";");
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
void testSplitStat(const char* fileName, vector<int> charactersPerLine, bool preprocessing){
    CImg<> img = image_io::import(fileName);
    display(img);
    CImg<> crop = projection::reduce((img));
    
    int nLine = 0;
    int nLineIncorrect = 0;
    int nCharacter = 0;
    int nCharacterFound = 0;
    int nLineNotEnough = 0;
    int nLineTooMuch = 0;
            
    vector< CImg<>* > split;
    projection::splitLines(crop, split);    

    if(split.size() != charactersPerLine.size()){
        cout << "Number of lines incorrect !" << endl;
        cout << "split " << split.size() << " expected " << charactersPerLine.size() << endl;
    }

    nLine = std::min(split.size(), charactersPerLine.size());
    
    for(int i = 0; i < nLine; i++){
       nCharacter += charactersPerLine.at(i);
       vector< CImg<>* > lines;
       
       CImg<> line = projection::reduce(*split.at(i));
       
       projection::splitCharacters(line, lines);
       nCharacterFound += lines.size();
       
       if(lines.size() != charactersPerLine.at(i)){
           nLineIncorrect++;
           if(lines.size() > charactersPerLine.at(i)){
               nLineTooMuch++;
           }else{
               nLineNotEnough++;
           }

           debug(DEBUG_LOT, "Error on line " << i << endl);
           debug(DEBUG_LOT, "Split " << lines.size());
           debug(DEBUG_LOT, " expected " << charactersPerLine.at(i) << endl);
           
           display(line);
           for(int j = 0; j < lines.size(); j++){
               display(*lines.at(j));
           }
       }
       
       
       image_io::delete_images(lines);       
    }

    image_io::delete_images(split);

    printColumns( 8, nLine << ";");
    printColumns(15, nLineIncorrect << ";");
    printColumns( 7, (nLine - nLineIncorrect)*100/nLine << "%;");
    printColumns(15, nCharacter << ";");
    printColumns(15, nCharacterFound << ";");
    printColumns( 7, nCharacterFound*100/nCharacter << "%;");
    printColumns(25, nLineNotEnough << ";");
    printColumns(25, nLineTooMuch << ";" << endl);
}

void testAll(const char* baseFile, const char* title, bool preprocessing, std::vector<int> charPerLine){
    cout << title << endl;
    printHeader();
    for(int i = 0; i < NBFONTS; i++){
        std::stringstream output;
        printColumns(14, fonts[i] << ";");
        output << FOLDER << baseFile << fonts[i] <<".png" ;
        testSplitStat(output.str().c_str(), charPerLine, preprocessing);
    }
}

int main(int argc, char** argv) {
    cout << "Test segmentation overlapping" << endl;
    
    int text1[] = {36, 46, 57, 55, 43, 34, 52, 43, 41};
    std::vector<int> charPerLine(text1, text1 + sizeof(text1) / sizeof(int) );
    
    testAll("text-1-", "Sans traitement: ", false, charPerLine);
    cout << endl << endl;
    testAll("text-1-", "Avec traitement: ", true, charPerLine);
    
   
    return (EXIT_SUCCESS);
}