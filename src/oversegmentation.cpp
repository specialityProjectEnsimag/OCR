#include "oversegmentation.h"

namespace oversegmentation {

	// #define  LIKELY_SECONDDIFF
	#ifdef LIKELY_SECONDDIFF

	vector<int> likelyCut(const CImg<>& img, unsigned int minHeight, unsigned int minSpace){
		cout << "secondDifference" << endl;
	    vector<int> histV = projection::verticalHistogram(img);
		histV = projection::secondDifference(histV);

		vector<int> cuts;
		// always take the beginning of the word
		cuts.push_back(0);
		unsigned int lastCut = 0;
		for(unsigned int x = 0; x < histV.size(); x++ ){
			int val = histV[x];
			if(val > minHeight){
				if(x > lastCut + minSpace){
					lastCut = x;
					cuts.push_back(lastCut);
				}else if(val < histV[lastCut] && cuts.size() != 1){
					// override last cuts only if it is not the first one
					lastCut = x;
					cuts[cuts.size() - 1] = lastCut;	
				}
			}
		}

		// remove the last cut if too close of the end of the image
		if(cuts.back() > img._width - minSpace){
			cuts.pop_back();
		}

		// add end of word
		cuts.push_back(img._width - 1);

		return cuts;
	}

	#else
	// seems to make better cuts !
	vector<int> likelyCut(const CImg<>& img, int minHeight, unsigned int minSpace){
	    vector<int> histV = projection::ANDing(img);
	    vector<int> heightV(img._height + 1, 0);
	    int total = 0;

	    //
	    //	If default value for minHeight, try to find the best value
	    // 	with an heuristic. We select the minimum height in order to have
	    //	1/20th of all pixel under the threshold. This ratio was found by
	    //	tests and provide quite good results.
	    //
	    if(minHeight == -1){
		    for(unsigned int x = 0; x < histV.size(); x++){
		    	heightV[histV[x]]++;
		    	total += histV[x];
		    }

		    int subTotal = 0;
		    minHeight = -1;
		    while(subTotal < total / 20){
		    	minHeight++;
		    	subTotal = heightV[minHeight] * minHeight;
		    }
	    	
	    }


		vector<int> cuts;
		// always take the beginning of the word
		cuts.push_back(0);
		unsigned int lastCut = 0;
		for(unsigned int x = 0; x < histV.size(); x++ ){
			int val = histV[x];
			if(val > minHeight){
				if(x > lastCut + minSpace){
					lastCut = x;
					cuts.push_back(lastCut);
				}else if(val < histV[lastCut] && cuts.size() != 1){
					// override last cuts only if it is not the first one
					lastCut = x;
					cuts[cuts.size() - 1] = x;	
				}
			}
		}

		// remove the last cut if too close of the end of the image
		if(cuts.back() > (int) (img._width - minSpace)){
			cuts.pop_back();
		}

		// add end of word
		cuts.push_back(img._width - 1);

		return cuts;
	}
	#endif


	vector< vector<double> > generateGraph(const CImg<>& img, Forecast& f, vector<int> cuts){
    	vector< vector<double> > matrix(cuts.size(), vector<double>(cuts.size(), -1));
    	unsigned int limitWindow = img._height;

    	for(unsigned int start = 0; start < cuts.size()-1; start++){
    		unsigned begX = cuts.at(start);
    		unsigned x = start+1;
    		while(x < cuts.size() && cuts.at(x) - begX < limitWindow){
				std::vector<forecast_type>  res;
				CImg<> elt(img.get_crop(begX, cuts.at(x)));
				preprocessing::preprocessing(elt);
				elt.resize(SQUARE, SQUARE);


				f.forecast(elt, res, MSE);
    			matrix[start][x] = res[0].probability;
    			x++;
    		}

    	}

    	// end of img, probability of 1 (end of graph as well)
    	matrix[cuts.size()-1][cuts.size()-1] = 0;

		return matrix;
	}

	void argMax(int& xChosen, double& xChosenProbability,
				const int& xCandidate, const double& xProb,
				const vector< vector<double> >& graph){

		if(xProb + graph[xCandidate][xCandidate] > xChosenProbability + graph[xChosen][xChosen]){
			xChosen = xCandidate;
			xChosenProbability = xProb;
		}
	}

	vector<int> findBestMatch(vector< vector<double> >& graph){
		vector<int> sol(graph.size() - 1, 0);

		for(int line = graph.size() - 2; line >= 0; line--){
			int indexC = line + 1;
			double probability = graph[line][indexC];
			unsigned int current = line + 2;
			while(current < graph[0].size() && graph[line][current] != -1){
				argMax(indexC, probability, current, graph[line][current], graph);
				current++;
			}
			graph[line][line] = probability;
			sol[line] = indexC;

		}

		#ifdef PRINT_DEBUG

			for(unsigned int i = 0; i < graph.size(); i++){
				for(unsigned int j = 0; j < graph[0].size(); j++){
					cout <<  setfill(' ') << setw(15) << setprecision(8) << (float) graph[i].at(j) << "  ";
				}
				cout << endl;
			}
		#endif

		return sol;
	}

}
