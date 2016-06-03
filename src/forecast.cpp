#include "forecast.h"

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
                if (!image_io::extract_images_compressed(it->path().string().c_str(), images)) {
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
void Forecast::forecast(const CImg<>& image, std::vector<forecast_type>& res, const std::vector< CImg<>* >& average, const std::vector<char>& labels, double (*dist)(const CImg<>&, const CImg<>&)) {
    int length = average.size();

    std::vector<forecast_type> tmp;
    
    for (int i = 0; i < length; i++) {
        double proba = (*dist)(image, *average.at(i));
        forecast_type obj;
        obj.probability = proba;
        obj.character = labels[i];
        tmp.push_back(obj);
    }
    
    std::sort(tmp.begin(), tmp.end(), less<forecast_type>());

    std::vector<forecast_type>::iterator i = tmp.begin();
    int index = 0;
    int sameNeight = 0;
    // Compute the KNN
    while (i != tmp.end() && sameNeight < NUMBERNEIGHT) {
        std::vector<forecast_type>::iterator j = res.begin();
        while (j != res.end()) {
            if (j->character == i->character) {
                j->probability += 1;
                sameNeight = j->probability;
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
}

#else
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

void Forecast::forecast(const CImg<>& image, std::vector<forecast_type>& res, const std::vector< CImg<>* >& average, const std::vector<char>& labels, double (*dist)(const CImg<>&, const CImg<>&)) {
    int length = average.size();
    double probability[length];
    double max = 0;
    memset(probability, 0, length*sizeof(double));
    for (int i = 0; i < length; i++) {
        probability[i] = (*dist)(image, *average.at(i));
        if (fabs(probability[i]) > max) {
            max = fabs(probability[i]);
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
    for (int i = 0; i < length; i++) {
        probability[i] /= sum;
        forecast_type obj;
        obj.probability = probability[i];
        obj.character = labels[i];
        res.push_back(obj);
    }
    
    // Sort res
    std::sort(res.begin(), res.end(), greater<forecast_type>());
    mergeContiguous(res);
}
#endif

void select( const vector< CImg<>* >& average, const vector<char>& labels, vector< CImg<>* >& sub_average, vector<char>& sub_labels, string s) {
    assert(average.size() == labels.size());
    int length = labels.size();
    for (unsigned int char_number = 0; char_number < s.size(); char_number++) {
        int l = 0;
        while (l < length) {
            if (s[char_number] == labels[l]) {
                sub_average.push_back(average[l]);
                sub_labels.push_back(labels[l]);
            }
            l++;
        }
    }
}

void Forecast::forecast(const text_character& image, vector<forecast_type>& res, const vector< CImg<>* >& average, const vector<char>& labels,double (*dist)(const CImg<>&, const CImg<>&)) {
    vector< CImg<>* > sub_average;
    vector<char> sub_labels;
    string characters;
    switch(image.upLow) {
        case -1:
            // Do not know what to do
            sub_average = average;
            sub_labels = labels;
            break;
        case 0:
            // Up
            characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZtidfhkl?/!\\(){}&*0123456789"; 
            select(average, labels, sub_average, sub_labels, characters);
            break;
        case 1:
            // Middle
            characters = "azeruosmwxcvn;:+-"; 
            select(average, labels, sub_average, sub_labels, characters);
            break;
        case 2:
            // Low
            characters = "ypqgj"; 
            select(average, labels, sub_average, sub_labels, characters);
            break;
        case 3:
            // Ponctuation
            characters = ".,"; 
            select(average, labels, sub_average, sub_labels, characters);
            break;
        default:
            throw invalid_argument("forecast : No character of this type");
    }
    forecast(image.img, res, sub_average, sub_labels, dist);
}

int Forecast::indexOfClosest(const CImg<>& image, const vector< CImg<>* >& average, double (*dist)(const CImg<>&, const CImg<>&)) {
    int length = average.size();
    double probability[length];
    int min = 0;
    for (int i = 0; i < length; i++) {
        probability[i] = (*dist)(image, *average.at(i));
        if (probability[min] > probability[i]) {
            min = i;
        }
    }
    return min;
}
