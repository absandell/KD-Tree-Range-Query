#include "rangequery.h"
using namespace std;

bool foundPoint(vector<int> nodePoint, vector<int> searchPoint) {
	for (int i = 0; i < nodePoint.size(); i++) {
		cout << nodePoint.at(i) << " - " << nodePoint.at(i) << endl;
		if (nodePoint.at(i) != searchPoint.at(i)) return false;
	}
	return true;
}

KDNode* KDTree::createKDNode(vector<int> datapoint) {
	KDNode* newNode = new KDNode();
	newNode->dimensions.push_back(datapoint);
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	newNode->isRoot = true;
	return newNode;
}

KDNode* KDTree::insertKDNode(KDNode* rootNode, vector<int> datapoint, int indexBlock) {
	return(insertKDNodeRecursively(rootNode, datapoint, 0, indexBlock));
}


KDNode* KDTree::insertKDNodeRecursively(KDNode* rootNode, vector<int> datapoint, unsigned depth, int indexBlock) {
	unsigned currentDepth = depth % datapoint.size();

	if (!rootNode) 
		return createKDNode(datapoint);

	if (rootNode->isLeaf && rootNode->dimensions.size() < indexBlock) {
		cout << "Inserting " << datapoint[0] << "," << datapoint[1] << endl;
		rootNode->dimensions.push_back(datapoint); // If we can fit datapoint into node's vector
		return rootNode;
	}

	else if (rootNode->isLeaf && rootNode->dimensions.size() >= indexBlock) { // Vector is now full. Need to create new node and split
		rootNode->dimensions.push_back(datapoint);
		int splitVal = getMedian(rootNode->dimensions, currentDepth);
		cout << "Splitting Now. Split value is: " << splitVal << endl;

		rootNode->splitValue = splitVal;
		KDNode * leftNode = new KDNode();
		leftNode->left = nullptr;
		leftNode->right = nullptr;
		leftNode->isLeaf = true;
		leftNode->isRoot = false;

		KDNode * rightNode = new KDNode();
		rightNode->left = nullptr;
		rightNode->right = nullptr;
		rightNode->isLeaf = true;
		rightNode->isRoot = false;

		vector<vector<int>> dimensionsCopy = rootNode->dimensions;

		rootNode->dimensions.clear();
		rootNode->isLeaf = false;
		rootNode->left = leftNode;
		rootNode->right = rightNode;

		for (int i = 0; i < dimensionsCopy.size(); i++) {
			cout << "i: " << i << endl;
			cout << "Trying to insert: " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << endl;
			if (dimensionsCopy[i][currentDepth] < rootNode->splitValue) {
				//leftNode->dimensions.push_back(rootNode->dimensions[i]);
				insertKDNodeRecursively(leftNode, dimensionsCopy[i], depth + 1, indexBlock);
				cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Left Node" << endl;
			}
			else if (dimensionsCopy[i][currentDepth] >= rootNode->splitValue) {
				//rightNode->dimensions.push_back(rootNode->dimensions[i]);
				insertKDNodeRecursively(rightNode, dimensionsCopy[i], depth + 1, indexBlock);
				cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Right Node" << endl;
			}

		}

		return rootNode;
	}

	else if (!rootNode->isLeaf && datapoint[currentDepth] < rootNode->splitValue)
		rootNode->left = insertKDNodeRecursively(rootNode->left, datapoint, depth + 1, indexBlock);
	
	else if (!rootNode->isLeaf && datapoint[currentDepth] >= rootNode->splitValue)
		rootNode->right = insertKDNodeRecursively(rootNode->right, datapoint, depth + 1, indexBlock);

	return rootNode;
}

vector<vector<int>> KDTree::KDRangeQuery(KDNode* rootNode, vector<int> queries) {
	vector<vector<int>> results;
	KDRangeQueryRecursive(rootNode, queries, 0, results);
	return results;
}

void KDTree::KDRangeQueryRecursive(KDNode* rootNode, vector<int> queries, unsigned depth, vector<vector<int>>& queryResults) {
	unsigned currentDepth = depth % (queries.size()/2);
	cout << "Current Depth: " << currentDepth << endl;
	
	if (!rootNode->isLeaf && queries[currentDepth*2] < rootNode->splitValue) {
		cout << "Query Value: " << queries[currentDepth * 2] << " Split Value: " << rootNode->splitValue << endl;
		KDRangeQueryRecursive(rootNode->left, queries, depth + 1, queryResults);
	}
	if (!rootNode->isLeaf && queries[currentDepth * 2 + 1] >= rootNode->splitValue) {
		cout << "Query Value: " << queries[currentDepth * 2 + 1] << " Split Value: " << rootNode->splitValue << endl;
		KDRangeQueryRecursive(rootNode->right, queries, depth + 1, queryResults);
	}



	if (rootNode->isLeaf && !rootNode->isRoot) {
		for (int i = 0; i < rootNode->dimensions.size(); i++) {
			bool isValid = false;

			for (int j = 0; j < queries.size(); j+=2) {
				if (rootNode->dimensions[i][j / 2] >= queries[j] && rootNode->dimensions[i][j / 2] <= queries[j + 1]) {
					isValid = true;
				}
				else {
					isValid = false;
					break;
				}
			}

			if (isValid) {
				queryResults.push_back(rootNode->dimensions[i]);
				cout << "Pushing Back: " << rootNode->dimensions[i][0] << "," << rootNode->dimensions[i][1] << endl;
			}
		}
		return;
	}
	if (rootNode == nullptr) return;
}

