#include "clustering_data.h"

void kmeans_directory(std::vector< CImg<>* >& images, const int k, const int iteration, string average_dataset) {
    if ((int)images.size() > k) {
        vector< CImg<>*> average; 
        // Initialisation of the average vector 
        for (int i = 0; i < k; i++) {
            // Random Choose of images with copy
            CImg<>* tmp = new CImg<>(*images.at(rand()%images.size()));
            average.push_back(tmp);
        }

        for (int i = 0; i < iteration; i++) {
            vector< CImg<>* > predicted_class[k];
            for (vector< CImg<>* >::iterator it = images.begin(); it != images.end(); ++it) {
                // We compute the best categorie
                int indice = Forecast::indexOfClosest(**it, average, MSE);
                predicted_class[indice].push_back(*it);
            }

            // Compute the average of the new images
            for (int j = 0; j < k; j++) {
                if (predicted_class[j].size() != 0) {
                    *average[j] = image_io::average(predicted_class[j]);
                }
            }
        }
        // Save images
        image_io::exportAll_compressed(average_dataset.c_str(), average);
        image_io::delete_images(average);
    } else {
        image_io::exportAll_compressed(average_dataset.c_str(), images);
    }
}

bool kmeans(string dataset, string average_dataset, const int k, const int iteration) {
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
                            if (image_io::extract_images(path.c_str(), images)) {
                                // We compute the final average images
                                preprocessing::preprocessing(images);
                                string file = average_dataset + "/" +filename + "/" + (filename);
                                kmeans_directory(images, k, iteration, file);
                            }
                            // Free images
                            image_io::delete_images(images);
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