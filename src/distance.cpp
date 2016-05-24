#include "distance.h"
       
double Euclidean(CImg<> src, CImg<> average) {
    double res = 0;
    cimg_forXY(src,x,y) {
        if (src(x,y) == 0) {
            double min = 0;
            cimg_forXY(average,u,v) {
                if (average(u,v) == 0) {
                    double tmp = sqrt(pow((u-x),2)+ pow((v-y),2));
                    if (tmp < min) {
                        min = tmp;
                    }
                }
            }
            res += min;
        }
    }
    return res;
}

double Chamfer(CImg<> src, CImg<> average) {
    // Higher result is, closier images are,
    // It is why we multiplie by -1
    return -chamfer::bin(src).get_mul(average).sum();
}

double MSE(CImg<> src, CImg<> average) {
    // Higher result is, closier images are,
    // It is why we multiplie by -1
    return src.MSE(average);
}


