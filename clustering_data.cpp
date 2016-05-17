#include "clustering_data.h"

void kmeans_directory(std::vector< CImg<>* >& images, int k, int iteration, string average_dataset) {
    CImg<> average[k];
    
    // Initialisation of the average vector 
    for (int i = 0; i < k; i++) {
        // TODO : Random Choose of images
        average[i] = *images.at(i);
    }
        
    for (int i = 0; i < iteration; i++) {
        cout << "\r" << i+1 << "/" << iteration << flush;
        std::vector< CImg<>* > predicted_class[k];
        for (std::vector< CImg<>* >::iterator it = images.begin(); it != images.end(); ++it) {
            forecast_type res[k];
            // We compute the best categorie
            int indice = forecast(**it, average, res, k);
            predicted_class[indice].push_back(*it);
        }
        
        // Compute the average of the new images
        for (int j = 0; j < k; j++) {
            if (predicted_class[j].size() != 0) {
                average[j] = Utils::average(predicted_class[j]);
            }
        }
    }
    cout << endl;
    // Save images
    Utils::exportAll((average_dataset+"/").c_str(), average, k);
}

bool kmeans(string dataset, string average_dataset, int k, int iteration) {
    path path(dataset); 
 
    // If the directory does not exist
    if(!exists(path)) { 
        std::cout << "Invalid path !" << std::endl;
        return false;
    } else if (is_directory(average_dataset)) {
        std::cout << "Result path already exists !" << std::endl;
        return false;
    }
    // Create directory of result images 
    create_directories(average_dataset);
 
    // Else the directory exists
    if(is_directory(path)) {
        for(directory_iterator it(path), end; it != end; ++it) { 
            if(is_directory(it->status())) {
                
                // We enter in a new directory of images
                std::vector< CImg<>* > images;
                
                // We extract all images
                if (Utils::extract_images(it->path().string().c_str(), images)) {
                    // We compute the final average images
                    string file = average_dataset + "/" + (it->filename());
                    cout << "Creation : " << file << endl;
                    create_directories(file);
                    kmeans_directory(images, k, iteration, file);
                    // Free images
                    Utils::delete_images(images);
                } else {
                    return false;
                }
            }
        }
        return true;
    }
    std::cerr << "Arborescence unrespected !" << std::endl;
    return false;
}
