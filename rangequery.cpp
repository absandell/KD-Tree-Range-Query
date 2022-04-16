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
		//cout << "Splitting Now. Split value is: " << splitVal << endl;

		rootNode->splitValue = splitVal;
		KDNode* leftNode = createKDLeaf();
		KDNode* rightNode = createKDLeaf();

		vector<vector<int> > dimensionsCopy = rootNode->dimensions;

		//rootNode->dimensions.clear();
		rootNode->isLeaf = false;
		rootNode->left = leftNode;
		rootNode->right = rightNode;

		for (int i = 0; i < dimensionsCopy.size(); i++) {
			//cout << "i: " << i << endl;
			//cout << "Trying to insert: " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << endl;
			if (dimensionsCopy[i][currentDepth] < rootNode->splitValue) {
				//leftNode->dimensions.push_back(rootNode->dimensions[i]);
				insertKDNodeRecursively(leftNode, dimensionsCopy[i], depth + 1);
				//cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Left Node" << endl;
			}
			else if (dimensionsCopy[i][currentDepth] >= rootNode->splitValue) {
				//rightNode->dimensions.push_back(rootNode->dimensions[i]);
				insertKDNodeRecursively(rightNode, dimensionsCopy[i], depth + 1);
				//cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Right Node" << endl;
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
	//cout << "Current Depth: " << currentDepth << endl;
	
	if (!rootNode->isLeaf && queries[currentDepth*2] < rootNode->splitValue) {
		//cout << "Query Value: " << queries[currentDepth * 2] << " Split Value: " << rootNode->splitValue << endl;
		KDRangeQueryRecursive(rootNode->left, queries, depth + 1, queryResults);
	}
	if (!rootNode->isLeaf && queries[currentDepth * 2 + 1] >= rootNode->splitValue) {
		//cout << "Query Value: " << queries[currentDepth * 2 + 1] << " Split Value: " << rootNode->splitValue << endl;
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
				//cout << "Pushing Back: " << rootNode->dimensions[i][0] << "," << rootNode->dimensions[i][1] << endl;
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

MyKDNode* MyKDTree::insertMyKDNode(MyKDNode* rootNode, vector<int> &dataPoint, int indexBlock) {
	return(insertMyKDNodeRecursively(rootNode, dataPoint, indexBlock));
}
double MyKDTree::standardDev(vector<vector<int> > points, int dim){
	int size = points.size();
	double sum = 0.0,mean, sDev = 0.0;
	int i;
	for(i = 0; i < size; i++){
		sum+=points[i][dim];
	}
	mean = sum/size;
	for(i = 0; i < size; i++){
		sDev += pow(points[i][dim]-mean,2);
	}
	return sqrt(sDev/size);
}

MyKDNode* MyKDTree::insertMyKDNodeRecursively(MyKDNode* rootNode, vector<int> &datapoint, int indexBlock){

	if (!rootNode)
		return createMyKDNode(datapoint);

	if (rootNode->isLeaf && rootNode->dimensions.size() < indexBlock) {
		// cout << "Inserting " << datapoint[0] << "," << datapoint[1] << endl;
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
			if(rootNode->dimensions[i][bestDim] < splitVal){
				addOne = false;
			}
		}

		if(addOne){
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
		if(bestSD == 0){
			
			for (int i = 0; i < (dimensionsCopy.size()/2); i++) {
				insertMyKDNodeRecursively(leftNode, dimensionsCopy[i],  indexBlock);
			}
			for (int i = (dimensionsCopy.size()/2); i < dimensionsCopy.size(); i++) {
				insertMyKDNodeRecursively(rightNode, dimensionsCopy[i],  indexBlock);
			}
		}
		else{
			for (int i = 0; i < dimensionsCopy.size(); i++) {
				//cout << "i: " << i << endl;
				//cout << "Trying to insert: " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << endl;
				if (dimensionsCopy[i][rootNode->dim] < rootNode->splitValue) {
					//leftNode->dimensions.push_back(rootNode->dimensions[i]);
					insertMyKDNodeRecursively(leftNode, dimensionsCopy[i],  indexBlock);
					//cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Left Node" << endl;
				}
				else if (dimensionsCopy[i][rootNode->dim] >= rootNode->splitValue) {
					//rightNode->dimensions.push_back(rootNode->dimensions[i]);
					insertMyKDNodeRecursively(rightNode, dimensionsCopy[i], indexBlock);
					//cout << "Inserting " << dimensionsCopy[i][0] << "," << dimensionsCopy[i][1] << " into Right Node" << endl;
				}

			}
		}

		return rootNode;
	}

	else if (!rootNode->isLeaf && datapoint[rootNode->dim] < rootNode->splitValue)
		rootNode->left = insertMyKDNodeRecursively(rootNode->left, datapoint,  indexBlock);

	else if (!rootNode->isLeaf && datapoint[rootNode->dim] >= rootNode->splitValue)
		rootNode->right = insertMyKDNodeRecursively(rootNode->right, datapoint, indexBlock);

	return rootNode;
}

vector<vector<int> > MyKDTree::MyKDRangeQuery(MyKDNode* rootNode, vector<int>& queries){
	vector<vector<int> > results;
	MyKDRangeQueryRecursive(rootNode, queries,  results);
	cout << "Results size: " << results.size() << endl;
	return results;
}

void MyKDTree::MyKDRangeQueryRecursive(MyKDNode* rootNode, vector<int>& queries, vector<vector<int> >& queryResults){
	unsigned currentDepth = rootNode->dim % (queries.size() / 2);
	//cout << "Current Depth: " << currentDepth << endl;

	int minQuery = queries[currentDepth * 2];
	int maxQuery = queries[currentDepth * 2 + 1];

	if (!rootNode->isLeaf && minQuery < rootNode->splitValue) {
		//if (rootNode->left->isLeaf && (rootNode->left->max >= minQuery || rootNode->left->min <= maxQuery))
			//cout << "In Here" << endl;
			MyKDRangeQueryRecursive(rootNode->left, queries, queryResults);
	}
	if (!rootNode->isLeaf && queries[currentDepth * 2 + 1] >= rootNode->splitValue) {
		//cout << "Query Value: " << queries[currentDepth * 2 + 1] << " Split Value: " << rootNode->splitValue << endl;
		//if (rootNode->right->isLeaf && (rootNode->right->max >= minQuery || rootNode->right->min <= maxQuery))
			//cout << "In Here 2" << endl;
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
				//cout << "Pushing Back: " << rootNode->dimensions[i][0] << "," << rootNode->dimensions[i][1] << endl;
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

/* Helper Functions */
bool foundPoint(vector<int> nodePoint, vector<int> searchPoint) {
	for (int i = 0; i < nodePoint.size(); i++) {
		cout << nodePoint.at(i) << " - " << nodePoint.at(i) << endl;
		if (nodePoint.at(i) != searchPoint.at(i)) return false;
	}
	return true;
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

int getIndex(vector<vector<int> > myVec, int min) {
	for (int i = 0; i < myVec.size(); i++) if (myVec[i][0] >= min) return i;
	return -1;
}

double getMedian(vector<vector<int> > dimensions, unsigned depth) {
	if (dimensions.size() == 1) {
		return dimensions[0][depth];
	}
	vector<int> dimensionalVector;
	for (int i = 0; i < dimensions.size(); i++) 
		dimensionalVector.push_back(dimensions[i][depth]);
	std::sort(dimensionalVector.begin(), dimensionalVector.end());

	if (dimensionalVector.size() % 2 == 0) {
		double middleVal = (dimensionalVector[(dimensionalVector.size() - 1) / 2] + dimensionalVector[dimensionalVector.size() / 2]) / 2.0;
		return middleVal;
	}
	else {
		return dimensionalVector[dimensionalVector.size() / 2];
	}
}

void printResult(vector<vector<int> > results) {
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

vector<double> getSplitVals(vector<vector<int> > dimensions, unsigned depth) {

	vector<double> returnVals;
	double splitValueLeft;
	double splitValueMid;
	double splitValueRight;

	vector<int> dimensionalVector;
	for (int i = 0; i < dimensions.size(); i++)
		dimensionalVector.push_back(dimensions[i][depth]);
	std::sort(dimensionalVector.begin(), dimensionalVector.end());


	if (dimensions.size() == 1) {
		splitValueLeft = 0.5 * dimensionalVector[0];
		splitValueMid = dimensionalVector[0];
		splitValueRight = 1.5 * dimensionalVector[0];
	}

	else if (dimensions.size() == 2) {
		splitValueLeft = dimensionalVector[0];
		splitValueMid = (dimensionalVector[0] + dimensionalVector[1]) / 2.0;
		splitValueRight = dimensionalVector[1];
	}

	else if (dimensions.size() == 3) {
		splitValueLeft = dimensionalVector[0];
		splitValueMid = dimensionalVector[1];
		splitValueRight = dimensionalVector[2];
	}

	else {
		if (dimensionalVector.size() % 2 == 0) {
			splitValueLeft = (dimensionalVector[(dimensionalVector.size() -1) / 4] + dimensionalVector[(dimensionalVector.size()) / 4]) / 2.0;
			splitValueMid = (dimensionalVector[(dimensionalVector.size() - 1) / 2] + dimensionalVector[dimensionalVector.size() / 2]) / 2.0;
			splitValueRight = (dimensionalVector[(dimensionalVector.size() - 1) * 3 / 4] + dimensionalVector[(dimensionalVector.size()) * 3 / 4]) / 2.0;
		}
		else {
			splitValueLeft = (dimensionalVector[dimensionalVector.size() / 4]);
			splitValueMid = (dimensionalVector[dimensionalVector.size() / 2]);
			splitValueRight = (dimensionalVector[dimensionalVector.size() * 3 / 4]);
			
		}

	}

	double summedAvg = (splitValueLeft + splitValueMid + splitValueRight) / 3.0;
	if (splitValueLeft == splitValueMid || splitValueMid == splitValueRight) {
		splitValueLeft = summedAvg * .75;
		splitValueMid = summedAvg;
		splitValueRight = summedAvg * 1.25;
	}

	returnVals.push_back(splitValueLeft);
	returnVals.push_back(splitValueMid);
	returnVals.push_back(splitValueRight);

	return returnVals;
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
	vector<vector<int> > dataPoints;
	fstream db;
	db.open(databaseFile, ios::in);
	if (db.is_open()) {
		//cout << "database opened" << endl;
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

	//std::sort(dataPoints.begin(), dataPoints.end(), compareFirstDim);

	if (searchOption == 0) { // Sequential Search
		std::sort(dataPoints.begin(), dataPoints.end(), compareFirstDim);
		//cout << "You have chosen sequential search" << endl;
		
		fstream qu;
		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			//cout << "query file opened" << endl;
			string temp;
			while (getline(qu, temp)) {
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
				/*cout << "***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i	/ 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}*/

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
					printResult(results);
				}
			}
		}
		qu.close();


	}

	else if (searchOption == 1) { // kd-tree
		KDNode* rootNode = nullptr;
		KDTree tree;
		auto buildStart = Clock::now();
		
		tree.indexBlock = indexBlock;

		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertKDNode(rootNode, dataPoints.at(i));
		}

		auto buildEnd = Clock::now();
		chrono::duration<double> buildElapsed = buildEnd - buildStart;
		cout << "Build Time: " << buildElapsed.count() << endl;

		vector<vector<int> > results;
		fstream qu;
		qu.open(queryFile, ios::in);
		if (qu.is_open()) {
			//cout << "query file opened" << endl;
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
				
				vector<vector<int> > tempResults;

				auto queryStart = Clock::now();
				tempResults = tree.KDRangeQuery(rootNode, query);
				auto queryEnd = Clock::now();
				chrono::duration<double> queryElapsed = queryEnd - queryStart;
				cout << "Query Time: " << queryElapsed.count() << endl;

				cout << "***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}
				printResult(tempResults);
			}
		}
		qu.close();
		tree.deleteKDTree(rootNode);
	}

	else if (searchOption == 2) { // MYkd-tree

		
		MyKDNode* rootNode = nullptr;
		MyKDTree tree;
		auto buildStart = Clock::now();


		for (int i = 0; i < dataPoints.size(); i++) {
			rootNode = tree.insertMyKDNode(rootNode, dataPoints.at(i), indexBlock);
		}


		auto buildEnd = Clock::now();
		chrono::duration<double> buildElapsed = buildEnd - buildStart;
		cout << "Build Time: " << buildElapsed.count() << endl;

		vector<vector<int> > results;
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

				cout << "***** Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2) {
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}

				vector<vector<int> > tempResults;

				auto queryStart = Clock::now();

				tempResults = tree.MyKDRangeQuery(rootNode, query);

				auto queryEnd = Clock::now();
				chrono::duration<double> queryElapsed = queryEnd - queryStart;
				cout << "Query Time: " << queryElapsed.count() << endl;

				printResult(tempResults);
				//results.insert(end(results), begin(tempResults), end(tempResults));
			}
		}
		 qu.close();
		
	}

	else cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main() {
	// cout << "2d Sequential" << endl;
	// int temp0 = rangeQuery(0, "2dtestDB", "2dprojquery", 0);

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
	// int temp6 = rangeQuery(0, "projDB", "projquery", 0);

	// cout << "\nDB KD" << endl;
	// int temp7 = rangeQuery(1, "projDB", "projquery", 50);

	cout << "\nDB MyKD" << endl;
	int temp8 = rangeQuery(2, "projDB", "projquery", 50);

	return 0;
}