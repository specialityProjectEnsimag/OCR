#include "forecast.h"

/**
 * Merge all contigus letter from the given list
 * @param list
 */
void mergeContiguous(std::vector<forecast_type>& list) {
    std::vector<forecast_type>::iterator j = list.begin();
    std::vector<forecast_type>::iterator i = ++list.begin();
    while (i != list.end()) {
        if (i->character == j->character) {
            j->probability += i->probability;
            i = list.erase(i);
        } else {
            j = i;
            i++;
        }
    }
}

double dist (CImg<> src, CImg<> average) {
    #ifdef EUCLIDEANDISTANCE
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
    #else
        #ifdef CHAMFER
            // Higher the result will be, closest are the images
            return -chamfer::bin(src).get_mul(average).sum();
        #else 
            return src.MSE(average);
        #endif
    #endif
}

Forecast::Forecast(string average_dataset) {
    path path(average_dataset); 
    // If the directory does not exist
    if(!exists(path)) { 
        std::cout << "You have not compute Kmeans !" << std::endl;
    } 

    // Else the directory exists
    if(is_directory(path)) {
        for(directory_iterator it(path), end; it != end; ++it) { 
            if(is_directory(it->status())) {
                int k = images.size();
                // We extract all images WITHOUT ANY PREPROCESSING
                if (!image_io::extract_images(it->path().string().c_str(), images)) {
                    std::cout << "Your dataset is corrupted !" << std::endl;
                }
                k = images.size() - k;
                for (int i = 0; i < k; i++) {
                    labels.push_back(corresponding_label(it->filename()));
                }
            }
        }
    }
}

#ifdef KNN
int Forecast::forecast(CImg<> image, std::vector<forecast_type>& res, std::vector< CImg<>* > average, std::vector<char> labels) {
    int length = average.size();

    std::vector<forecast_type> tmp;
    
    for (int i = 0; i < length; i++) {
        double proba = dist(image, *average.at(i));
        forecast_type obj;
        obj.probability = proba;
        obj.character = labels[i];
        tmp.push_back(obj);
    }
    
    std::sort(tmp.begin(), tmp.end(), less<forecast_type>());

    std::vector<forecast_type>::iterator i = tmp.begin();
    int index = 0;
    int min = 0;
    int sameNeight = 0;
    // Compute the KNN
    while (i != tmp.end() && sameNeight < NUMBERNEIGHT) {
        std::vector<forecast_type>::iterator j = res.begin();
        while (j != res.end()) {
            if (j->character == i->character) {
                j->probability += 1;
                sameNeight = j->probability;
                min = index;
                break;
            } else {
                j++;
            }
        }
        if (j == res.end()) {
            forecast_type obj;
            obj.probability = 1;
            obj.character = i->character;
            res.push_back(obj);
        }
        i++;
        index ++;
    }
    
    std::sort(res.begin(), res.end(), greater<forecast_type>());
    
    for (vector<forecast_type>::iterator it = res.begin(); it != res.end(); ++it) {
        it->probability /= index;
    }
    
    return min;
}

#else
int Forecast::forecast(CImg<> image, std::vector<forecast_type>& res, std::vector< CImg<>* > average, std::vector<char> labels) {
    int length = average.size();
    double probability[length];
    double max = 0;
    memset(probability, 0, length*sizeof(double));
    for (int i = 0; i < length; i++) {
        probability[i] = dist(image, *average.at(i));
        if (probability[i] > max) {
            max = probability[i];
        }
    }
    
    
    // Computes softmax in order to have probability
    // of the NUMBERLETTER first recognized letter
    // Normalization to have small numbers
    double sum = 0;
    for (int i = 0; i < length; i++) {
        // -1 is in order to reverse the function
        // A high error has a little probability
        probability[i] = exp(-probability[i]/max);
        sum += probability[i];
    }
    int min = 0;
    for (int i = 0; i < length; i++) {
        probability[i] /= sum;
        forecast_type obj;
        obj.probability = probability[i];
        obj.character = labels[i];
        res.push_back(obj);
        if (probability[min] < probability[i]) {
            min = i;
        }
    }
    
    // Sort res
    std::sort(res.begin(), res.end(), greater<forecast_type>());
    mergeContiguous(res);
    
    return min;
}
#endif