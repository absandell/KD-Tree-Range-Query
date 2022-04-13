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
	cout << "Creating True Root" << endl;
	KDNode* newNode = new KDNode();
	newNode->dimensions.push_back(datapoint);
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	return newNode;
}

KDNode* KDTree::insertKDNode(KDNode* rootNode, vector<int> datapoint, int indexBlock) {
	return(insertKDNodeRecursively(rootNode, datapoint, 0, indexBlock));
}


KDNode* KDTree::insertKDNodeRecursively(KDNode* rootNode, vector<int> datapoint, unsigned depth, int indexBlock) {
	unsigned currentDepth = depth % datapoint.size();
	cout << "Current Depth: " << currentDepth << endl;


	if (!rootNode) 
		return createKDNode(datapoint);

	if (rootNode->isLeaf && rootNode->dimensions.size() < indexBlock) {
		cout << "Inserting " << datapoint[0] << "," << datapoint[1] << endl;
		rootNode->dimensions.push_back(datapoint); // If we can fit datapoint into node's vector
		return rootNode;
	}

	else if (rootNode->isLeaf && rootNode->dimensions.size() >= indexBlock) { // Vector is now full. Need to create new node and split

		int splitVal = getMean(rootNode->dimensions, currentDepth);
		cout << "Splitting Now. Split value is: " << splitVal << endl;

		rootNode->splitValue = splitVal;
		KDNode * leftNode = new KDNode();
		leftNode->left = nullptr;
		leftNode->right = nullptr;
		leftNode->isLeaf = true;

		KDNode * rightNode = new KDNode();
		rightNode->left = nullptr;
		rightNode->right = nullptr;
		rightNode->isLeaf = true;

		rootNode->dimensions.push_back(datapoint);

		for (int i = 0; i < rootNode->dimensions.size(); i++) {
			if (rootNode->dimensions[i][currentDepth] < rootNode->splitValue) {
				leftNode->dimensions.push_back(rootNode->dimensions[i]);
				cout << "Inserting " << rootNode->dimensions[i][0] << "," << rootNode->dimensions[i][1] << " into Left Node" << endl;
			}
			else {
				rightNode->dimensions.push_back(rootNode->dimensions[i]);
				cout << "Inserting " << rootNode->dimensions[i][0] << "," << rootNode->dimensions[i][1] << " into Right Node" << endl;
			}
		}

		rootNode->dimensions.clear();
		rootNode->isLeaf = false;
		rootNode->left = leftNode;
		rootNode->right = rightNode;
		return rootNode;
	}

	else if (!rootNode->isLeaf && datapoint[currentDepth] < rootNode->splitValue)
		rootNode->left = insertKDNodeRecursively(rootNode->left, datapoint, depth + 1, indexBlock);
	
	else if (!rootNode->isLeaf && datapoint[currentDepth] >= rootNode->splitValue)
		rootNode->right = insertKDNodeRecursively(rootNode->right, datapoint, depth + 1, indexBlock);

	return rootNode;
}

/*bool KDTree::searchKDNode(KDNode* rootNode, vector<int> searchPoint) {
	return(searchKDNodeRecursively(rootNode, searchPoint, 0));
}

bool KDTree::searchKDNodeRecursively(KDNode* rootNode, vector<int> searchPoint, unsigned depth) {
	if (!rootNode) return false;
	if (foundPoint(rootNode->dimensions, searchPoint)) return true;
	unsigned currentDepth = depth % rootNode->dimensions.size();
	if (searchPoint[currentDepth] < rootNode->dimensions[currentDepth]) return searchKDNodeRecursively(rootNode->left, searchPoint, depth + 1);
	return searchKDNodeRecursively(rootNode->right, searchPoint, depth + 1);
}*/

/*
Input Command: ./rangeQ searchOption database queries (indexBlock)
	* searchOption:
		0: Sequential Search
		1: kd-tree Search
		2: MYkd-tree Search
	* database:
		Name of database file
	* queries:
		Name of query file
	* indexBlock:
		Number of key-pointer pairs per block
		Only needed if searchOption == 1 || searchOption == 2

*/

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

int getMean(vector<vector<int>> dimensions, unsigned depth) {
	float runningSum = 0;
	for (int i = 0; i < dimensions.size(); i++) {
		runningSum += dimensions[i][depth];
	}
	return ceil(runningSum / dimensions.size());
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

	std::sort(dataPoints.begin(), dataPoints.end(), compareFirstDim);

	if (searchOption == 0) { // Sequential Search
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
				cout << "Index: " << index << endl;
				if (index >= 0) {
					int value = dataPoints[index][0];

					//For loop taking advantage of sort (only looping through 1st dim passes)
					cout << "Points: " << endl;
					for (int i = index; value <= maxVal; i++) {
						vector<bool> pass;
						for (int j = 2; j < query.size(); j += 2) {
							if (query[j] <= dataPoints[i][j / 2]) {
								if (query[j + 1] >= dataPoints[i][j / 2]) pass.push_back(true);
								else pass.push_back(false);
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

	/*
		Tree-based options. Each block at leaf level will contain indexBlock number of key->pointer pairs
	*/
	else if (searchOption == 1) { // kd-tree
		cout << "You have chosen search using kd-tree" << endl;

		KDNode* rootNode = nullptr;
		KDTree tree;
		tree.indexBlock = indexBlock;


		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertKDNode(rootNode, dataPoints.at(i), indexBlock);
			//cout << "Entry " << i << ": " << endl;
			//cout << dataPoints[i][0] << "," << dataPoints[i][1] << endl;
		}

		vector<int> tempSearch1;
		tempSearch1.push_back(75);
		tempSearch1.push_back(347);

		vector<int> tempSearch2 = { -10, 515 };

		//cout << tree.searchKDNode(rootNode, tempSearch1) << endl;
		//cout << tree.searchKDNode(rootNode, tempSearch2) << endl;
	}

	else if (searchOption == 2) { // MYkd-tree
		cout << "You have chose search using MYkd-tree" << endl;
	}

	else cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main() {
	int temp0 = rangeQuery(0, "2dtestDB", "projquery", 0);
	//int temp1 = rangeQuery(1, "2dtestDB", "queryFile", 3);

	return 0;
}