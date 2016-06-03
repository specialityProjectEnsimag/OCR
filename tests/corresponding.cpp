/* 
 * File:   corresponding_int.cpp
 * Author: jeanselv
 *
 * Created on 26 mai 2016, 10:42:48
 */

#include <stdlib.h>
#include <iostream>
#include "boost/filesystem.hpp"
using namespace boost::filesystem;
#include "dataset.h"
#include "forecast.h"
using namespace std;

int main() {
    path path("dataset");
    int tab[NUMBER_LETTER];
    
    cout << "Begin" << endl;
    // Else the directory exists
    if(is_directory(path)) {
        directory_iterator it(path), end;
        memset(tab, 0, NUMBER_LETTER*sizeof(int));
        while(it != end) { 
            if(is_directory(it->status())) {
                string filename = it->filename();
                
                char c = corresponding_label(filename);
                int i = corresponding_int(c);
                char r = corresponding_char(i);
                cout << c << " - " << i << " - " << r << endl;
                tab[i] ++;
                if (!(0 <= i && i < NUMBER_LETTER)) {
                    cout << "Invalid number"<< endl;
                    return false;
                } else if (c != r) {
                    cout << "Invalid character"<< endl;
                    return false;
                }
                
            }
            it++;
        }
    }
    cout << "End" << endl;
    cout << "Verification" << endl;
    for (int i = 0; i < NUMBER_LETTER; i++) {
        if (tab[i] != 1) {
            cout << i << " - " << tab[i] << endl;
        } 
    }
    return true;
}