bool compareResult(vector<int> i1, vector<int> i2)
{
	for (int i = 0; i < i1.size(); i++)
	{
		if (!(i1[i] == i2[i]))
		{
			return (i1[i] < i2[i]);
		}
		if ((i + 1) >= i1.size())
		{
			return (i1[i] < i2[i]);
		}
	}
	return (i1[0] < i2[0]);
}

bool compareFirstDim(vector<int> i1, vector<int> i2)
{
	return (i1[0] < i2[0]);
}

int getIndex(vector<vector<int>> myVec, int min) {
	for (int i = 0; i < myVec.size(); i++) if (myVec[i][0] >= min) return i;
	return -1;
}

int getMedian(vector<vector<int>> dimensions, unsigned depth) {
	if (dimensions.size() == 1) {
		return dimensions[0][depth];
	}
	if (dimensions.size() == 2) {
		float sum = dimensions[0][depth] + dimensions[1][depth];
		return ceil(sum / 2);
	}
	vector<int> dimensionalVector;
	for (int i = 0; i < dimensions.size(); i++) dimensionalVector.push_back(dimensions[i][depth]);
	std::sort(dimensionalVector.begin(), dimensionalVector.end());
	float middle = dimensionalVector.size() / 2;
	cout << "Median is: " << dimensionalVector[ceil(middle)] << endl;
	return dimensionalVector[ceil(middle)];
}

void printResult(vector<vector<int>> results) {
	cout << "***** All Results: *******" << endl;
	// Sort Result Vector on all dimmensions
	std::sort(results.begin(), results.end(), compareResult);

	// Print sorted list
	for (int w = 0; w < results.size(); w++)
	{
		for (int k = 0; k < results[w].size(); k++)
		{

			if (k + 1 >= results[w].size())
			{
				cout << results[w][k] << endl;
			}
			else
			{
				cout << results[w][k] << ",";
			}
		}
	}
}


//int rangeQuery(int argc, char* argv[]) {
int rangeQuery(int searchOption, string databaseFile, string queryFile, int indexBlock) {
	/*int searchOption = atoi(argv[1]);
	string databaseFile = string(argv[2]);
	string queryFile = string(argv[3]);*/

	/*
		Array of data points. Each entry in array will contain record

		1.) Read data into an array / vector
		2.) Free to sort data by any of the dimensions if wanted (not required)
		3.) Search sequentially

	*/
	vector<vector<int>> dataPoints;
	fstream db;
	db.open(databaseFile, ios::in);
	if (db.is_open()) {
		cout << "database opened" << endl;
		string temp;
		while (getline(db, temp)) {
			vector<int> entry;
			stringstream line(temp);

			while (line.good()) {
				string substring;
				getline(line, substring, ',');
				entry.push_back(stoi(substring));
			}

			dataPoints.push_back(entry);
		}
	}
	db.close();

	

	if (searchOption == 0) { // Sequential Search
		std::sort(dataPoints.begin(), dataPoints.end(), compareFirstDim);
		cout << "You have chosen sequential search" << endl;

		vector<vector<int>> results;
		fstream qu;
		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			cout << "query file opened" << endl;
			string temp;
			while (getline(qu, temp)) {
				vector<int> query;
				stringstream line(temp);
				int queryCount = 0;
				while (line.good()) {
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}
				//Query Info
				cout << "***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i	/ 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}

				//Sort based info
				int minVal = query[0];
				int maxVal = query[1];
				int index = getIndex(dataPoints, minVal);
				//cout << "Index: " << index << endl;
				if (index >= 0) {
					int value = dataPoints[index][0];

					//For loop taking advantage of sort (only looping through 1st dim passes)
					//cout << "Points: " << endl;
					for (int i = index; value <= maxVal; i++) {
						vector<bool> pass;
						for (int j = 2; j < query.size(); j += 2) {
							if (query[j] <= dataPoints[i][j / 2]) {
								if (query[j + 1] >= dataPoints[i][j / 2]) 
									pass.push_back(true);
								else 
									pass.push_back(false);
							}
							else pass.push_back(false);
						}

						//If it passes 2->k dimmension range query
						if (find(pass.begin(), pass.end(), false) == pass.end()) {
							results.push_back(dataPoints[i]);
						}
						//At end of sorted DB
						if (i + 1 >= dataPoints.size()) break;

						//Set value to next index for conditional 
						value = dataPoints[i + 1][0];
					}
				}
			}
		}
		qu.close();

		printResult(results);
	}

	else if (searchOption == 1) { // kd-tree
		cout << "You have chosen search using kd-tree" << endl;

		KDNode* rootNode = nullptr;
		KDTree tree;
		tree.indexBlock = indexBlock;


		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertKDNode(rootNode, dataPoints.at(i), indexBlock);
		}

		vector<vector<int>> results;
		fstream qu;
		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			cout << "query file opened" << endl;
			string temp;
			while (getline(qu, temp)) {
				vector<int> query; // each 2 
				stringstream line(temp);

				while (line.good()) {
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}
				//Query Info
				cout << "***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}
				vector<vector<int>> tempResults;
				tempResults = tree.KDRangeQuery(rootNode, query);
				results.insert(end(results), begin(tempResults), end(tempResults));
			}
		}
		qu.close();

		printResult(results);
	}

	else if (searchOption == 2) { // MYkd-tree
		cout << "You have chose search using MYkd-tree" << endl;
	}

	else cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main() {
	//int temp0 = rangeQuery(0, "3dtestDB", "projquery", 0);
	int temp1 = rangeQuery(1, "2dtestDB", "2dprojquery", 1);
	int temp2 = rangeQuery(1, "3dtestDB", "3dprojquery", 1);

	return 0;
}