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

    vector<int> verticalHistogram(const CImg<>& img){
        vector<int> hist(img._width, 0);
        cimg_forX(img, x){
            int count = 0;
            for(unsigned int y = 0; y <img._height; y++){
                if(img(x, y) != WHITE_PIXEL){
                    count++;
                }
            }
            hist.at(x) = count;
        }

        return hist;
    }

    vector<int> horizontalHistogram(const CImg<>& img){
        vector<int> hist(img._width, 0);
        cimg_forY(img, y){
            int count = 0;
            for(unsigned int x = 0; x <img._width; x++){
                if(img(x, y) != WHITE_PIXEL){
                    count++;
                }
            }
            hist.at(y) = count;
        }

        return hist;
    }

    vector<int> secondDifference(const vector<int>& vect){
        vector<int> res(vect.size(), 0);
        res.at(0) = vect.at(0);
        for(unsigned int i = 1; i < (vect.size() - 1); i++){
            res.at(i) = vect.at(i-1) - 2*vect.at(i) + vect.at(i + 1);
        }

        return res;
    }

    vector<int> ANDing(const CImg<>& img){
        CImg<> imgAND(img._width, img._height, 1, 1, 255);
        for(unsigned x = 0; x < (img._width - 1); x++){
            cimg_forY(img, y){
                if((img(x, y) == BLACK_PIXEL) && (img(x+1, y) == BLACK_PIXEL)){
                    imgAND(x, y) = BLACK_PIXEL;
                }

            }
        }

        return verticalHistogram(imgAND);
    }

};


