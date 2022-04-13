#include "rangequery.h"
using namespace std;

KDNode* KDTree::createKDNode(vector<int> dimensions) {
	KDNode* newNode = new KDNode();
	for (int i = 0; i < dimensions.size(); i++) newNode->dimensions.push_back(dimensions[i]);
	newNode->left = nullptr;
	newNode->right = nullptr;
	return newNode;
}

KDNode* KDTree::insertKDNode(KDNode* rootNode, vector<int> dimensions) {
	return(insertKDNodeRecursively(rootNode, dimensions, 0));
}


KDNode* KDTree::insertKDNodeRecursively(KDNode* rootNode, vector<int> dimensions, unsigned depth) {
	if (!rootNode) return createKDNode(dimensions);

	unsigned currentDepth = depth % dimensions.size();

	if (dimensions[currentDepth] < rootNode->dimensions[currentDepth]);

}


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

Query Format: 
	xmin xmax ymin ymax ... nmin nmax

Sample Queries:
	20 28 54 62
	1002 1018 567 573
	214 246 611 643
	1 65 1000 1064


*/

//int rangeQuery(int argc, char* argv[]) {
int rangeQuery(int searchOption, string databaseFile, string queryFile) {
	/*int searchOption = atoi(argv[1]);
	string databaseFile = string(argv[2]);
	string queryFile = string(argv[3]);*/


	/*
		Array of data points. Each entry in array will contain record

		1.) Read data into an array / vector
		2.) Free to sort data by any of the dimensions if wanted (not required)
		3.) Search sequentially

	*/
	if (searchOption == 0) { // Sequential Search

		cout << "You have chosen sequential search" << endl;

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

			for (int i = 0; i < dataPoints.size(); i++) {
				cout << "Entry " << i << ": " << endl;
				cout << dataPoints[i][0] << "," << dataPoints[i][1] << endl;
			}
		}
		db.close();
	}

	/*
		Tree-based options. Each block at leaf level will contain indexBlock number of key->pointer pairs
	*/
	else if (searchOption == 1) { // kd-tree
		cout << "You have chosen search using kd-tree" << endl;
	}

	else if (searchOption == 2) { // MYkd-tree
		cout << "You have chose search using MYkd-tree" << endl;
	}

	else cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main() {

	int temp = rangeQuery(0, "projDB", "queryFile");


	return 0;
}