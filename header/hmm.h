/* 
 * File:   hmm.h
 * Author: jeanselv
 *
 * Created on 26 mai 2016, 09:21
 */

#ifndef HMM_H
#define	HMM_H

#include "dataset.h"
#include "forecast.h"
#include <string>
#include <stdlib.h> 
#include <algorithm>
#include <vector> 
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
using namespace std;

#define printColumns(size, content) cout << setfill(' ') << setw(size) << setprecision(4) << content;

class HMM {
    friend class boost::serialization::access;
    // The array of probabilities from the char x to y in a language
    double transition_probability[NUMBER_LETTER][NUMBER_LETTER];
    // The probability that a word begins with a given letter
    double start_probability[NUMBER_LETTER];
    // The probability of the observation given the hidden letter
    double observation_probability[NUMBER_LETTER][NUMBER_LETTER];
    
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar  & transition_probability;
        ar  & start_probability;
        ar  & observation_probability;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & transition_probability;
        ar  & start_probability;
        ar  & observation_probability;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
public: 
    HMM() {
        memset(transition_probability, 0, NUMBER_LETTER*NUMBER_LETTER*sizeof(double));
        memset(start_probability, 0, NUMBER_LETTER*sizeof(double));
        memset(observation_probability, 0, NUMBER_LETTER*NUMBER_LETTER*sizeof(double));
    };
    
    HMM(string dataset, string dictionary, Forecast& forecast) {
        memset(transition_probability, 0, NUMBER_LETTER*NUMBER_LETTER*sizeof(double));
        memset(start_probability, 0, NUMBER_LETTER*sizeof(double));
        memset(observation_probability, 0, NUMBER_LETTER*NUMBER_LETTER*sizeof(double));
        cout << "Learn on dictionary" << endl;
        learn_transition(dictionary);
        cout << "Learn on dataset" << endl;
        learn_observation(dataset, forecast);
    };
    
    vector<char> viterbi(vector<char> observed_word);
    
    void print();
    
private:
    void learn_observation(string dataset, Forecast& forecast);
    
    void learn_transition(string dictionary);
    
    void upperLower(char letter);

    void last(char letter);
};

BOOST_CLASS_VERSION(HMM, 1)

#endif	/* HMM_H */

