#include "rangequery.h"
using namespace std;

typedef chrono::high_resolution_clock Clock;

KDNode* KDTree::createKDNode(vector<int>& datapoint) {
	KDNode* newNode = new KDNode();
	newNode->dimensions.push_back(datapoint);
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	newNode->isRoot = true;
	return newNode;
}

KDNode* KDTree::createKDLeaf() {
	KDNode* newNode = new KDNode();
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	newNode->isRoot = false;
	return newNode;
}

KDNode* KDTree::insertKDNode(KDNode* rootNode, vector<int>& dataPoint) {
	return(insertKDNodeRecursively(rootNode, dataPoint, 0));
}

KDNode* KDTree::insertKDNodeRecursively(KDNode* rootNode, vector<int>& datapoint, unsigned depth) {
	unsigned currentDepth = depth % datapoint.size();

	if (!rootNode) 
		return createKDNode(datapoint);

	if (rootNode->isLeaf && rootNode->dimensions.size() < indexBlock) {
		//cout << "Inserting " << datapoint[0] << "," << datapoint[1] << endl;
		rootNode->dimensions.push_back(datapoint); // If we can fit datapoint into node's vector
		return rootNode;
	}

	else if (rootNode->isLeaf && rootNode->dimensions.size() >= indexBlock) { // Vector is now full. Need to create new node and split
		rootNode->dimensions.push_back(datapoint);
		double splitVal = getMedian(rootNode->dimensions, currentDepth);

		rootNode->splitValue = splitVal;
		KDNode* leftNode = createKDLeaf();
		KDNode* rightNode = createKDLeaf();

		vector<vector<int> > dimensionsCopy = rootNode->dimensions;

		rootNode->isLeaf = false;
		rootNode->left = leftNode;
		rootNode->right = rightNode;

		for (int i = 0; i < dimensionsCopy.size(); i++) {
			if (dimensionsCopy[i][currentDepth] < rootNode->splitValue) {
				insertKDNodeRecursively(leftNode, dimensionsCopy[i], depth + 1);
			}
			else if (dimensionsCopy[i][currentDepth] >= rootNode->splitValue) {
				insertKDNodeRecursively(rightNode, dimensionsCopy[i], depth + 1);
			}

		}

		return rootNode;
	}

	else if (!rootNode->isLeaf && datapoint[currentDepth] < rootNode->splitValue)
		rootNode->left = insertKDNodeRecursively(rootNode->left, datapoint, depth + 1);
	
	else if (!rootNode->isLeaf && datapoint[currentDepth] >= rootNode->splitValue)
		rootNode->right = insertKDNodeRecursively(rootNode->right, datapoint, depth + 1);

	return rootNode;
}

vector<vector<int> > KDTree::KDRangeQuery(KDNode* rootNode, vector<int>& queries) {
	vector<vector<int> > results;
	KDRangeQueryRecursive(rootNode, queries, 0, results);
	return results;
}

void KDTree::KDRangeQueryRecursive(KDNode* rootNode, vector<int>& queries, unsigned depth, vector<vector<int> >& queryResults) {
	unsigned currentDepth = depth % (queries.size()/2);
	
	if (!rootNode->isLeaf && queries[currentDepth*2] < rootNode->splitValue) {
		KDRangeQueryRecursive(rootNode->left, queries, depth + 1, queryResults);
	}
	if (!rootNode->isLeaf && queries[currentDepth * 2 + 1] >= rootNode->splitValue) {
		KDRangeQueryRecursive(rootNode->right, queries, depth + 1, queryResults);
	}

	if (rootNode->isLeaf) {
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
			}
		}
		return;
	}
	if (rootNode == nullptr) return;
}

void KDTree::deleteKDTree(KDNode* rootNode) {
	if (rootNode) {
		deleteKDTree(rootNode->left);
		deleteKDTree(rootNode->right);
		free(rootNode);
	}
}


/* MyKDTree */
MyKDNode* MyKDTree::createMyKDNode(vector<int>& datapoint) {
	MyKDNode* newNode = new MyKDNode();
	newNode->dimensions.push_back(datapoint);
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	newNode->isRoot = true;
	//Should be chosen
	newNode->dim = 0;
	return newNode;
}

MyKDNode* MyKDTree::createMyKDLeaf() {
	MyKDNode* newNode = new MyKDNode();
	newNode->left = nullptr;
	newNode->right = nullptr;
	newNode->isLeaf = true;
	newNode->isRoot = false;
	return newNode;
}

MyKDNode* MyKDTree::insertMyKDNode(MyKDNode* rootNode, vector<int> &dataPoint) {
	return(insertMyKDNodeRecursively(rootNode, dataPoint));
}

