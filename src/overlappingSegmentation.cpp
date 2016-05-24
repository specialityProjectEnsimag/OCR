#include "overlappingSegmentation.h"

namespace overlappingSegmentation
{
    #ifdef PRINT_DEBUG
        CImg<> colored;
        #define colorIfDebug(img,x,y){  \
            colored(x, y) -= 50;        \
        }
    #else
        #define colorIfDebug(img,x,y)     
    #endif

    /**
     * This function calls itself recursively until it reaches a black pixel
     * that stops it. Then depending on the boolean right, has the following behaviour: 
     * If right is set to true, when it reaches a black pixel
     *      -> try right
     *      -> try left
     *      -> pull back and try right only
     * if right is set to false
     *      -> try left
     *      -> pull back and try left only 
     * 
     * @param img       The image to go through
     * @param x         Current position on x
     * @param y         Current position on y
     * @param right     See explanation above
     * @param solution  When a path is found, all functions put their position
     *                  in it in order to get the path to separate the two parts
     */
    void moveToTopInternal(const CImg<>& img,
                            sol& path, 
                            int x, 
                            int y, 
                            bool right, 
                            const vector<int>& solution){
        
        if(y == 0){
            // found a way out to the top
            path = {true, -1, -1, -1, -1};
        }else if(img(x, y - 1) == WHITE_PIXEL){
            // keep going to the top
            moveToTopInternal(img, path, x, y-1, right, solution);
            
            // try alternatives if no path was found
            // here path contains both blocking pixels on the left and on the right
            // (describe as objects in the paper) that we try to bypass
            if(!path.found){
                if(right){
                    moveTopRightUntil(img, path, x, y, path.rx + 1, solution);
                }else{
                    moveTopLeftUntil(img, path, x, y, path.lx - 1, solution);
                }
            }
            colorIfDebug(img, x, y);
        }else{
            // default behaviour, try to the right and then to the left
            if(right){
                // try to go to the right
                moveTopRight(img, path, x, y, solution);
                if(!path.found){
                    // try to go after the blocking pixel in case of hole
                    // indeed the moveTopRight function would have go in the hole
                    // if there is any. With this function we try to bypass it
                    moveTopRightUntil(img, path, x, y, path.rx + 1, solution);
                    if(!path.found){
                        // can't go right, try to the left
                        // same logic
                        moveTopLeft(img, path, x, y, solution);
                        if(!path.found){
                            moveTopLeftUntil(img, path, x, y, path.lx - 1, solution);
                        }
                    }
                }   
            }else{
                // check only on the left
                // Note: We check only if there is no left obstacle discovered
                // but for test purpose, I let is that way (it should not happen
                // since we call first with right set to TRUE)
                if(path.lx == -1 && path.ly == -1){
                    moveTopLeft(img, path, x, y, solution);
                }
            }
        }
    }
    
    void moveToTop(const CImg<>& img,
                    sol& path, 
                    int x, 
                    int y, 
                    const vector<int>& solution){
        
        moveToTopInternal(img, path, x, y, true, solution);
        if(!path.found){
            moveToTopInternal(img, path, x, y, false, solution);
        }        
    }
    
    void moveTopLeft(const CImg<>& img,
                        sol& path, 
                        int x, 
                        int y,
                        const vector<int>& solution){
        
        path = {false, x, y, path.rx, path.ry};
        colorIfDebug(img, x, y);
        
        if(img(x, y-1) == WHITE_PIXEL){
            moveToTop(img, path, x, y - 1, solution);
        }else if(x > 0 && img(x-1, y) == WHITE_PIXEL){
            // keep going to the left
            moveTopLeft(img, path, x-1, y, solution);
        }
    }
    
    void moveTopRight(const CImg<>& img,
                        sol& path, 
                        int x, 
                        int y, 
                        const vector<int>& solution){
        
        path = {false, path.lx, path.ly, x, y};
        colorIfDebug(img, x, y);
        
        if(img(x, y-1) == WHITE_PIXEL){
            moveToTop(img, path, x, y - 1, solution);
        }else if(x < img._width - 1 && img(x+1, y) == WHITE_PIXEL){
            // keep going to the right
            moveTopRight(img, path, x+1, y, solution);
        }
    }
    
    void moveTopLeftUntil(const CImg<>& img,
                                sol& path,
                                int x,
                                int y,
                                int destX,
                                const vector<int>& solution){
        
        do{
            colorIfDebug(img, x, y);
            x--;
        }while(img(x, y) == WHITE_PIXEL && x > destX);
        
        // reached the destination column
        if(x == destX && img(x, y) == WHITE_PIXEL){
            // keep going to the left looking for a way to go up
            moveTopLeft(img, path, x, y, solution);
        }else{
            // means that (x, y) is a black pixel, so sets the pixel before
            path = {false, x + 1, y, path.rx, path.ry};
        }
    }
    
    void moveTopRightUntil(const CImg<>& img,
                            sol& path, 
                            int x,
                            int y,
                            int destX,
                            const vector<int>& solution){
        
        destX = std::min(destX, (int)(img._width - 1));

        do{
            colorIfDebug(img, x, y);
            x++;
        }while(img(x, y) == WHITE_PIXEL && x < destX);
        
        // reached the destination column
        if(x == destX && img(x, y) == WHITE_PIXEL){
            // keep going to the right looking for a way to go up
            moveTopRight(img, path, x, y, solution);
        }else{
            // means that (x, y) is a black pixel, so sets the pixel before
            path = {false, path.lx, path.ly, x - 1, y};
        }
    }
    
    /**
     * This function calls itself recursively until it reaches a black pixel
     * that stops it. Then depending on the boolean left, has the following behaviour: 
     * If left is set to true, when it reaches a black pixel
     *      -> try left
     *      -> try right
     *      -> pull back and try leftonly
     * if left is set to false
     *      -> try right
     *      -> pull back and try right only 
     * 
     * @param img
     * @param x
     * @param y
     * @param right
     * @return 
     */
    void moveToBottomInternal(const CImg<>& img,
                                sol& path,
                                int x,
                                int y,
                                bool left,
                                const vector<int>& solution){
        
        if(y == img._height - 1){
            path = {true, -1, -1, -1, -1};
        }else if(img(x, y + 1) == WHITE_PIXEL){

            moveToBottomInternal(img, path, x, y+1, left, solution);
            if(!path.found){
                if(left){
                    moveBottomLeftUntil(img, path, x, y, path.lx - 1, solution);
                }else{
                    moveBottomRightUntil(img, path, x, y, path.rx + 1, solution);
                }
            }
            colorIfDebug(img, x, y);
        }else{
            // default behaviour, try to the right and then to the left
            if(left){
                // can't go up, try to go to the left
                moveBottomLeft(img, path, x, y, solution);
                if(!path.found){
                    moveBottomLeftUntil(img, path, x, y, path.lx - 1, solution);
                    if(!path.found){
                        // can't go right, try to the left
                        moveBottomRight(img, path, x, y, solution);
                        if(!path.found){
                            moveBottomRightUntil(img, path, x, y, path.rx + 1, solution);
                        }
                    }
                }   
            }else{
                // check only on the left
                if(path.rx == -1 && path.ry == -1){
                    moveBottomRight(img, path, x, y, solution);
                }
            }
        }
    }
    

    void moveToBottom(const CImg<>& img,
                            sol& path, 
                            int x,
                            int y,
                            const vector<int>& solution){
        
        moveToBottomInternal(img, path, x, y, true, solution);
        if(!path.found){
            moveToBottomInternal(img, path, x, y, false, solution);
        }        
    }
    
    void moveBottomLeft(const CImg<>& img,
                            sol& path,
                            int x, 
                            int y, 
                            const vector<int>& solution){
        
        path = {false, x, y, path.rx, path.ry};
        colorIfDebug(img, x, y);
        
        if(img(x, y+1) == WHITE_PIXEL){
            moveToBottom(img, path, x, y + 1, solution);
        }else if(x > 0 && img(x-1, y) == WHITE_PIXEL){
            // keep going to the left
            moveBottomLeft(img, path, x-1, y, solution);
        }
    }
    
    void moveBottomRight(const CImg<>& img,
                            sol& path, 
                            int x,
                            int y,
                            const vector<int>& solution){
        
        colorIfDebug(img, x, y);
        
        if(img(x, y+1) == WHITE_PIXEL){ 
            moveToBottom(img, path, x, y + 1, solution);
        }else if(x == img._width - 1){  
            // went as far as possible to the right and can't go up
            path = {false, path.lx, path.ly, x, y};
        }else if(img(x+1, y) == WHITE_PIXEL){
            // keep going to the right
            moveBottomRight(img, path, x+1, y, solution);
        }else{
            // blocked by a black pixel
            path = {false, path.lx, path.ly, x, y};
        }
    }
    
    void moveBottomLeftUntil(const CImg<>& img,
                                sol& path, 
                                int x, 
                                int y,
                                int destX,
                                const vector<int>& solution){
        
        do{
            colorIfDebug(img, x, y);
            x--;
        }while(img(x, y) == WHITE_PIXEL && x > destX);
        
        // reached the destination column
        if(x == destX && img(x, y) == WHITE_PIXEL){
            // keep going to the left looking for a way to go up
            moveBottomLeft(img, path, x, y, solution);
        }else{
            path = {false, x + 1, y, path.rx, path.ry};
        }
    }
    
    void moveBottomRightUntil(const CImg<>& img, 
                                    sol& path, 
                                    int x, 
                                    int y,
                                    int destX,
                                    const vector<int>& solution){
        
        destX = std::min(destX, (int)(img._width - 1));

        do{
            colorIfDebug(img, x, y);
            x++;
        }while(img(x, y) == WHITE_PIXEL && x < destX);
        
        // reached the destination column
        if(x == destX && img(x, y) == WHITE_PIXEL){
            // keep going to the right looking for a way to go up
            moveBottomRight(img, path, x, y, solution);
        }else{
            path = {false, path.lx, path.ly, x - 1, y};
        }
    }
    
    void splitCharLine(const CImg<>& image, CImg<>& left, CImg<>& right, int* line){
        int index = 0;
        cimg_forY(image, y) {
            cimg_forX(image, x) {
                if (x < line[index]) {
                    left(x,y) = image(x,y);
                    right(x,y) = WHITE_PIXEL;
                } else {
                    left(x,y) = WHITE_PIXEL;
                    right(x,y) = image(x,y);
                }
            }
            index ++;
        }
        right = projection::reduce(right);
        left = projection::reduce(left);
    }

    bool stop(const CImg<>& image){
        cimg_forXY(image, x, y) {
            if (image(x,y) != WHITE_PIXEL) {
                return false;
            }  
        }
        return true;
    }
}
