#include "projection.h"
#include "chamfer.h"

namespace projection {
    
    vector<int> leftward(CImg<> img){
        vector<int> sum(img._height, 0);
        
        for(unsigned int h = 0; h < img._height; h++){
            unsigned int j = 0;
            while(j < img._width && img(j,h) == 255){
                sum.at(h) = sum.at(h) + 1 ;
                j++;
            }
        }
        return sum;
    }
    
    CImg<> leftwardToImg(vector<int> left, int width){
        CImg<> img(width, left.size(), 1, 1, 255);
        for(unsigned int h = 0; h < left.size(); h++){
            for(int w = 0; w < left.at(h); w++){
                img(w, h) = 0;
            }
        }
        return img;
    }
    
    vector<int> upward(CImg<> img){
        vector<int> sum(img._width, 0);
        
        for(unsigned int w = 0; w < img._width; w++){
            unsigned int j = 0;
            while(j < img._height && img(w, j) == 255){
                sum.at(w) = sum.at(w) + 1 ;
                j++;
            }
        }
        return sum;
    }
  
    CImg<> upwardToImg(vector<int> top, int height){
        CImg<> img(top.size(), height, 1, 1, 255);
        for(unsigned int w = 0; w < top.size(); w++){
            for(int h = 0; h < top.at(w); h++){
                img(w, h) = 0;
            }
        }
        return img;
    }
 
    int firstNonEqual(vector<int> projection, int value, bool beginning){
        int index;
        if(beginning){
            index = -1;
            do{
                index++;
            }while(index < (int) projection.size() && projection.at(index) == value);
        }else{
            index = projection.size();
            do{
                index--;
            }while(index >= 0 && projection.at(index) == value);

        }
        
        return index;
    }
    
    CImg<> reduce(CImg<> img){
        vector<int> up = upward(img);
        int upB = firstNonEqual(up, img._height);
        int upE = firstNonEqual(up, img._height, false);

        vector<int> left = leftward(img);
        int leftB = firstNonEqual(left, img._width);
        int leftE = firstNonEqual(left, img._width, false);

        if(upB == (int) img._width){
            return CImg<>(1, 1, 1, 1, 255);
        }else{
            return img.get_crop(upB, leftB, upE, leftE);            
        }
    }
    
    void splitLines(const CImg<>& text, vector< CImg<>* >& lines){
        vector<int> sep = leftward(text);
        
        int state = 1; // 0: blank, 1: something
        int beg = 0;
        
        int full = text._width;
        sep.push_back(full);
        for(unsigned int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;
                    CImg<>* elt = new CImg<>(text.get_crop(0, beg, full-1, i-1));
                    lines.push_back(elt);
                }
            }
        }
    }
    
#ifdef SLIDING_WINDOW
    
    void splitCharacters(const CImg<>& line, vector< CImg<>* >& characters){
        vector<int> sep = upward(line);

        int state = 1; // 0: blank, 1: something
        int beg = 0;

        int full = line._height;
        sep.push_back(full);
        for(int i = 0; i < sep.size(); i++){
            if(state == 0){
                if(sep.at(i) != full){
                    beg = i;
                    state = 1;
                }
            }else if(state == 1){
                if(sep.at(i) == full){
                    state = 0;

                    CImg<>* elt = new CImg<>(line.get_crop(beg, 0, i-1, full-1));
                    //.resize(128, 128, 1, 1))
                    characters.push_back(elt);
                }
            }
        }
    }
#else
    void splitCharacters(const CImg<>& line, vector< CImg<>* >& characters){
        CImg<> img(line);
        CImg<> left, right;
        
        do{
            // adding white column to make sure the splitting always work
            img = img.get_resize(img._width + 1, img._height, -100, -100, 0);
            cimg_forY(img, y){
                img(img._width - 1, y) = 255;
            }

            overlappingSegmentation::splitChar(img, left, right);
            
            CImg<>* res = new CImg<>(left);
            img = CImg<>(right);
            characters.push_back(res);
        }while(right.min() != WHITE_PIXEL);
    }
    
    
#endif
};