MyKDNode* MyKDTree::insertMyKDNodeRecursively(MyKDNode* rootNode, vector<int> &datapoint){

	if (!rootNode)
		return createMyKDNode(datapoint);

	if (rootNode->isLeaf && rootNode->dimensions.size() < indexBlock) {
		rootNode->dimensions.push_back(datapoint); // If we can fit datapoint into node's vector
		return rootNode;
	}

	else if (rootNode->isLeaf && rootNode->dimensions.size() >= indexBlock) { // Vector is now full. Need to create new node and split
		rootNode->dimensions.push_back(datapoint);
		int bestDim = 0;
		double bestSD = 0;
		for(int i = 0; i < datapoint.size(); i++){
			double sd = standardDev(rootNode->dimensions, i);
			if(bestSD < sd){
				bestSD = sd;
				bestDim = i;
			}
		}

		double splitVal = getMedian(rootNode->dimensions, bestDim);
		bool addOne = true;

		for(int i = 0; i < rootNode->dimensions.size(); i++){
			if(rootNode->dimensions[i][bestDim] < splitVal){ // If we have values going to left branch, prevents infinite overflow
				addOne = false;
			}
		}

		if(addOne){ // If we have no values going in left branch, add one to split value to break up infinite overflow
			splitVal+= 1;
		}

		rootNode->splitValue = splitVal;
		rootNode->dim = bestDim;
		MyKDNode* leftNode = createMyKDLeaf();
		MyKDNode* rightNode = createMyKDLeaf();

		vector<vector<int> > dimensionsCopy = rootNode->dimensions;

		rootNode->dimensions.clear();
		rootNode->isLeaf = false;
		rootNode->left = leftNode;
		rootNode->right = rightNode;

		if(bestSD == 0) { // No standard deviation, all same values
			for (int i = 0; i < (dimensionsCopy.size()/2); i++) { // Insert first half left
				insertMyKDNodeRecursively(leftNode, dimensionsCopy[i]);
			}
			for (int i = (dimensionsCopy.size()/2); i < dimensionsCopy.size(); i++) { // Insert first half right
				insertMyKDNodeRecursively(rightNode, dimensionsCopy[i]);
			}
		}
		else{
			for (int i = 0; i < dimensionsCopy.size(); i++) {
				if (dimensionsCopy[i][rootNode->dim] < rootNode->splitValue) { // If value at dimension < split value, put left
					insertMyKDNodeRecursively(leftNode, dimensionsCopy[i]);
				}
				else if (dimensionsCopy[i][rootNode->dim] >= rootNode->splitValue) { // If value at dimension >= split value, put right
					insertMyKDNodeRecursively(rightNode, dimensionsCopy[i]);
				}

			}
		}

		return rootNode;
	}

	else if (!rootNode->isLeaf && datapoint[rootNode->dim] < rootNode->splitValue)
		rootNode->left = insertMyKDNodeRecursively(rootNode->left, datapoint);

	else if (!rootNode->isLeaf && datapoint[rootNode->dim] >= rootNode->splitValue)
		rootNode->right = insertMyKDNodeRecursively(rootNode->right, datapoint);

	return rootNode;
}

vector<vector<int> > MyKDTree::MyKDRangeQuery(MyKDNode* rootNode, vector<int>& queries){
	vector<vector<int> > results;
	MyKDRangeQueryRecursive(rootNode, queries,  results);
	return results;
}

void MyKDTree::MyKDRangeQueryRecursive(MyKDNode* rootNode, vector<int>& queries, vector<vector<int> >& queryResults){
	unsigned currentDepth = rootNode->dim % (queries.size() / 2);

	int minQuery = queries[currentDepth * 2];
	int maxQuery = queries[currentDepth * 2 + 1];

	if (!rootNode->isLeaf && minQuery < rootNode->splitValue) {
		MyKDRangeQueryRecursive(rootNode->left, queries, queryResults);
	}
	if (!rootNode->isLeaf && queries[currentDepth * 2 + 1] >= rootNode->splitValue) {
		MyKDRangeQueryRecursive(rootNode->right, queries, queryResults);
	}

	if (rootNode->isLeaf) {
		for (int i = 0; i < rootNode->dimensions.size(); i++) {
			bool isValid = false;

			for (int j = 0; j < queries.size(); j += 2) {
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
			}
		}
		return;
	}
	if (rootNode == nullptr) return;
}

void MyKDTree::deleteMyKDTree(MyKDNode* rootNode) {
	if (rootNode) {
		deleteMyKDTree(rootNode->left);
		deleteMyKDTree(rootNode->right);
		free(rootNode);
	}
}

double MyKDTree::standardDev(vector<vector<int> > points, int dim) {
	int size = points.size();
	double sum = 0.0, mean, sDev = 0.0;
	int i;
	for (i = 0; i < size; i++) {
		sum += points[i][dim];
	}
	mean = sum / size;
	for (i = 0; i < size; i++) {
		sDev += pow(points[i][dim] - mean, 2);
	}
	return sqrt(sDev / size);
}

