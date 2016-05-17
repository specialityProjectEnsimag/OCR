#include "clustering_data.h"

int forecast(CImg<> image, CImg<>* average, forecast_type* res, int length) {
    double probability[length];
    double max = 0;
    memset(probability, 0, length*sizeof(double));
    for (int i = 0; i < length; i++) {
        probability[i] = image.MSE(average[i]);
        if (probability[i] > max) {
            max = probability[i];
        }
    }
    
    // Computes softmax in order to have probability
    // Normalization to have small numbers
    double sum = 0;
    for (int i = 0; i < length; i++) {
        // -1 is in order to resevere the function
        // A high error has a little probability
        probability[i] = exp(-probability[i]/max);
        sum += probability[i];
    }
    int min = 0;
    for (int i = 0; i < length; i++) {
        probability[i] /= sum;
        if (probability[min] < probability[i]) {
            min = i;
        }
    }
    
    // TODO Fill res in order to have letter and there probability
    
    return min;
}
