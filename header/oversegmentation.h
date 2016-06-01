/* 
 * File:   Projection.h
 * Author: clinm
 *
 * Created on 30 mai 2016, 08:26
 */
#include "CImg.h"
#include <iostream>
#include <vector>
#include "projection.h"
#include "forecast.h"
#include "image_io.h"
#include <iomanip>

using namespace cimg_library;
using namespace std;

#ifndef OVERSAGMENTATION
#define OVERSAGMENTATION 

namespace oversegmentation {

	/**
	 * Given an image, choose the likely cuts that could separate the
	 * word into characters.
	 * @param img 		Image representing a word
	 * @param minHeight Minimum value for the histogram to be taken in consideration
	 * 					Default is '-1' then the value will be computed with an heuristic
	 * @param minSpace  Minimum space between two cuts (in pixel)
	 * @return 			Vector of chosen cuts
	 */
	vector<int> likelyCut(const CImg<>& img, int minHeight = -1, unsigned int minSpace = 10);

	/**
	 * Constructs the graph associated with the cuts
	 * 
	 * @param img 	Image
	 * @param f 	Forecast object use to compute the edge's weight
	 * @param cuts 	The X position where cuts can be performed (result of 'likeCut') 	
	 * @return 		A matrix representing the graph where at (x, y) we find the probability to
	 * 				go from x->y (index in cuts) 
	 */
	vector< vector<double> > generateGraph(const CImg<>& img, Forecast& f, vector<int> cuts);

	/**
	 * Find the best match in the graph. For now it is trying to maximise the sum of the 
	 * probabilities but it is not efficiente enough for now
	 * 
	 * @param graph Graph generated with 'generateGraph'
	 * @return		A solution vector, the index is the beginning of the cut (x value can be
	 * 				found in the cuts vector from generateGraph) and the value in the end.
	 * 				Meaning: if we have (0 -> 5) ... (5 -> 8) the first cut is [cuts[0], cuts[5]] 
	 */
	vector<int> findBestMatch(vector< vector<double> >& graph);

}

#endif