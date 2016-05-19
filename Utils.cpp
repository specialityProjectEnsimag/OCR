#include "Utils.h"

void gray_repartition(CImg<> img, int* gray, int length) {   
    memset(gray, 0, length*sizeof(int));    
    
    cimg_forXY(img, x, y){
        int pixel = img(x, y, 0, 0);
        gray[pixel]++;
    }
}

int threshold(CImg<> img) {
    int gray[256];
    gray_repartition(img, gray, 256);

    int total = img.width() * img.height();
    double sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += i * gray[i];
    }

    double sumB = 0;
    int wB = 0;
    int wF = 0;

    double max = 0;
    int lvl = 0;

    for (int i = 0; i < 256; i++) {
        wB += gray[i];
        if (wB == 0) continue;

        wF = total - wB;
        if (wF == 0) break;

        sumB += (double)(i * gray[i]);

        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;

        double between = (double)wB * (double)wF * (mB - mF) * (mB - mF);

        if (between >= max) {
            lvl = i;
            max = between;
        }
    }

    return lvl;
}

CImg<> Utils::otsu_binarization(CImg<> src) {
    CImg<> binarized(src);
    int lvl = threshold(src);
    
    cimg_forXY(src, x, y) {
        binarized(x, y, 0, 0) = src(x, y, 0, 0) > lvl ? 255 : 0;
    }
    
    return binarized;
}

CImg<> Utils::smooth(CImg<> src) {    
    CImg<> smoothed (src);
    int lvl = threshold(src);
    
    CImg<> Neight(3,3);
    cimg_for2x2(src,x,y,0,0,Neight,float) {
        smoothed (x, y, 0, 0) = Neight.sum()/(2*2);
    }
    
    return smoothed ;
}