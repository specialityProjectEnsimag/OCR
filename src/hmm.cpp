#include "hmm.h"

string HMM::viterbi(string dictionary, vector<forecast_type> observed_word) {
    if (observed_word.size() == 0) {
        return "";
    }
    string res;
    unsigned int length = observed_word.size();
    double T1[NUMBER_LETTER][length];
    double T2[NUMBER_LETTER][length];
    
    // Initialization
    memset(T1, 0,NUMBER_LETTER*length*sizeof(double));
    memset(T2, 0,NUMBER_LETTER*length*sizeof(double));
    for (unsigned int state = 0; state < NUMBER_LETTER; state++) {
        T1[state][0] = start_probability[state]
                *observation_probability[state][corresponding_int(observed_word.at(0).character) + NUMBER_LETTER*(observed_word.at(0).upLow+1)];
    }

    // Computing
    for (unsigned int i = 1; i < length; i++) {
        for (unsigned int statej = 0; statej < NUMBER_LETTER; statej++) {
            for (unsigned int k = 0; k < NUMBER_LETTER; k++) {
                double t1ji = T1[k][i-1] * transition_probability
                                            [k]
                                            [statej]
                                         * observation_probability
                                            [statej][corresponding_int(observed_word.at(i).character) + NUMBER_LETTER*(observed_word.at(i).upLow+1)]
                                            ;
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
        if (T1[statek][length-1] > T1[imax][length-1]) {
            imax = statek;
        }
    }
    res += corresponding_char(imax);
    
    for (int i = (int) length-1; i >= 1; i--) {
        res += corresponding_char(T2[imax][i]);
        imax = T2[imax][i];
    }
    
    // Order the vector
    res = string (res.rbegin(),res.rend());
    
    #ifdef LEVENSHTEIN
    return levenshtein(dictionary, res);
    #else
    return res;
    #endif
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
    
    cout << "\nObservation" << endl;
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

/**
 * Distance of Levenshtein between two words using two sliding columns
 * @param word      The first word to compare (low case only)
 * @param reference The second (low case only)
 * @return          The distance between both
 */
int distanceLevenshtein(string word, string reference) {
    if (word == reference) return 0;
    if (word.length() == 0) return word.length();
    if (reference.length() == 0) return reference.length();

    int length = reference.length() + 1;
    
    int c1[length];
    int c2[length];

    // Initialize the colum
    for (int i = 0; i < length; i++) {
        c1[i] = i;
    }

    for (unsigned int i = 0; i < word.length(); i++) {
        c2[0] = i + 1;

        // Compute the distance
        for (unsigned int j = 0; j < reference.length(); j++) {
            bool cost = (reference[i] == word[j]) ? 0 : 1;
            c2[j + 1] = min(c2[j] + 1, min(c1[j + 1] + 1, c1[j] + cost));
        }

        // Move from one the column
        for (int j = 0; j < length; j++) {
            c1[j] = c2[j];
        } 
    }

    return c2[reference.length()];
}

vector<string> lowercase(string word) {
    vector<string> res;
    string tmp;
    for (unsigned int j = 0; j < word.length(); j++) {
        if ('A' <= word[j] && word[j] <= 'Z') {
            tmp += 'a' + word[j] - 'A';
        } else if ('a' <= word[j] && word[j] <= 'z') {
            tmp += word[j];
        } else {
            res.push_back(tmp);
            tmp = word[j];
            res.push_back(tmp);
            tmp = "";
        }
    } 
    res.push_back(tmp);
    return res;
}

string HMM::levenshtein(string dictionary, string word) {
    string word_dictionnary;
    string most_probable;
    int min = 100;
    vector<string> words(lowercase(word));
    for (unsigned int i = 0; i < words.size(); i++) {
        if (!words[i].empty()) {
            ifstream dico(dictionary);
            string closest;
            if (dico.is_open()) {
                // Letter transition
                while (getline(dico,word_dictionnary)) {

                    int dL = distanceLevenshtein(word_dictionnary, words[i]);
                    if (dL <= min) {
                        min = dL;
                        closest = string(word_dictionnary);                     
                    }
                }
                // Close the dictionary
                dico.close();
            }
            // Insertion of the other character
            most_probable += closest;
            if (i < words.size() - 1) {
                i++;
                most_probable += words[i];
            }
        }
    }
    
    // Correction of the first letter
    if ('A' <= word[0] && word[0] <= 'Z' && 'a' <= most_probable[0] && most_probable[0] <= 'z') {
        most_probable[0] = 'A' + most_probable[0] - 'a';
    }
    return most_probable;
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
                    int letter = corresponding_int(corresponding_label(it->filename()));
                    for (int i = -1; i < 4; i++) {
                        vector<forecast_type> prediction;
                        forecast.forecast(text_character(**img,i), prediction, MSE);  
                        for (vector<forecast_type>::iterator res = prediction.begin();
                                res != prediction.end();
                                ++res) {
                            this->observation_probability
                                    [corresponding_int(res->character)]
                                    [letter + NUMBER_LETTER*(i+1)]
                                    += res->probability/images.size();
                        }
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
            start_probability[corresponding_int(integer[current])] ++;
        }
        
        // Symbol transition
        string special = "&#~{}\\";
        for (unsigned int current = 0; current < special.size(); current++) {
            for (unsigned int next = 0; next < NUMBER_LETTER; next++) {
                transition_probability
                        [corresponding_int(special[current])]
                        [next] = 1./NUMBER_LETTER;
            }
            start_probability[corresponding_int(special[current])] ++;
        }
        
        // Normalization of transition
        int tot_start = 0;
        for (int first = 0; first < NUMBER_LETTER; first++) {
            int tot = 0;
            for (int second = 0; second < NUMBER_LETTER; second++) {
                tot += transition_probability[first][second];
            }
            if (tot != 0) {
                // Normalization of transition
                for (int second = 0; second < NUMBER_LETTER; second++) {
                    transition_probability[first][second] /= (double) tot;
                }
            }
            tot_start += start_probability[first];
        }

        // Normalization of start
        for (int first = 0; first < NUMBER_LETTER; first++) {
            start_probability[first] /= tot_start;
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