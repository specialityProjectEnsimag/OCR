#include "dataset.h"

char corresponding_label(string dir) {
    // This function is parametrated for the dataset
    // First we observe the special character
    if (dir.length() == 1) {
        return dir.c_str()[0];
    }
    // Second the other
    int value = atoi(dir.substr(6,3).c_str());
    if (value < 11) {
        return value - 1 + '0';
    } else if (value < 37) {
        return value - 11 + 'A';
    } else if (value < 63){
        return value - 37 + 'a';
    } else if (value == 63) {
        return '/';
    } else if (value == 64) {
        return '.';
    }
    throw invalid_argument("No corresponding character !");
}

int corresponding_int(char c) {
    int last = 0;
    if ('A' <= c && c <= ']') {
        return c - 'A';
    } else {
        last = ']' - 'A' + 1;
        if ('a' <= c && c <= 'z'){
            return c - 'a' + last;
        } else {
            last += 'z' - 'a' + 1;
            if ('(' <= c && c <= '?' ) {
                return c - '(' + last;
            } else {
                last += '?' - '(' + 1;
                if ('!' <= c && c <= '#') {
                    return c - '!' + last;
                } else {
                    last += '#' - '!' + 1;
                    if (c =='&') {
                        return last; 
                    } else if (c=='{') {
                        return last + 1; 
                    } else if (c=='}') {
                        return last + 2;
                    } else if (c=='~') {
                        return last + 3;
                    }
                }
            }
        }
    }
    throw invalid_argument("No corresponding integer !");
}

char corresponding_char(int i) {
    int last = 0;
    if (i < 0) {
        return '\n';
    } else if (i - last <= ']' - 'A') {
        return 'A' + i - last;
    } else {
        last += ']' - 'A' + 1;
        if (i - last <= 'z' - 'a'){
            return 'a' + i - last ;
        } else {
            last += 'z' - 'a' + 1;
            if (i - last  <= '?' - '(') {
                return '(' + i - last;
            } else {
                last += '?' - '(' + 1;
                if (i - last <= '#' - '!') {
                    return '!' + i - last;
                } else {
                    last += '#' - '!' + 1;
                    if (i == last) {
                        return '&'; 
                    } else if (i == last + 1) {
                        return '{'; 
                    } else if (i == last + 2) {
                        return '}';
                    } else if (i == last + 3) {
                        return '~';
                    }
                }
            }
        }
    }
    throw invalid_argument("No corresponding character !");
}