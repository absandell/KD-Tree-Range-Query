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
	int splitValue;
	bool isLeaf;
	bool isRoot;
	vector<vector<int>> dimensions;
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
	int indexBlock;
	KDNode* createKDNode(vector<int> datapoint);
	KDNode* insertKDNode(KDNode* rootNode, vector<int> dataPoint, int indexBlock);
	KDNode* insertKDNodeRecursively(KDNode* rootNode, vector<int> dataPoint, unsigned depth, int indexBlock);
	vector<vector<int>> KDRangeQuery(KDNode* rootNode, vector<int> queries);
	void KDRangeQueryRecursive(KDNode* rootNode, vector<int> queries, unsigned depth, vector<vector<int>>& queryResults);

	//bool searchKDNode(KDNode* rootNode, vector<int> searchPoint);
	//bool searchKDNodeRecursively(KDNode* rootNode, vector<int> searchPoint, unsigned depth);

};

class MyKDTree {
public:

};

bool foundPoint(vector<int> nodePoint, vector<int> searchPoint);
bool compareResult(vector<int> i1, vector<int> i2);
bool compareFirstDim(vector<int> i1, vector<int> i2);
int getIndex(vector<vector<int> > myVec, int min);
int getMedian(vector<vector<int>> dimensions, unsigned depth);
void printResult(vector<vector<int>> results);