/* Helper Functions */
bool foundPoint(vector<int> nodePoint, vector<int> searchPoint) {
	for (int i = 0; i < nodePoint.size(); i++) {
		cout << nodePoint.at(i) << " - " << nodePoint.at(i) << endl;
		if (nodePoint.at(i) != searchPoint.at(i)) return false;
	}
	return true;
}

bool compareResult(vector<int> i1, vector<int> i2) {
	for (int i = 0; i < i1.size(); i++) {
		if (!(i1[i] == i2[i])) {
			return (i1[i] < i2[i]);
		}
		if ((i + 1) >= i1.size()) {
			return (i1[i] < i2[i]);
		}
	}
	return (i1[0] < i2[0]);
}

bool compareFirstDim(vector<int> i1, vector<int> i2) {
	return (i1[0] < i2[0]);
}

int getIndex(vector<vector<int> > myVec, int min) {
	for (int i = 0; i < myVec.size(); i++) if (myVec[i][0] >= min) return i;
	return -1;
}

double getMedian(vector<vector<int> > dimensions, unsigned depth) {
	if (dimensions.size() == 1) return dimensions[0][depth];
	vector<int> dimensionalVector;
	for (int i = 0; i < dimensions.size(); i++) 
		dimensionalVector.push_back(dimensions[i][depth]);
	std::sort(dimensionalVector.begin(), dimensionalVector.end());

	if (dimensionalVector.size() % 2 == 0) {
		double middleVal = (dimensionalVector[(dimensionalVector.size() - 1) / 2] + dimensionalVector[dimensionalVector.size() / 2]) / 2.0;
		return middleVal;
	}
	else return dimensionalVector[dimensionalVector.size() / 2];
}

void printResult(vector<vector<int> > results) {
	cout << "***** All Results: *******" << endl;
	// Sort Result Vector on all dimmensions
	std::sort(results.begin(), results.end(), compareResult);

	// Print sorted list
	for (int w = 0; w < results.size(); w++) {
		for (int k = 0; k < results[w].size(); k++) {
			if (k + 1 >= results[w].size()) {
				cout << results[w][k] << endl;
			}
			else {
				cout << results[w][k] << ",";
			}
		}
	}
}

void printToFile(vector<vector<int> > results, string outputFileName) {

	ofstream outputFile;
	outputFile.open(outputFileName, fstream::app);
	outputFile << "********************\n" << endl;
	// Sort Result Vector on all dimmensions
	std::sort(results.begin(), results.end(), compareResult);

	// Print sorted list
	for (int w = 0; w < results.size(); w++) {
		for (int k = 0; k < results[w].size(); k++) {
			if (k + 1 >= results[w].size()) {
				outputFile << results[w][k] << endl;
			}
			else {
				outputFile << results[w][k] << ",";
			}
		}
	}
}

