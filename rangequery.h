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
	double splitValue;
	bool isLeaf;
	bool isRoot;
	vector<vector<int>> dimensions;
	KDNode* left;
	KDNode* right;
};

struct MyKDNode {
	double splitValueLeft;
	double splitValueMid;
	double splitValueRight;
	bool isLeaf;
	bool isRoot;
	vector<vector<int>> dimensions;
	MyKDNode* left;
	MyKDNode* midleft;
	MyKDNode* midright;
	MyKDNode* right;
};

class KDTree {
public:
	KDNode* createKDNode(vector<int> datapoint);
	KDNode* createKDLeaf();
	KDNode* insertKDNode(KDNode* rootNode, vector<int> dataPoint, int indexBlock);
	KDNode* insertKDNodeRecursively(KDNode* rootNode, vector<int> dataPoint, unsigned depth, int indexBlock);
	vector<vector<int>> KDRangeQuery(KDNode* rootNode, vector<int> queries);
	void KDRangeQueryRecursive(KDNode* rootNode, vector<int> queries, unsigned depth, vector<vector<int>>& queryResults);
};

class MyKDTree {
public:
	MyKDNode* createMyKDNode(vector<int> datapoint);
	MyKDNode* createMyKDLeaf();
	MyKDNode* insertMyKDNode(MyKDNode* rootNode, vector<int> dataPoint, int indexBlock);
	MyKDNode* insertMyKDNodeRecursively(MyKDNode* rootNode, vector<int> dataPoint, unsigned depth, int indexBlock);
	vector<vector<int>> MyKDRangeQuery(MyKDNode* rootNode, vector<int> queries);
	void MyKDRangeQueryRecursive(MyKDNode* rootNode, vector<int> queries, unsigned depth, vector<vector<int>>& queryResults);

};

bool foundPoint(vector<int> nodePoint, vector<int> searchPoint);
bool compareResult(vector<int> i1, vector<int> i2);
bool compareFirstDim(vector<int> i1, vector<int> i2);
int getIndex(vector<vector<int> > myVec, int min);
double getMedian(vector<vector<int>> dimensions, unsigned depth);
void printResult(vector<vector<int>> results);
vector<double> getSplitVals(vector<vector<int>> dimensions, unsigned depth);
