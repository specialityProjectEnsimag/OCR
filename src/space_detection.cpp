#include "space_detection.h"

namespace space_detection{

    double getSpaceThreshold(const vector< text_line* >& lines){
        vector<int> spaceSize(10, 0);


        for(unsigned int li = 0; li < lines.size(); li++){
            vector<int> sep = projection::upward(projection::reduce(lines[li]->img));

            int state = 1; // 0: blank, 1: something
            unsigned int whiteSpaceSize = 0;
            int full = lines[li]->img._height;
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

        double value = 0;
        double total = 0;
        for(unsigned int i = 0; i < spaceSize.size(); i++){
            if(spaceSize[i] != 0){
                total += 1;
                value += i;
            }
        }

        return std::floor(value / total);
    }
}