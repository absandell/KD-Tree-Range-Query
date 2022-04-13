#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#pragma once

struct KDNode {
	vector<int> dimensions;
	KDNode* left;
	KDNode* right;
};

class KDTree {
public:
	KDNode* createKDNode(vector<int> dimensions);
	KDNode* insertKDNode(KDNode* rootNode, vector<int> dataPoint);
	KDNode* insertKDNodeRecursively(KDNode* rootNode, vector<int> dataPoint, unsigned depth);

};
