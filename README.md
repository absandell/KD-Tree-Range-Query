# rangeQ
## Contributors
Andrew Sandell: asandell@ufl.edu

Mack Hummel: mackhummel@ufl.edu

## Instructions
To run this project, navigate to the Sandell directory and use the makefile to compile.


Sequential Array Outputs will be in SequentialOutput.txt

KD-Tree Outputs will be in KD-TreeOutput.txt

MyKD-Tree Outputs will be in MyKD-TreeOutput.txt 

## Implementation
The variation of the KD-Tree that we implemented relied on the concept of carefully choosing which dimension to use when dividing a node. Our solution was to determine the dimension of the points with the greatest standard deviation, then divide along that dimension to create the most useful split possible. We accomplish this by proceeding with the normal KD-Tree insertion algorithm, up until we arrive at a block overflow. At this point, we loop over the points contained within that block, once for each dimension, and determine which of the dimensions has the greatest standard deviation. Once this is accomplished, we set this dimension as the split dimension, and proceed with the rest of our insertion algorithm. Finally, before recursively adding our nodes, we check to see if the chosen dimension has a standard deviation of 0. If so, we know that the points in this block are identical, so we split them evenly between two trees. We believe that this strategy will improve performance in datasets with high dimensionality, which have a number of points spread unevenly amongst a single dimension.

## Performance
From our tests, we observed that there is a slight difference in query time with the MyKD-Tree for queries with lower ranges of values. At higher sizes of query ranges, the MyKD-Tree performs equal to, or better than the KD-Tree. This is a promising result and shows that the approach might have potential, especially for datasets with high standard deviations along a single axis, or datasets in which large points in higher-dimensional spaces are clustered together. However, we cannot conclude if the changes implemented in the MyKD-Tree are significantly more efficient than the KD-Tree at scale, or at dimensionality of k >= 3. 
