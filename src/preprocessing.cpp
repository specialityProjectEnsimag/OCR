#include "preprocessing.h"
#include "image_io.h"

namespace preprocessing 
{
    /**
     * Computes the repartition of gray
     * @param img       The image to study
     * @param gray      An array for different shades
     * @param length    The length of the array
     */
    void gray_repartition(const CImg<>& img, int* gray, int length) {   
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
    int threshold(const CImg<>& img) {
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

    CImg<> otsu_binarization(const CImg<>& src) {
        CImg<> binarized(src);
        int lvl = threshold(src);
        cimg_forXY(src, x, y) {
            binarized(x, y, 0, 0) = src(x, y, 0, 0) > lvl ? WHITE_PIXEL : BLACK_PIXEL;
        }

        return binarized;
    }

    CImg<> linear_filter(const CImg<>& src, const CImg<>& mask) {    
        CImg<> smoothed(src);

        CImg<> Neight(3,3);
        cimg_for3x3(src,x,y,0,0,Neight,float) {
            smoothed(x, y, 0, 0) = Neight.get_mul(mask).sum()/(mask.sum());
        }

        return smoothed;
    }
    
    CImg<> median_filter(const CImg<>& src) {
        CImg<> smoothed(src);

        CImg<> Neight(3,3);
        cimg_for3x3(src,x,y,0,0,Neight,float) {
            float* tmp = Neight._data;
            sort(tmp, tmp+9);
            smoothed(x, y, 0, 0) = tmp[5];
        }

        return smoothed;
    }

    CImg<> noise(const CImg<>& src) {
        CImg<> clean (src);
        clean.mirror('x');
        clean.mirror('y');
        CImg<> Neight(3,3);
        bool change = true;
        //int tour = 0 ;
        //while (change) {
            change = false;
            CImg<> tmp(clean);
            cimg_for3x3(tmp,x,y,0,0,Neight,float) {
                float P2 = Neight(1,0,0,0);
                float P3 = Neight(2,0,0,0);
                float P4 = Neight(2,1,0,0);
                float P5 = Neight(2,2,0,0);
                float P6 = Neight(2,1,0,0);
                float P7 = Neight(2,0,0,0);
                float P8 = Neight(1,0,0,0);
                float P9 = Neight(0,0,0,0);
                // P2 = P3 = P4 = P5 = P6
                if (P2 == P3 && P3 == P4 && P4 == P5 && P5 == P6) {
                    clean(x,y,0,0) = P2;
                    change = true;
                // P4 = P5 = P6 = P7 = P8
                } else if (P4 == P5 && P5 == P6 && P6 == P7 && P7 == P8) {
                    clean(x,y,0,0) = P4;
                    change = true;
                // P6 = P7 = P8 = P9 = P2
                } else if (P6 == P7 && P7 == P8 && P8 == P9 && P9 == P2) {
                    clean(x,y,0,0) = P6;
                    change = true;
                // P8 = P9 = P2 = P3 = P4
                } else if (P8 == P9 && P9 == P2 && P2 == P3 && P3 == P4) {
                    clean(x,y,0,0) = P8;
                    change = true;
                }
            }
        //}
        clean.mirror('x');
        clean.mirror('y');
        return clean;
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
    int getA(const CImg<>& src) {
        int res = 0;
        // P2 == WHITE_PIXEL and P3 == BLACK_PIXEL
        if (src(1,0,0,0) == WHITE_PIXEL && src(2,0,0,0) == BLACK_PIXEL) res++;
        // P3 == WHITE_PIXEL and P4 == BLACK_PIXEL
        if (src(2,0,0,0) == WHITE_PIXEL && src(2,1,0,0) == BLACK_PIXEL) res++;
        // P4 == WHITE_PIXEL and P5 == BLACK_PIXEL
        if (src(2,1,0,0) == WHITE_PIXEL && src(2,2,0,0) == BLACK_PIXEL) res++;
        // P5 == WHITE_PIXEL and P6 == BLACK_PIXEL
        if (src(2,2,0,0) == WHITE_PIXEL && src(1,2,0,0) == BLACK_PIXEL) res++;
        // P6 == WHITE_PIXEL and P7 == BLACK_PIXEL
        if (src(1,2,0,0) == WHITE_PIXEL && src(0,2,0,0) == BLACK_PIXEL) res++;
        // P7 == WHITE_PIXEL and P8 == BLACK_PIXEL
        if (src(0,2,0,0) == WHITE_PIXEL && src(0,1,0,0) == BLACK_PIXEL) res++;
        // P8 == WHITE_PIXEL and P9 == BLACK_PIXEL
        if (src(0,1,0,0) == WHITE_PIXEL && src(0,0,0,0) == BLACK_PIXEL) res++;
        // P9 == WHITE_PIXEL and P2 == BLACK_PIXEL
        if (src(0,0,0,0) == WHITE_PIXEL && src(1,0,0,0) == BLACK_PIXEL) res++;
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
    int getB(const CImg<>& src) {
        int res = 0;
        cimg_forXY(src, x, y) {
            // Not compute for the central pixel
            if (!(x == 1 && y == 1)) {
                res += src(x, y, 0, 0) == 0 ? 1 : 0;
            }
        }
        return res;
    }

    CImg<> skeletonization(const CImg<>& src) {
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
                        res(x, y, 0, 0) = WHITE_PIXEL; 
                        change = true;
                    }
                }
            }
        }

        return res;
    }
    
