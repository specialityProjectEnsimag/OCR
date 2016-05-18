#include "clustering_data.h"

int Forecast::forecast(CImg<> image, std::vector< CImg<>* > average, std::vector<char> labels, std::vector<forecast_type>& res) {
    int length = average.size();
    double probability[length];
    double max = 0;
    memset(probability, 0, length*sizeof(double));
    for (int i = 0; i < length; i++) {
        probability[i] = image.MSE(*average.at(i));
        if (probability[i] > max) {
            max = probability[i];
        }
    }
    
    // Computes softmax in order to have probability
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
    std::sort(res.begin(), res.end());
    mergeContiguous(res);
    
    return min;
}

std::vector<forecast_type>* Forecast::testLetter(CImg<> toTest, string average_dataset) {
    path path(average_dataset); 
    std::vector<forecast_type>* res = new std::vector<forecast_type>();
 
    // If the directory does not exist
    if(!exists(path)) { 
        std::cout << "You have not compute Kmeans !" << std::endl;
        return res;
    } 

    // Else the directory exists
    if(is_directory(path)) {
        for(directory_iterator it(path), end; it != end; ++it) { 
            if(is_directory(it->status())) {
                int k = images.size();
                // We extract all images
                if (!Utils::extract_images(it->path().string().c_str(), images)) {
                    std::cout << "Your dataset is corrupted !" << std::endl;
                    return res;
                }
                k = images.size() - k;
                for (int i = 0; i < k; i++) {
                    labels.push_back(corresponding_label(it->filename()));
                }
            } else {
                return res;
            }
        }
    }
    
    // Computes the res
    forecast(toTest, images, labels, *res);
    
    return res;
}

void Forecast::mergeContiguous(std::vector<forecast_type>& list) {
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