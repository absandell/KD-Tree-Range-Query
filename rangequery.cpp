#include "rangequery.h"
using namespace std;

KDNode *KDTree::createKDNode(vector<int> dimensions)
{
	KDNode *newNode = new KDNode();
	for (int i = 0; i < dimensions.size(); i++)
		newNode->dimensions.push_back(dimensions[i]);
	newNode->left = nullptr;
	newNode->right = nullptr;
	return newNode;
}

KDNode *KDTree::insertKDNode(KDNode *rootNode, vector<int> dimensions)
{
	return (insertKDNodeRecursively(rootNode, dimensions, 0));
}

KDNode *KDTree::insertKDNodeRecursively(KDNode *rootNode, vector<int> dimensions, unsigned depth)
{
	if (!rootNode)
		return createKDNode(dimensions);

	unsigned currentDepth = depth % dimensions.size();

	if (dimensions[currentDepth] < rootNode->dimensions[currentDepth])
		;
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

bool compareVector(vector<int> i1, vector<int> i2)
{
	return (i1[0] < i2[0]);
}

int getIndex(vector<vector<int> > myVec, int min)
{
	for (int i = 0; i < myVec.size(); i++)
	{
		if (myVec[i][0] >= min)
		{
			return i;
		}
	}
	return -1;
}

// int rangeQuery(int argc, char* argv[]) {
int rangeQuery(int searchOption, string databaseFile, string queryFile)
{
	/*int searchOption = atoi(argv[1]);
	string databaseFile = string(argv[2]);
	string queryFile = string(argv[3]);*/

	/*
		Array of data points. Each entry in array will contain record

		1.) Read data into an array / vector
		2.) Free to sort data by any of the dimensions if wanted (not required)
		3.) Search sequentially

	*/
	if (searchOption == 0)
	{ // Sequential Search

		cout << "You have chosen sequential search" << endl;

		// Read DB points into k-dim vector
		vector<vector<int> > dataPoints;
		fstream db;
		db.open(databaseFile, ios::in);
		if (db.is_open())
		{
			cout << "database opened" << endl;
			string temp;
			while (getline(db, temp))
			{
				vector<int> entry;
				stringstream line(temp);

				while (line.good())
				{
					string substring;
					getline(line, substring, ',');
					entry.push_back(stoi(substring));
				}

				dataPoints.push_back(entry);
			}
		}
		db.close();

		// Sort DB points on 1st value
		sort(dataPoints.begin(), dataPoints.end(), compareVector);

		
		//Print sorted list
		// for(int w = 0; w < dataPoints.size();w++){
		// 	cout << "[" << dataPoints[w][0] << ", " << dataPoints[w][1] << ", " << dataPoints[w][2] << "] ";
		// }


		fstream qu;
		qu.open(queryFile, ios::in);
		if (qu.is_open())
		{
			cout << "query file opened" << endl;
			string temp;
			while (getline(qu, temp))
			{
				vector<int> query;
				stringstream line(temp);

				while (line.good())
				{
					string substring;
					getline(line, substring, ' ');
					query.push_back(stoi(substring));
				}
				//Query Info
				cout << "*****Query: *******" << endl;
				for (int i = 0; i < query.size(); i += 2)
				{
					cout << "Range " << (i / 2) + 1 << ": " << query[i] << "->" << query[i + 1] << endl;
				}
				
				//Sort based info

				int minVal = query[0];
				int maxVal = query[1];
				int index = getIndex(dataPoints, minVal);
				int value = dataPoints[index][0];

				// cout << "Min: " << minVal << endl;
				// cout << "Max: " << maxVal << endl;
				// cout << "Index: " << index << endl;
				// cout << "Value: " << value << endl;
				
				//For loop taking advantage of sort (only looping through 1st dim passes)
				cout << "Points: " << endl;
				for(int i = index; value <= maxVal; i++){
					vector<bool> pass;
					for (int j = 2; j < query.size(); j += 2)
					{
						if (query[j] <= dataPoints[i][j / 2])
						{
							if (query[j + 1] >= dataPoints[i][j / 2])
							{
								pass.push_back(true);
							}
							else
							{
								pass.push_back(false);
							}
						}
						else
						{
							pass.push_back(false);
						}
					}
					
					//If it passes 2->k dimmension range query
					if (find(pass.begin(), pass.end(), false) == pass.end())
					{
						for (int k = 0; k < dataPoints[k].size(); k++)
						{
							if (k + 1 == dataPoints[k].size())
							{
								cout << dataPoints[i][k] << endl;
							}
							else
							{
								cout << dataPoints[i][k] << ",";
							}
						}
					}
					//At end of sorted DB
					if(i+1 >= dataPoints.size()){
						break;
					}
					//Set value to next index for conditional 
					value = dataPoints[i+1][0];
				}
				
			}
		}
		qu.close();
	}

	/*
		Tree-based options. Each block at leaf level will contain indexBlock number of key->pointer pairs
	*/
	else if (searchOption == 1)
	{ // kd-tree
		cout << "You have chosen search using kd-tree" << endl;
	}

	else if (searchOption == 2)
	{ // MYkd-tree
		cout << "You have chose search using MYkd-tree" << endl;
	}

	else
		cout << "Please enter a valid query selection!" << endl;

	return 0;
}

int main()
{

	int temp = rangeQuery(0, "projDB", "projquery");

	return 0;
}