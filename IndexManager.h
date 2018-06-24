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
	//int CreateIndex(string Table_name, string vector_Attribute);
	
	//build an index on primary key on this table
	//Return 1 if create succeed, while return  0 if fails
	int CreatePrimaryIndex(string Table_name, vector<Attribute> PrimaryKey);

	//Return 1 if drop index successfully, while return 0 if fails.
	int DropIndex(string Table_name, string Index_name);

	//Drop all indexs given in "Index_name"
	//return 0 if fails
	int DropIndex(string Table_name, vector<string> Index_name);
	
	//Add some values into indexs;
	//map<string, string> includes attributes and their values;
	//Return 1 if add successfully, while return 0 if fails.
	int AddIndex(string Table_name, map<string, string> insert_vector);
	
	//Delete all indexs in this table;
	//Return 1 if delete successfully, while return 0 if fails.
    int Delete(string Table_name, vector<string> DeleteInex_attr);
	
	//Delete some values in indexs in this table;
	//map<int, int> saves the index of block and offset of record;
	//Return 1 if delete successfully, while return 0 if fails.
	int Delete(string Table_name, map<int, int> ForDeleteIndex);
	
	//Show all indexs of this table;
	//Return 1 if show index successfully, while return 0 if fails.
	//int ShowIndex(string Table_name);

	//Find records fit condition
	//We promise the attributes in "Selet_Condition" always have index
	//Return index of block and ofset of record
	//map<int, int> Select(string Table_name, vector<condition> Select_Condition);
};





#endif
