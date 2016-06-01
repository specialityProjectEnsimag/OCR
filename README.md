# Authors

- Jeanselme Vincent
- Clin Matthieu


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

## Overlapping segmentation
The overlapping segmentation algorithm comes from a publication **Separation of overlapping character 
N. Papamarkos and T. Koutalianos Department of Electrical and Computer Engineering Democritus University
 of Thrace, 67100 Xanthi, Greece.**. This algorithm is usefull when two characters are overlapping. Overlapping
 here means that the second character starts before the first one is finis (looking from a vertical line point of
 view). 

###The pros :

- As far as there is one pixel between two characters, it can separate them
- Keeps accents on letter (and dot on 'i' and 'j' for example)

###The cons :

- Characters have to be 'full' (no missing pixels) overwise the algorithm will separate them
- It cannot separated two merged characters.

## Oversegmentation
The oversegmentation algorithm is a dynamic algorithm. We first try to determinate where the cuts
are more likely to be usefull and then we try all possibility. This algorithm is tied with the
recognition part as it needs it to weight the cuts. That's why the results of this algorithm is really
dependent of the quality of the recognition algorithm and make it tricky to use it.

### Vocabulary
There are three kind of segmentation :

- Undersegmentation : The segmentation performed can still group more than one character in a box
- Segmentation 		: The segmentation is always correct. One cut means one character
- Oversegmentation  : One character can be cut several times


###The pros :

- Can segment touching character
- As seen in the litterature, gives really good results when the recognition part is handled by a neural network
###The cons :

- Due to oversegmentation, it can easily find two words where there was only one ('m'-> 'r' + 'n')
- The cost can become rapidly huge if there is a lot of possible cuts
- Some heuristics have to be made and we haven't been able to found publications on it for now as
	there all using a neural network to learn the best way to cut..



# DataSet
The data set used in this project can be found on 
[The Chars74K dataset page](http://www.ee.surrey.ac.uk/CVSSP/demos/chars74k/#download). 
However we add some special characters as '.', ',', '\'', '\\', '/' ...

# Libraries
- [Cimg](http://cimg.eu/)
- [Boost](http://www.boost.org/)