#include "clustering_data.h"

void kmeans_directory(std::vector< CImg<>* >& images, char label, int k, int iteration, string average_dataset) {
    vector < CImg<>* > average;
    vector < char > labels;
    
    // Initialisation of the average vector 
    for (int i = 0; i < k; i++) {
        // TODO : Random Choose of images
        average.push_back(images.at(i));
        labels.push_back(label);
    }
        
    for (int i = 0; i < iteration; i++) {
        vector< CImg<>* > predicted_class[k];
        for (vector< CImg<>* >::iterator it = images.begin(); it != images.end(); ++it) {
            vector< forecast_type > res;
            // We compute the best categorie
            int indice = Forecast::forecast(**it, res, average, labels);
            predicted_class[indice].push_back(*it);
        }
        
        // Compute the average of the new images
        for (int j = 0; j < k; j++) {
            if (predicted_class[j].size() != 0) {
                *average[j] = Utils::average(predicted_class[j]);
            }
        }
    }
    // Save images
    Utils::exportAll(average_dataset.c_str(), average);
}

bool kmeans(string dataset, string average_dataset, int k, int iteration) {
    path path(dataset); 
 
    // If the directory does not exist
    if(!exists(path)) { 
        cout << "Invalid path !" << endl;
        return false;
    } else if (is_directory(average_dataset)) {
        cout << "Result path already exists !" << endl;
        return false;
    }
    // Create directory of result images 
    create_directories(average_dataset);
 
    // Else the directory exists
    if(is_directory(path)) {
        #pragma omp parallel
        {
            #pragma omp single
            {
                directory_iterator it(path), end;
                while(it != end) { 
                    if(is_directory(it->status())) {
                        string path = it->path().string();
                        string filename = it->filename(); 
                        create_directories(average_dataset + "/" +filename);
                        #pragma omp task firstprivate(path, filename)
                        {
                            // We enter in a new directory of images
                            vector< CImg<>* > images;

                            // We extract all images
                            if (Utils::extract_images(path.c_str(), images)) {
                                // We compute the final average images
                                string file = average_dataset + "/" +filename + "/" + (filename);
                                kmeans_directory(images, corresponding_label(filename), k, iteration, file);
                            }
                            // Free images
                            Utils::delete_images(images);
                        }
                    }
                    it++;
                }
                #pragma omp taskwait
            }
        }
        return true;
    }
    cerr << "Arborescence unrespected !" << endl;
    return false;
}

char corresponding_label(string dir) {
    // This function is parametrated for the dataset
    int value = atoi(dir.substr(6,3).c_str());
    if (value < 11) {
        return value - 1 + '0';
    } else if (value < 37) {
        return value - 11 + 'A';
    } else {
        return value - 37 + 'a';
    }
}