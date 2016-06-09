# Authors

- Jeanselme Vincent
- Clin Matthieu


# Project
This project has been realized from the 24th May to the 10th June 2016. 
We want to recognize text from scan. However, we do not use Neural Network
in order to understand every issue and complexity of the problem.



# Pre Processing
This section presents the different techniques used in order to clean the image and 
correct its rotation.

## Filters
Several filters exist, however few have good results on test images where characters
are considered as bruit. It is why the bilateral and the median filters that are implemented 
are not used in the current version due to poor results.
Linears filters have more interesting results. The software uses a linear filter with a mask
which corresponds to a gaussian correction. This way, it reduces the noise.

## Binarization
A global [Otsu binarization](https://en.wikipedia.org/wiki/Otsu%27s_method) is used in order to delete all the softened noise.
However this method could be more accurate by a local binarization.

## Rotation
First, the projection in the [Hough space](https://en.wikipedia.org/wiki/Hough_transform) is computed. Thus, we implemented
an heuristic in order to find the rotation needed in order to have the highest number
of straight lines parallel to the bottom of the page.

## Further work
- Enhance the text by increasing the contrast.
- Find other filters in order to delete all remaining noise.
- Cut the contours of the text.
- Correct other distortions.




# Segmentation algorithms
This section will present various segmentation algorithms implemented in this project.
Algorithms are presented in their order of implementation (which also means complexity in
this case).

## Sliding window
The sliding window algorithm is the easiest way to separate lines and characters but it is
also the most limited one. The principle is to look for a row (or column) where all pixels
are white (meaning that there is a space between two lines of text or two characters).

###The pros : 

- Easy implementation
- Able to spot paragraphs and words (bigger space)

###The cons : 

- Noise will be considered as line
- Low DPI that leads to missing pixels (cutting a character in two) will be considered as two distinct characters.
- A noise pixel which links two characters will not be cut by this method

## Overlapping segmentation
The overlapping segmentation algorithm comes from a publication **Separation of overlapping character 
N. Papamarkos and T. Koutalianos Department of Electrical and Computer Engineering Democritus University
 of Thrace, 67100 Xanthi, Greece.**. This algorithm is useful when two characters are overlapping. Overlapping
 here means that the second character starts before the first one is finished (looking from a vertical line point of
 view). 

###The pros :

- As far as there is one pixel between two characters, it can separate them
- Keeps accents on letter (and dot on 'i' and 'j' for example)

###The cons :

- Characters have to be 'full' (no missing pixels) overwise the algorithm will separate them
- It cannot separated two merged characters.

## Oversegmentation
The oversegmentation algorithm is a dynamic algorithm. We first try to determinate where the cuts
are more likely to be useful and then we try all possibilities. This algorithm is tied with the
recognition part as it needs it to weight the cuts. That's why the results of this algorithm is really
dependent of the quality of the recognition algorithm and make it tricky to use it.

### Vocabulary
There are three kinds of segmentation :

- Undersegmentation : The segmentation performed can still group more than one character in a box
- Segmentation 		: The segmentation is always correct. One cut means one character
- Oversegmentation  : One character can be cut several times


###The pros :

- Can segment touching characters
- As seen in the litterature, gives really good results when the recognition part is handled by a neural network
###The cons :

- Due to oversegmentation, it can easily find two words where there was only one ('m'-> 'r' + 'n')
- The cost can become rapidly huge if there is a lot of possible cuts
- Some heuristics have to be made and we haven't been able to found publications on it for now as
	there all using a neural network to learn the best way to cut..




# Character Recognition
This section presents the recognition process.

## Clustering - KMeans
Due to the large number of images of each category, it is necessary to compute K clusters for each label.
For each letter, the algorithm gathers closest images in an average image. For computing the different clusters,
the [KMeans](https://en.wikipedia.org/wiki/K-means_clustering) algorithm is used.

## Recognition - KNN
After computing the clusters, we compare the image obtained by segmentation to the different clusters. The K identic closest 
pictures define the label of the character. The MSE distance is used, even if Chamfer distance has been tried with and without 
skeletonization. 

## Further work
- Neural Network is the solution for recognition




# Post Processing
Some imperfections remain after the recognition, it is why it is necessary to correct the obtained text.

## HMM
In order to correct incoherences in the text, one needs to know the language of the text, it is why this software could be used only
on english text. The hidden states are the real letters and the observed states are the result of the recogntion. The [Viterbi](https://en.wikipedia.org/wiki/Viterbi_algorithm) algorithm computes the most probable word respecting the transition between letters in english.

## Levenshtein
Even after the HMM, there is some flaws. The algorithm computes the [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) 
between the result and the dictionnary and replaces the word by the closest word.
In order to have this correction, add -DLEVENSHTEIN in the Makefile.

## Further work
- Change the Levenshtein distance in order to allow more insertion of letters.
- Taking account of the error of over-segmentation in the HMM by creating new states.


# DataSet
The dataset used in this project can be found on 
[The Chars74K dataset page](http://www.ee.surrey.ac.uk/CVSSP/demos/chars74k/#download). 
However we add some special characters as '.', ',', '\'', '\\', '/' ...
In order to insert new characters, you have to create a new directory in which you include new images,
after, you have to change the bijections between the directory's name and the character label.


# Libraries
- [Cimg](http://cimg.eu/)
- [Boost](http://www.boost.org/)
- [OpenMP](http://openmp.org/wp/)


# Compilation options
- DSKELETONIZATION	: Computes the skeltonization on each charcters.
- DLEVENSHTEIN		: Computes at the end of the HMM analysis the most probable word.
- DCHAMFER			: Computes the distance of Chamfer on each characters.
- DKNN				: Computes the KNN algorithm for recognition.


# How to install and run OCR ?
- Unzip the dataset at the root of the project.
- Change the options of compilation in the Makefile.
- And type : "make" at the root.

WARNING : 
- At the first running, there is a long learning phase, which could go on 3 hours.
- If you change the compilation options, do "make clean; make" and delete the result_dataset and the hmm_save file.