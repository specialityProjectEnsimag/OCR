/* 
 * File:   fontTest.h
 * Author: clinm
 *
 * Created on 19 mai 2016, 09:17:50
 * Statistic on the reconition of simple characters from various fonts
 */

#ifndef FONTTEST_H
#define	FONTTEST_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image_io.h"
#include "clustering_data.h"
#include "CImg.h"
#include "distance.h"
#include "projection.h"
#include "forecast.h"

#include "boost/filesystem.hpp"
#include "CImg.h"
#include <iomanip>

using namespace boost::filesystem;
using namespace cimg_library;
using namespace std;

#define FONTS_DIR "./Images_Tests/fonts/"
#define NB_CHARACTER 62
#define NB_CHARACTER_PER_LINE 10
int totalCharacter;
int errorCharacter;
int errorUpperLowerCharacter;

vector<int> errorByCharacter;
vector<int> errorUpperLowerByCharacter;
string check("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    
void init(){
    totalCharacter = 0;
    errorCharacter = 0;
    errorUpperLowerCharacter = 0;
    errorByCharacter = vector<int>(NB_CHARACTER, 0);
    errorUpperLowerByCharacter = vector<int>(NB_CHARACTER, 0);
}

void displayStats(){
    cout << "Statistics: " << endl;
    cout << "\t Total: "   << totalCharacter << endl;
    cout << "\t Error: "   << errorCharacter << endl;
    cout << "\t Ratio: "   << (errorCharacter*100)/totalCharacter << "%" << endl;
    cout << "\t Error Upper lower case: "<< errorUpperLowerCharacter << endl;
    cout << "\t Ratio                 : "<< (errorUpperLowerCharacter*100)/totalCharacter << "%" << endl;
    cout << "Notation: Letter (ErrorUpperLower / ErrorTotalCharacter) " << endl;
    int l = 0;
    for(int i = 0; i < NB_CHARACTER; i++){
        cout << "\t" << check.at(i);
        
        if(errorByCharacter.at(i) == 0){
            cout << "( 0/ 0)";
        }else{
            cout << "(";
            if(errorUpperLowerByCharacter.at(i) == errorByCharacter.at(i)){
                cout << "\033[1;31m"<< setfill(' ') << setw(2) << errorUpperLowerByCharacter.at(i) <<"\033[0m";
            }else{
                cout << "\033[1;34m"<< setfill(' ') << setw(2) << errorUpperLowerByCharacter.at(i) <<"\033[0m";
            }
            cout << "/" ;
            cout << "\033[1;31m"<< setfill(' ') << setw(2) << errorByCharacter.at(i) <<"\033[0m";        
            cout << ")";            
        }

        
        l++;
        if(l == NB_CHARACTER_PER_LINE){
            l = 0;
            cout << endl;
        }
    }
    cout << endl;    
}

void checkString (string check, string expected){
    if(check.size() != expected.size()){
        cout << "Size are not the same ! Proceding anyway" << endl;
    }
    
    int min = std::min(check.size(), expected.size());
    cout << "Check   : " << check << endl;
    cout << "Expected: " << expected << endl;
    cout << "Diff   :  " ;
    for(int i = 0; i < min; i++){
        totalCharacter++;
        if(check.at(i) == expected.at(i)){
            cout << ".";
        }else{
            cout << "F";
            errorCharacter++;
            errorByCharacter.at(i) = errorByCharacter.at(i) + 1;
            
            if(check.at(i) == expected.at( (i + 26) % 52)){
                errorUpperLowerCharacter ++;
                errorUpperLowerByCharacter.at(i) = errorUpperLowerByCharacter.at(i) + 1;
            }
        }
    }
    
    cout << endl;
    
}

string analyse(const char* path, Forecast* f);

/**
 * Goes trough all fonts availables in the fonts directory
 * @param f
 */
void checkfont(Forecast* f){
    init();
    path path(FONTS_DIR); 
    // If the directory does not exist
    if(!exists(path)) { 
        cout << "Directory not found " << endl;
    } 

    directory_iterator end_itr;
    for(directory_iterator it(path); it != end_itr; ++it) { 
        cout << endl << "File    : " << it->path().filename() << endl;
        std::stringstream filePath;

        string res = analyse(it->path().string().c_str(), f);
        checkString(res, check);   
        
    }

    displayStats();
}



#endif	/* FONTTEST_H */

