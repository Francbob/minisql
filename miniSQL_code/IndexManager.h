#ifndef IndexManager_H
#define IndexManager_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Attribute.h"
#include "Condition.h"

using namespace std;

class IndexManager{
public:
	IndexManager(){};
	~IndexManager(){};
	
	//Return 1 if create an index successfully, while return 0 if fails to create one
	int CreateIndex(string Table_name, vector<Attribute> vector_Attribute);
	
	//Return 1 if drop all the indexs successfully, while return 0 if fails.
	int DropIndex(string Table_name, vector<vector<string> > Index_name);
	
	//Add some values into indexs;
	//map<string, string> includes attributes and their values;
	//Return 1 if add successfully, while return 0 if fails.
	int AddIndex(string Table_name, vector<map<string, string> > insert_vector);
	
	//Delete all indexs in this table;
	//Return 1 if delete successfully, while return 0 if fails.
	int DeleteWhole(string Table_name);
	
	//Delete some values in indexs in this table;
	//map<string, string> saves the attributes and values which are deleted in the table;
	//vector<map<string, string>> saves all masp;
	//Return 1 if delete successfully, while return 0 if fails.
	int Delete(string Table_name, vector<map<string, string> > ForDeleteIndex);
	
	//Show all indexs of this table;
	//Return 1 if show index successfully, while return 0 if fails.
	int ShowIndex(string Table_name);
};





#endif