    CImg<> rotate(const CImg<>& src, double angle) {
        float angle_mod = angle;
        double c = cos(angle_mod/180.0*PI);
        double s = sin(angle_mod/180.0*PI);
        double h = abs(c*src.height()) + abs(s*src.width());
        double w = abs(s*src.height()) + abs(c*src.width());
        
        // Create a copy
        CImg<> res((int) w, (int) h, src._depth, src._spectrum, WHITE_PIXEL);

        // Compute the rotation      
        cimg_forXY(res, x, y) {
            float i = src._width/2. + (x -w/2.)*c + (y -h/2.)*s; 
            float j = src._height/2 + (y -h/2.)*c - (x -w/2.)*s;
            if (0 <= i && i < src.width() && 0 <= j && j < src.height())
                res(x,y) = src.linear_atXYZC(i,j);
        }
        return res;
    }
    
    CImg<> hough_rotation(const CImg<>& src,const double deg_min, const double deg_max, const int step) {
        // We major the possible line to the width of the image
        int hypo = sqrt(pow(src.width(),2) + pow(src.height(),2));
        CImg<> hough(step, hypo, 1, 1, 0);
        double dth = (deg_max - deg_min) / step;
        
        int max_color = 0;
        cimg_forXY(src, x, y) {
            if (src(x,y) != WHITE_PIXEL) {
                for (int s = 0; s < step; s++) {
                    double th = deg_min + s * dth;
                    int r = (int) (x*cos(th*PI/180) + y*sin(th*PI/180));
                    if (0 <= r && r < hypo) {
                        hough(s, r) += 1;
                        if (hough(s, r) > max_color) {
                            max_color = hough(s, r);
                        }
                    }
                }
            }
        }

        // Compute the angle of rotation
        int angle[step];
        int dark[step];
        int white[step];
        
        int max = 0;
        bool active = true;
        
        memset(angle, 0, sizeof(int)*step);
        memset(dark, 0, sizeof(int)*step);
        memset(white, 0, sizeof(int)*step);
        
        cimg_forX(hough, x) {
            active = true;
            cimg_forY(hough, y) {
                if (hough(x,y) > 0.9*max_color) {
                    white[x] ++;
                    if (active) {
                        angle[x] ++;
                        active = false;
                    }
                }
                if (hough(x,y) < 0.1*max_color) {
                    active = true;
                    dark[x] ++;
                }
            }
            if (angle[x] > angle[max]) {
                max = x;
            } else if (angle[x] == angle[max] && dark[x] > dark[max]) {
                max = x;
            } else if (angle[x] == angle[max] && dark[x] == dark[max] && white[x] < white[max]) {
                max = x;
            }
        }
     
                cimg_forY(hough, y) {
                    hough(max, y ) = 150;
                }
                image_io::displayImage(hough);
        // Rotation
        return rotate(src,90-(deg_min + max*dth));
    }
    
     void preprocessing(CImg<>& image){
        image = otsu_binarization(image);
        #ifdef SKELETONIZATION
            image = skeletonization(image);
            #ifdef CHAMFER
                image = chamfer::chamfer(image);
            #else
                float tab[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
                CImg<> mask(tab,3,3);
                image = linear_filter(image, mask);
                image = otsu_binarization(image);
            #endif
        #endif
    }

    void preprocessing(vector< CImg<>* >& images){
        for(vector< CImg<>* >::iterator it = images.begin(); it != images.end(); ++it) { 
            preprocessing(**it);
        }
    }
}