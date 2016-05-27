#include "hmm.h"

vector<char> HMM::viterbi(vector<char> observed_word) {
    if (observed_word.size() == 0) {
        return vector<char>();
    }
    vector<char> res;
    unsigned int length = observed_word.size();
    double T1[NUMBER_LETTER][length];
    double T2[NUMBER_LETTER][length];
    
    // Initialization
    memset(T1, 0,NUMBER_LETTER*length*sizeof(double));
    memset(T2, 0,NUMBER_LETTER*length*sizeof(double));
    for (unsigned int state = 0; state < NUMBER_LETTER; state++) {
        T1[state][0] = start_probability[state]
                *observation_probability[state][corresponding_int(observed_word.at(0))];
    }
    
    // Computing
    for (unsigned int i = 1; i < length; i++) {
        for (unsigned int statej = 0; statej < NUMBER_LETTER; statej++) {
            for (unsigned int k = 0; k < length; k++) {
                int t1ji = T1[k][i - 1] * transition_probability
                                            [statej]
                                            [corresponding_int(observed_word.at(i))];
                if (T1[statej][i] < t1ji) {
                    T1[statej][i] = t1ji;
                    T2[statej][i] = k;
                }
            }
        }
    }
    
    // Solution construction
    int imax = 0;
    for (unsigned int statek = 0; statek < NUMBER_LETTER; statek++) {
        if (T1[statek][length] > T1[imax][length]) {
            imax = statek;
        }
    }
    res.push_back(corresponding_char(imax));
    
    for (unsigned int i = length - 1; i > 0; i--) {
        res.push_back(corresponding_char(T2[imax][i]));
        imax = T2[imax][i];
    }
    
    // Order the vector
    reverse(res.begin(),res.end());
    
    return res;
    
}

void HMM::print() {
    cout << "Transition" << endl;
    printColumns(9, " ");
    for (int i = 0; i < NUMBER_LETTER; i++) {
        printColumns(9, corresponding_char(i));
    }
    cout << endl;
    for (int i = 0; i < NUMBER_LETTER; i++) {
        printColumns(9, corresponding_char(i));
        for (int j = 0; j < NUMBER_LETTER; j++) {
            printColumns(9, (float) transition_probability[i][j]);
        }
        cout << endl;
    }
    
    cout << "\nStart" << endl;
    for (int i = 0; i < NUMBER_LETTER; i++) {
        cout << "(" << corresponding_char(i) << " " << (float) start_probability[i] << ") ";
    }
    
    cout << "\nTransition" << endl;
    printColumns(9, " ");
    for (int i = 0; i < NUMBER_LETTER; i++) {
        printColumns(9, corresponding_char(i));
    }
    cout << endl;
    for (int i = 0; i < NUMBER_LETTER; i++) {
        printColumns(9, corresponding_char(i));
        for (int j = 0; j < NUMBER_LETTER; j++) {
            printColumns(9, (float) observation_probability[i][j]);
        }
        cout << endl;
    }
}
    
void HMM::learn_observation(string dataset, Forecast& forecast) {
    path path(dataset); 
 
    // If the directory does not exist
    if(!exists(path)) { 
        throw invalid_argument("Invalid path !");
    }

    // Else the directory exists
    if(is_directory(path)) {
        directory_iterator it(path), end;
        while(it != end) { 
            if(is_directory(it->status())) {
                vector< CImg<> * > images;
                image_io::extract_images(it->path().string().c_str(), images);
                for (vector< CImg<>* >::iterator img = images.begin();
                        img != images.end();
                        ++img) {
                    vector<forecast_type> prediction;
                    forecast.forecast(**img, prediction, MSE);
                    int letter = corresponding_int(corresponding_label(it->filename()));
                    for (vector<forecast_type>::iterator res = prediction.begin();
                            res != prediction.end();
                            ++res) {
                        this->observation_probability
                                [corresponding_int(res->character)]
                                [letter]
                                += res->probability/images.size();
                    }
                }
            }
            it++;
        }
    } else {
        throw invalid_argument("Arborescence unrespected !");
    }
}

void HMM::learn_transition(string dictionary) {
    string line;
    ifstream dico(dictionary);
    if (dico.is_open()) {
        // Letter transition
        int words = 0;
        while (getline(dico,line)) {
            char previous = line[0];
            char current = line[0];
            for (int letter = 0; letter < (int) line.size(); letter++) {
                current = line[letter];
                if (letter == 0) {
                    upperLower(current);
                } else if (letter == (int) line.size() - 1) {
                    last(current);
                } else {
                    transition_probability
                            [corresponding_int(previous)]
                            [corresponding_int(current)] ++;
                }
                previous = current;
            }
            words++;
        }
        for (int first = 0; first < NUMBER_LETTER; first++) {
            int tot = 0;
            for (int second = 0; second < NUMBER_LETTER; second++) {
                tot += transition_probability[first][second];
            }
            if (tot != 0) {
                // Normalization
                for (int second = 0; second < NUMBER_LETTER; second++) {
                    transition_probability[first][second] /= (double) tot;
                }
            }
        }
        // Close the dictionary
        dico.close();
        
        // Number transition
        string integer = "0123456789*/-+=.;!?,\"";
        for (unsigned int current = 0; current < integer.size(); current++) {
            for (unsigned int next = 0; next < integer.size(); next++) {
                transition_probability
                        [corresponding_int(integer[current])]
                        [corresponding_int(integer[next])] = 1./integer.size();
            }
        }
        
        // Symbol transition
        string special = "&#~{}\\";
        for (unsigned int current = 0; current < special.size(); current++) {
            for (unsigned int next = 0; next < NUMBER_LETTER; next++) {
                transition_probability
                        [corresponding_int(special[current])]
                        [next] = 1./NUMBER_LETTER;
            }
        }
        
    } else cout << "Unable to open file"; 
}

void HMM::upperLower(char letter) {
    if ('a' <= letter && letter <= 'z') {
        start_probability[corresponding_int('A' + letter - 'a')]++;
    }
    start_probability[corresponding_int(letter)]++;
}

void HMM::last(char letter){
    string end = ".;!?,\"";
    for (unsigned int next = 0; next < end.size(); next++) {
        transition_probability
                [corresponding_int(letter)]
                [corresponding_int(end[next])]++;
    }
}