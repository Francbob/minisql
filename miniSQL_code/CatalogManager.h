#ifndef CatalogManager_H
#define CatalogManager_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Attribute.h"
#include "Condition.h"

using namespace std;

class CatalogManager{
public:
	CatalogManager(){};
	~CatalogManager(){};
	
	//Return true if there's a table named table_name,
	//while return false if  there's not a table named table_name. 
	bool FindTable(string Table_name);
	
	//Return all the combination of attributes. Each vector<string> includes a combination;
	//All vectors are in a vector<string>
	vector<vector<string> > FindIndex(string Table_name); 
	
	//"FindInIndex" includes the attributes we're going to judge whether there's an index on these attributes.
	//Return true if there's an index on them, while return false if not.
	bool WhetherIndex(string Table_name, vector<string> FindInIndex);
	
};

#endif
