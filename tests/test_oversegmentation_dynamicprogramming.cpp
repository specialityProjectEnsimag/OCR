/**
 * 
 */

#include "base_test_segmentation.h"
#include <iostream>
#include <vector>
#include "image_io.h"
#include "preprocessing.h"
#include "clustering_data.h"
#include "distance.h"
#include "CImg.h"

#include "projection.h"
#include "text_line.h"
#include "sliding_window.h"
#include "oversegmentation.h"
#include "overlapping.h"

using namespace overlapping;
using namespace cimg_library;
using namespace std;

void guess(CImg<> img, Forecast& f){
	CImg<> image(img);
	preprocessing::preprocessing(image);
	image.resize(SQUARE, SQUARE);
	std::vector<forecast_type>  res;
	f.forecast(image, res, MSE);
	debug(DEBUG_FEW, res[0].character << " " << res[0].probability << endl); 
}

void displayCutAtPost(CImg<> img, vector<int> cuts){
	CImg<> image(img);
	for(unsigned int i = 0; i < cuts.size(); i++){
		cimg_forY(image, y){
			image(cuts[i], y) = 100;
		}
	}
	image_io::displayImage(image, "Cuts chosen by likelyCut");
}

void displayVerticalHistogram(CImg<> img){
	CImg<> image(img);
	vector<int> histV = projection::verticalHistogram(img);
	histV = projection::secondDifference(histV);
	cimg_forX(image, x){
		int colour;
		int val;
		if(histV[x] < 0){
			val = -histV[x];
			colour = 100;
		}else{
			val = histV[x];
			colour = 200;
		}

		for(int y = 0; y < val; y++){
			image(x, y) = colour;
		}
	}
	display(image.resize_tripleXY());
}

#define WORKING_DIRECTORY "./result-normal"

int main(int argc, char** argv) {
    
	debug(DEBUG_FEW, "This test assumes that kmeans has been alread performed and that" << endl);
	debug(DEBUG_FEW, "The working directory for Forecast is: " WORKING_DIRECTORY << endl);

	#if DEBUG_LEVEL == DEBUG_NONE
		cerr << "Debug is disable, you won't be able to see anything" << endl;
		exit(1);
	#endif

	#if KNN
		debug(DEBUG_FEW, "KNN has been activated, probabilities displayed can be 'huge'" << endl);
	#endif


    if(argc < 2){
    	cerr << "Not enough arguments" << endl;
    	cerr << "This is an interative tests" << endl;
    	cerr << "Usage ./" __FILE__ " image_with_merged_character.png minHeight minSpace" << endl;
    	cerr << "\t image_with_merged_character.png 	The image containing texts with characters";
    	cerr << " that are not perfectly separated" << endl;
    	cerr << "\t minHeight 	if not defined, -1 will be used as it is the algorithm's default" << endl;
    	cerr << "\t minSpace 	if not defined,  9 will be used " << endl;

    	exit(1);
   }

	int minHeight = -1; 
	if(argc > 2){
		minHeight = atoi(argv[2]);
		debug(DEBUG_FEW, "Custom minHeight: " << minHeight << endl);
	}

	int minSpace = 9; 
   	if(argc > 3){
   		minSpace = atoi(argv[3]);
		debug(DEBUG_FEW, "Custom minSpace: " << minSpace << endl);
   	}
   

    CImg<> text = preprocessing::otsu_binarization(projection::reduce(image_io::import(argv[1])));
    display(text);
    Forecast f(WORKING_DIRECTORY);
    vector< text_line* > lines;
    sliding_window::splitLines(text, lines);
    for(unsigned int li = 0; li < lines.size(); li++){
    	CImg<> img(lines.at(li)->img);

    	img = projection::reduce(img);
    	vector< text_character* > characters;
    	sliding_window::splitCharacters(img, characters);

    	for(unsigned int ci = 0; ci < characters.size(); ci++){
    		//cout << "Part " << ci << endl;
			CImg<> charImg(characters.at(ci)->img);
			charImg = projection::reduce(charImg);

			vector<int> SsecDiff = oversegmentation::likelyCut(charImg, minHeight, minSpace);
			//cout << "number of section " << SsecDiff.size() << endl;
			vector< vector<double> > g = oversegmentation::generateGraph(charImg, f, SsecDiff);
			vector<int> selectedCuts = oversegmentation::findBestMatch(g);


			displayCutAtPost(charImg, oversegmentation::likelyCut(charImg, minHeight, minSpace));
			// going trough the selected cuts
			unsigned int p = 0;

			debug(DEBUG_LOT, "Section on image :" << endl);
			for(unsigned int i = 0; i < SsecDiff.size(); i++){
				debug(DEBUG_LOT, "i " << i << " " << SsecDiff[i] << endl);
			}

			debug(DEBUG_LOT, endl << "Result : " << endl);
			for(unsigned int i = 0; i < selectedCuts.size(); i++){
				debug(DEBUG_LOT, "i " << i << " " << selectedCuts[i] << endl);
			}
			debug(DEBUG_LOT, endl);

			while(p < SsecDiff.size() - 1){
				guess(charImg.get_crop(SsecDiff[p], SsecDiff[selectedCuts[p]]), f);
				CImg<> imgChar= charImg.get_crop(SsecDiff[p], SsecDiff[selectedCuts[p]]);
				display(imgChar);
				p = selectedCuts[p];

			}
			debug(DEBUG_FEW, endl << endl);			
    	}
    }
    return 0;
}