#include "rangequery.h"
using namespace std;


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

*/

int rangeQuery(int argc, char* argv[]) {
	int searchOption = atoi(argv[1]);
	string databaseFile = string(argv[2]);
	string queryFile = string(argv[3]);


	/*
		Array of data points. Each entry in array will contain record

		1.) Read data into an array
		2.) Free to sort data by any of the dimensions if wanted (not required)
		3.) Search sequentially

	*/
	if (searchOption == 0) { // Sequential Search

		cout << "You have chose sequential search" << endl;
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