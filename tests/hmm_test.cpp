/* 
 * File:   hmm.cpp
 * Author: jeanselv
 *
 * Created on 27 mai 2016, 09:00:13
 */

#include <stdlib.h>
#include <iostream>
#include "hmm.h"
#include "forecast.h"
#include "clustering_data.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

int main() {
    kmeans("./dataset", "./result");
    Forecast forecast("./result");
    HMM hmm = HMM("./dataset", "./dictionnary.txt", forecast);

    hmm.print();
    {
        cout << "Save" << endl;
        std::ofstream ofs("hmm_save");
        boost::archive::text_oarchive oa(ofs);
        oa << hmm;
    }
    
    HMM hmm2 = HMM();
    {
        cout << "Load" << endl;
        std::ifstream ifs("hmm_save");
        boost::archive::text_iarchive ia(ifs);
        ia >> hmm2;
    }
    hmm2.print();    
    return (EXIT_SUCCESS);
}