//int rangeQuery() {
int rangeQuery(int searchOption, string databaseFile, string queryFile, int indexBlock) {
	vector<vector<int> > dataPoints;
	fstream db;
	db.open(databaseFile, ios::in);
	if (db.is_open()) {
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
		auto buildStart = Clock::now();

		std::sort(dataPoints.begin(), dataPoints.end(), compareFirstDim);

		auto buildEnd = Clock::now();
		chrono::duration<double> buildElapsed = buildEnd - buildStart;
		cout << "Build Time: " << buildElapsed.count() << endl;

		fstream qu;
		ofstream outputFile;
		outputFile.open("Sequential Query Output.txt"); // To Erase prior contents
		outputFile.close();

		qu.open(queryFile, ios::in);

		if (qu.is_open()) {
			string temp;
			while (getline(qu, temp)) {
				outputFile.open("Sequential Query Output.txt", fstream::app);
				vector<int> query;
				vector<vector<int> > results;
				stringstream line(temp);
				int queryCount = 0;
				while (line.good()) {
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}
				//Query Info
				/*cout << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i	/ 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}*/
				outputFile << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					outputFile << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}

				auto queryStart = Clock::now();

				//Sort based info
				int minVal = query[0];
				int maxVal = query[1];
				int index = getIndex(dataPoints, minVal);
				if (index >= 0) {
					int value = dataPoints[index][0];

					//For loop taking advantage of sort (only looping through 1st dim passes)
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
					auto queryEnd = Clock::now();
					chrono::duration<double> queryElapsed = queryEnd - queryStart;
					cout << "Query Time: " << queryElapsed.count() << endl;
					//printResult(results);

					outputFile.close();
					printToFile(results, "Sequential Query Output.txt");
				}
				else {
					outputFile << "********************\n" << endl;
					outputFile.close();
					//cout << "********************\n" << endl;
				}
			}
		}
		outputFile.close();
		qu.close();

	}

	else if (searchOption == 1) { // kd-tree
		KDNode* rootNode = nullptr;
		KDTree tree;
		tree.indexBlock = indexBlock;

		auto buildStart = Clock::now();

		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertKDNode(rootNode, dataPoints.at(i));
		}

		auto buildEnd = Clock::now();
		chrono::duration<double> buildElapsed = buildEnd - buildStart;
		cout << "Build Time: " << buildElapsed.count() << endl;

		vector<vector<int> > results;
		fstream qu;
		ofstream outputFile;
		outputFile.open("KD-Tree Query Output.txt"); // To Erase prior contents
		outputFile.close();

		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			string temp;
			while (getline(qu, temp)) {
				outputFile.open("KD-Tree Query Output.txt", fstream::app);
				vector<int> query; // each 2 
				stringstream line(temp);

				while (line.good()) {
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}
				
				vector<vector<int> > tempResults;

				auto queryStart = Clock::now();
				tempResults = tree.KDRangeQuery(rootNode, query);
				auto queryEnd = Clock::now();
				chrono::duration<double> queryElapsed = queryEnd - queryStart;
				cout << "Query Time: " << queryElapsed.count() << endl;

				/*cout << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}*/

				outputFile << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					outputFile << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}

				//printResult(tempResults);

				outputFile.close();
				printToFile(tempResults, "KD-Tree Query Output.txt");
			}
		}
		qu.close();
		outputFile.close();
		tree.deleteKDTree(rootNode);
	}

	else if (searchOption == 2) { // MYkd-tree
		MyKDNode* rootNode = nullptr;
		MyKDTree tree;
		tree.indexBlock = indexBlock;

		auto buildStart = Clock::now();

		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertMyKDNode(rootNode, dataPoints.at(i));
		}


		auto buildEnd = Clock::now();
		chrono::duration<double> buildElapsed = buildEnd - buildStart;
		cout << "Build Time: " << buildElapsed.count() << endl;

		vector<vector<int> > results;
		fstream qu;
		ofstream outputFile;
		outputFile.open("MyKD-Tree Query Output.txt"); // To Erase prior contents
		outputFile.close();

		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			string temp;
			while (getline(qu, temp)) {
				outputFile.open("MyKD-Tree Query Output.txt", fstream::app);
				vector<int> query; // each 2 
				stringstream line(temp);

				while (line.good()) {
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}

				/*cout << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}*/

				outputFile << "\n***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					outputFile << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}

				vector<vector<int> > tempResults;
				auto queryStart = Clock::now();
				tempResults = tree.MyKDRangeQuery(rootNode, query);
				auto queryEnd = Clock::now();
				chrono::duration<double> queryElapsed = queryEnd - queryStart;
				cout << "Query Time: " << queryElapsed.count() << endl;

				//printResult(tempResults);

				outputFile.close();
				printToFile(tempResults, "MyKD-Tree Query Output.txt");
			}
		}
		qu.close();
		tree.deleteMyKDTree(rootNode);
	}

	else cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main(int argc, char* argv[]) {
	//cout << argv[1] << endl;
	//cout << argv[2] << endl;
	//cout << argv[3] << endl;
	//int searchOption = 1;
	int searchOption = atoi(argv[1]);
	string databaseFile = string(argv[2]);
	string queryFile = string(argv[3]);
	int indexBlock = 0;

	if (argc == 5) {
		indexBlock = atoi(argv[4]);
	}

	int temp = rangeQuery(searchOption, databaseFile, queryFile, indexBlock);

	// cout << "2d Sequential" << endl;
	//int temp0 = rangeQuery(0, "2dtestDB", "2dprojquery", 0);

	// cout << "\n3d Sequential" << endl;
	// int temp1 = rangeQuery(0, "3dtestDB", "3dprojquery", 0);

	// cout << "\n2d KD" << endl;
	//int temp2 = rangeQuery(1, "2dtestDB", "2dprojquery", 2);

	// cout << "\n3d KD" << endl;
	// int temp3 = rangeQuery(1, "3dtestDB", "3dprojquery", 50);

	// cout << "\n2d MyKD" << endl;
	//int temp4 = rangeQuery(2, "2dtestDB", "2dprojquery", 2);

	// cout << "\n3d MyKD" << endl;
	//int temp5 = rangeQuery(2, "3dtestDB", "3dprojquery", 2);

	// cout << "\nDB Sequential" << endl;
	//int temp6 = rangeQuery(0, "projDB", "projquery", 0);

	// cout << "\nDB KD" << endl;
	//int temp7 = rangeQuery(1, "projDB", "projquery", 2);

	//cout << "\nDB MyKD" << endl;
	//int temp8 = rangeQuery(2, "projDB", "projquery", 2);

	return 0;
}