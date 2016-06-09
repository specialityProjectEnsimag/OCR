#include "space_detection.h"

namespace space_detection{

    /**
     * collects given an axis (either horizontal or vertical). 
     * @param spaceSize Contains the summary of all space size where i represents the width
     *                  of the space and the value at spaceSize[i] is the number of times this width
     *                  as space was encountered
     * @param upward    if set to true, look for vertical spaces, otherwise look for horizontal space
     */
    void collectSpaces(const CImg<>& img, vector<int>& spaceSize, bool upward = true){
        vector<int> sep;
        CImg<> reducedImg(projection::reduce(img));
        if(upward){
            sep = projection::upward(reducedImg);
        }else{            
            sep = projection::leftward(reducedImg);
        }

        int state = 1; // 0: blank, 1: something
        unsigned int whiteSpaceSize = 0;
        int full;
        if(upward){
            full = reducedImg._height;
        }else{            
            full = reducedImg._width;
        }


        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    if(whiteSpaceSize >= spaceSize.size()){
                        spaceSize.resize(whiteSpaceSize + 1, 0);
                    }
                    spaceSize[whiteSpaceSize] += 1; 

                    state = 1;
                }else{
                    whiteSpaceSize++;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    whiteSpaceSize = 0;
                }
            }
        }
    }

    /**
     *  Sums up value in order to compute the mean 
     * @param values results of collectSpaces
     * @param value Sum of all 'i' where values[i] != 0
     * @param total Number of values[i] != 0
     */
    void inline fold(const vector<int>& values, double& value, double& total){
        value = 0;
        total = 0;
        for(unsigned int i = 0; i < values.size(); i++){
            if(values[i] != 0){
                total += 1;
                value += i;
            }
        }
    }

    double getSpaceThreshold(const vector< text_line* >& lines){
        vector<int> spaceSize(10, 0);


        for(unsigned int li = 0; li < lines.size(); li++){
            collectSpaces(lines[li]->img, spaceSize);
        }

        double value = 0;
        double total = 0;
        fold(spaceSize, value, total);

        return std::floor(0.95*value / total);
    }

    double getParagraphThreshold(const CImg<>& document){
        vector<int> spaceSize(10, 0);

        collectSpaces(document, spaceSize, false);

        double value = 0;
        double total = 0;
        fold(spaceSize, value, total);

        return std::floor(value / total);
    }

}