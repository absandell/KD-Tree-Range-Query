#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
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

struct MyKDNode {
	vector<int> dimensions;
	MyKDNode* left;
	MyKDNode* right;
};

class KDTree {
public:
	KDNode* createKDNode(vector<int> dimensions);
	KDNode* insertKDNode(KDNode* rootNode, vector<int> dataPoint);
	KDNode* insertKDNodeRecursively(KDNode* rootNode, vector<int> dataPoint, unsigned depth);
	bool searchKDNode(KDNode* rootNode, vector<int> searchPoint);
	bool searchKDNodeRecursively(KDNode* rootNode, vector<int> searchPoint, unsigned depth);

};

class MyKDTree {
public:

};

bool foundPoint(vector<int> nodePoint, vector<int> searchPoint);
bool compareVector(vector<int> i1, vector<int> i2);
int getIndex(vector<vector<int> > myVec, int min);