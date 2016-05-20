#include "Utils.h"

/**
 * Computes the repartition of gray
 * @param img       The image to study
 * @param gray      An array for different shades
 * @param length    The length of the array
 */
void gray_repartition(CImg<> img, int* gray, int length) {   
    memset(gray, 0, length*sizeof(int));    
    
    cimg_forXY(img, x, y){
        int pixel = img(x, y, 0, 0);
        gray[pixel]++;
    }
}

/**
 * Computes the threshold using the algorithm of otsu
 * @param img   The origine image
 * @return      The threshold
 */
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
    
    CImg<> Neight(3,3);
    cimg_for3x3(src,x,y,0,0,Neight,float) {
        smoothed (x, y, 0, 0) = Neight.sum()/(3*3);
    }
    
    return smoothed ;
}

/**
 * Computes the number of white, black patterns (transitions from 255 to 0)
 * in the neighborhood : P2, P3, P4, P5, P6, P7, P8, P9, P2
 * P9 P2 P3
 * P8 P1 P4
 * P7 P6 P5
 * @param src   A 3x3 image
 * @return      The number of pattern
 */
int getA(CImg<> src) {
    int res = 0;
    // P2 == 255 and P3 == 0
    if (src(1,0,0,0) == 255 && src(2,0,0,0) == 0) res++;
    // P3 == 255 and P4 == 0
    if (src(2,0,0,0) == 255 && src(2,1,0,0) == 0) res++;
    // P4 == 255 and P5 == 0
    if (src(2,1,0,0) == 255 && src(2,2,0,0) == 0) res++;
    // P5 == 255 and P6 == 0
    if (src(2,2,0,0) == 255 && src(1,2,0,0) == 0) res++;
    // P6 == 255 and P7 == 0
    if (src(1,2,0,0) == 255 && src(0,2,0,0) == 0) res++;
    // P7 == 255 and P8 == 0
    if (src(0,2,0,0) == 255 && src(0,1,0,0) == 0) res++;
    // P8 == 255 and P9 == 0
    if (src(0,1,0,0) == 255 && src(0,0,0,0) == 0) res++;
    // P9 == 255 and P2 == 0
    if (src(0,0,0,0) == 255 && src(1,0,0,0) == 0) res++;
    return res;
}

/**
 * Computes the number of black pixel
 * in the neighborhood : P2, P3, P4, P5, P6, P7, P8, P9, P2
 * P9 P2 P3
 * P8 P1 P4
 * P7 P6 P5
 * @param src   A 3x3 image
 * @return      The number of black pixel
 */
int getB(CImg<> src) {
    int res = 0;
    cimg_forXY(src, x, y) {
        // Not compute for the central pixel
        if (!(x == 1 && y == 1)) {
            res += src(x, y, 0, 0) == 0 ? 1 : 0;
        }
    }
    return res;
}

CImg<> Utils::skeletonization(CImg<> src) {
    CImg<> res (src);
    CImg<> Neight(5,5);
    bool change = true;
    
    while (change) {
        change = false;
        CImg<> skeleton (res);
        cimg_for5x5(skeleton,x,y,0,0,Neight,float) {
            // Black is equal to one for the Hilditch’s Thinning Algorithm
            int P2 = Neight(2,1,0,0) == 0 ? 1 : 0;
            int P4 = Neight(3,2,0,0) == 0 ? 1 : 0;
            int P6 = Neight(2,3,0,0) == 0 ? 1 : 0;
            int P8 = Neight(1,2,0,0) == 0 ? 1 : 0;
            // If the pixel is black
            if (skeleton(x,y,0,0) == 0) {
                CImg<> MyNeight(Neight.get_crop(1,1,3,3));
                int a = getA(MyNeight);
                int b = getB(MyNeight);
                if ((2 <= b && b <= 6) 
                 && (a == 1) 
                 && (P2*P4*P8 == 0 || getA(Neight.get_crop(1,0,3,2)) != 1)
                 && (P2*P4*P6 == 0 || getA(Neight.get_crop(2,1,4,3)) != 1)) {
                    res(x, y, 0, 0) = 255; 
                    change = true;
                }
            }
        }
    }
    
    return res;
}