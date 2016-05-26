/* 
 * File:   newsimpletest.cpp
 * Author: jeanselv
 *
 * Created on 17 mai 2016, 16:06:41
 */

#include <stdlib.h>
#include <iostream>
#include <clustering_data.h>

/*
 * Simple C++ Test Suite
 */

void directoryDoesNotExist() {
    assert(!kmeans("NotExist","NotExistToo"));
}

void targetWithSameName() {
    assert(kmeans("dataset", "dataset"));
}

void realTest() {
    assert(kmeans("dataset", "result_dataset"));
}

int main() {
    directoryDoesNotExist();
    targetWithSameName();
    realTest();

    return (EXIT_SUCCESS);
}

