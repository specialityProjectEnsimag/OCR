/* 
 * File:   main.cpp
 * Author: clinm
 *
 * Created on 17 mai 2016, 10:32
 */
#define cimg_verbosity 3
#include <cstdlib>

#include <iostream>
#include <vector>
#include "image_io.h"
#include "preprocessing.h"
#include "clustering_data.h"
#include "distance.h"
#include "CImg.h"
#include "space_detection.h"
#include "projection.h"

using namespace cimg_library;
using namespace std;
#include "segmentation.h"
using namespace segmentation;

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "boost/filesystem.hpp"
using namespace boost::filesystem;


#include "hmm.h"


#define result_dataset "./result_dataset"
#define dataset "./dataset"
#define hmm_data "./dictionnary.txt"
#define hmm_save "./hmm_save"

int main(int argc, char** argv) {
    // in order to remove unwanted error in output
    //cimg::exception_mode(0);
    
    cout << "Welcome in our beautiful OCR!" << endl;
    
    if(argc < 2){
        cout << "Not enough parameters" << endl;
        cout << "Usage ./"__FILE__ << " file..." << endl;
        cout << "        file...\t\tTexts" << endl;
        exit(1);
    }

    cout << "Loading OCR ..." << endl;
    path forecastPath(result_dataset); 
    // If the directory does not exist
    if(!exists(forecastPath)) { 
        cout << result_dataset" directory has not been found. Computing it right now ... (Takes time)" << endl;
        kmeans(dataset, result_dataset);
        cout << "End of computation" << endl;
        cout << "Loading OCR ..." << endl;
    }

    Forecast forecast(result_dataset);
    cout << "OCR loaded !" << endl;

    HMM hmm;
    cout << "Loading HMM" << endl;
    path hmmPath(hmm_save);
    if(!exists(hmmPath)) { 
        cout << result_dataset" File has not been found. Computing it right now ... (Takes time)" << endl;
        
        hmm = HMM(dataset, hmm_data, forecast);

        {
            cout << "Save" << endl;
            std::ofstream ofs(hmm_save);
            boost::archive::text_oarchive oa(ofs);
            oa << hmm;
        }

        cout << "End of computation" << endl;
    }else{
        hmm = HMM();
        {
            std::ifstream ifs(hmm_save);
            boost::archive::text_iarchive ia(ifs);
            ia >> hmm;
        }
    }
    cout << "HMM loaded !" << endl;

    vector <forecast_type> words;
    string hmmWords;

    for(int i = 1; i < argc; i++){
        cout << endl << endl;
        cout << "Working on : " << argv[i] << endl;

        CImg<> document;
        try{
            document = image_io::import(argv[i]);
        }catch(CImgIOException e){
            cerr << "Error on file " << argv[i] << endl;
            exit(1);
        }

        // Display the unmodified document
        image_io::displayImage(document);
        // Pre preocessing of the image
        float tab[9] = { 1, 2, 1, 2, 3, 2, 1, 2, 1 };
        CImg<> mask(tab,3,3);
        document = preprocessing::linear_filter(document, mask);
        document = preprocessing::otsu_binarization(document);
        document = preprocessing::linear_filter(document, mask);
        document = preprocessing::otsu_binarization(document);
        document = preprocessing::hough_rotation(document,80,110,600);
        document = preprocessing::otsu_binarization(document);
        image_io::displayImage(document);
        
        vector< text_line* > lines;
        splitLines(document, lines);
        for(unsigned int li = 0; li < lines.size(); li++){
            // Delete little lines
            if (lines[li]->img._height < 9) {
                continue;
            } 
            
            vector< text_line* > l;
            l.push_back(lines[li]);
            double wordsThreshold = space_detection::getSpaceThreshold(l);
            text_line line = *lines[li];
            line.img = projection::reduce(line.img);
            vector< text_character* > characters;
            splitCharacters(line.img, characters, wordsThreshold);
            for(unsigned int ch = 0; ch < characters.size(); ch++){
                if (characters[ch]->img._height < 5 || characters[ch]->img._width < 5) {
                    continue;
                } 
                
                text_character character = *characters[ch];
                character = projection::reduce(character, line.up_barrier, line.low_barrier);
                preprocessing::preprocessing(character.img);
                character.img.resize(SQUARE, SQUARE, -100, -100, 3);
                std::vector<forecast_type>  res;
                forecast.forecast(character, res, MSE);
                std::vector<forecast_type>::iterator i = res.begin();
                // when end of words, use HMM to fix and print result
                if(character.endOfWords){
                    words.push_back(*i);
                    
                    hmmWords = hmm.viterbi(hmm_data, words);
                    cout << hmmWords;
                    words.clear();
                    hmmWords.clear();
                    cout << " ";
                }else{
                    words.push_back(*i);
                }


            }
            
            // end of line in the document
            cout << endl;

            cimg_forXY(lines[li]->img, x, y){
                if (y == lines[li]->up_barrier) {
                    (lines[li]->img)(x,y) = 150;
                } else if ( y == lines[li]->low_barrier) {
                    (lines[li]->img)(x,y) = 200;
                }
            }
            image_io::displayImage(lines[li]->img);
            
            text_character::freeVector(characters);
        }

        text_line::freeVector(lines);
    }
}