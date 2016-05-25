#include "chamfer.h"

namespace chamfer 
{
    CImg<> chamfer(const CImg<>& image){
        CImg<> res((image));
        float max = 0;
        
        CImg<> N(3,3);
        cimg_for3x3(res, x, y, 0, 0, N, float){
            res(x,y) = 1+min(N(0,1), min(N(0,0), min(N(1,0), N(2,0))));  
        }
        
        // Second pass needs to go from buttom right
        res.mirror('x');
        res.mirror('y');
        cimg_for3x3(res, x, y, 0, 0, N, float){
            res(x,y) = 1+min(N(0,1), min(N(0,0), min(N(1,0), N(2,0))));  
            if (res(x,y) > max) {
                max = res(x,y);
            }
        }
        res.mirror('y');
        res.mirror('x');
        
        return max - res;
    }
    
    CImg<> bin(const CImg<>& image){
        CImg<> res(image);
        
        cimg_forXY(res, x, y){
            res(x,y) = res(x,y) == WHITE_PIXEL ? 0 : 1;  
        }
        
        return res;
    }
}
