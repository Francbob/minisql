#ifndef API_H
#define API_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Attribute.h"
#include "Condition.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "CatalogManager.h"

class CatalogManager;
class RecordManager;
class IndexManager;

class API{
public:
	API(){};
	~API(){};
	
	
//Operations of tables
	
	//Give the name of the table(table_name), a vector of attributes and a vector of primarykeys.
	void API_CreateTable(string Table_name, vector<Attribute> set_Attribute, vector<string> PrimaryKey);
	
	//Give the name of the table
	void API_DropTable(string Table_name);
	
	//It's about a select without any conditions(such as 'select * ')
	void API_Select(vector<string> select_Attribute, string Table_name);
	
	//It's about a select with some conditions. You need to give a vector of conditions.    
    void API_Select(vector<string> select_Attribute, string Table_name, vector<condition> select_condition);
    
    //It's a function of insert. You need to give a vector of some maps. 
    //Each map includes a stirng of attribute and a string of data to be inserted.
	void API_Insert(string Table_name, vector<map<string, string> > insert_vector);
	
	//It's a function of dropping a table
	void API_Delete(string Table_name);
	
	//It's a function of dropping some tuples, you need to give a vector of conditions.
	void API_Delete(string Table_name, vector<condition> delete_vector);
	
//Operations of index

	//It's a funcion of creating an index of this table.
	//You need to give a vector of atrributes, and we'll build an index on them.
	void API_CreateIndex(string Table_name, vector<Attribute> index_Attribute);
	
	//It's a function of dropping an index. You need to give a vector of attributes in this index.
	void API_DropIndex(string Table_name, vector<Attribute> index_Attribute);
	
	//It's a function of show index in the whole table.
	void Show_Index(string Table_name);	
	
	
	
};

#endif
