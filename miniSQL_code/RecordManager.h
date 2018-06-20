#ifndef RecordManager_H
#define RecordManager_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Attribute.h"
#include "Condition.h"

using namespace std;

class RecordManager{
public:
	RecordManager(){};
	~RecordManager(){};
	
	//Return 1 if create a table successfully, while return 0 if fails.
	int CreateTable(string Table_name, vector<Attribute> set_Attribute, vector<string> PrimaryKey);
	
	//Return 1 if drop a table successfully, while return 0 if fails.
	int DropTable(string Table_name);
	
	//Select all the tuples in a table;
	//In this function you need to show all attributes included in "select_Attribute" in this table;
	//Return 1 if select successfully, while return 0 if fails.
	int SelectAll(string Table_name, vector<string> select_Attribute);
	
	//Select some attributes with index;
	//In this funtion you need to show all attributes included in "select_Attribute" in this table;
	//All tuples need to satisfy conditions in "select_condition";
	//Return 1 if select successfully, while return 0 if fails.
	int SelectWithIndex(vector<string> select_Attribute, string table_name, vector<condition> select_condition);
	
	//Select some attributes without index;
	//In this funtion you need to show all attributes included in "select_Attribute" in this table;
	//All tuples need to satisfy conditions in "select_condition";
	//Return 1 if select successfully, while return 0 if fails.
	int Select(vector<string> select_Attribute, string Table_name, vector<condition> select_condition);
	
	//Insert some tuples into a table;
	//We use a map<string, string> to save a tuple;
	//The combination of tuples are in a vector<map>;
	//Return 1 if insert successfully, while return 0 if fails.
	int Insert(string Table_name, vector<map<string, string> > insert_vector);
	
	//Delete all tuples in this table;
	//Return 1 if delete successfully, while return 0 if fails;
	int DeleteWhole(string Table_name);
	
	//Delete tuples which satisfy conditions in "delete_vector" in this table;
	//Return 1 if delete successfully, while return 0 if fails;
 	int Delete(string Table_name, vector<condition> delete_vector);
 	
 	//Find attributes and their values which satisfy conditions in "delete_vector" in this table;
 	//A map<string, string> incules attributes and their values in one tuple;
 	//A vector saves all maps;
 	vector<map<string, string> > FindA_and_V(string Table_name, vector<condition> delete_vector);
};



#endif
