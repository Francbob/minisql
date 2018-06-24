#ifndef CatalogManager_H
#define CatalogManager_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Attribute.h"
#include "Condition.h"
//#include "buffermanager.h"
//#include "DBfile.h"

using namespace std;


/* There are three tables in catalog
 * First one:
 * Table_name: DB
 * Usage: This table is used to save name of tables in each DataBase
 * (string)DataBase_name | (string)Table_name
 *
 * Second one:
 * Table_name: Table_Information
 * Usage: This table is used to save information of each table
 * (string)Table_name | (string)Attributes | (int)RecordCount
 *
 * Third one:
 * Table_name: Indexs
 * Usage: This tabel is used to save information of indexs in each table
 * (string)Table_name | (string)AttrOfIndex | (int)IfPrimary
 */

class CatalogManager{
public:
	
	//buffermanager* bm;
	
	CatalogManager(){};
	~CatalogManager(){};
	
	
//-------------------------------------------------------------------------------------------------------------//
	//Operations for records

	//build a table
	//add table to "DB"
	//add record to "table_information"
	//add index of primary to "indexs"
	bool Createtable(string Table_name, vector<Attribute> set_Attribute, vector<string> PrimaryKey);
	
	//Return true if there's a table named table_name,
	//while return false if  there's not a table named table_name.
	bool FindTable(string Table_name);
	
	//!****************API call this function*******************************!
	//When insert a record, we need to add the count of record of this table
	//Return true if succeed
	//bool InsertRecord(string Table_name, vector<map<string, string> >)

	bool InsertRecod(string Table_name);//record++
	
	//!**********RecordManager call this function***************************!
	//give count of records tobe deleted
	//return true if succeed
	bool DeleteRecord(string Table_name, int count);
	
	//!**********API call this function*************************************!
	//This function will delete all records of this table in "Table_Information"
	//Return true if delete succeed 
	bool DeleteRecord(string Table_name);
	
	//!**********API call this function*************************************!
	//Give name of the table
	//This function will drop this table in "DB" and "Table_Information" and "Indexs"
	//Return true if succeed
	bool DropTable(string Table_name);

	
	//!**********If you want, every module can call this function***********!
	//This function will show how many records are there in a table
	//Return the number
	int ShowRecordNumber(string Table_name);
	
	//!**********API call this function*************************************!
	//This function judege whether the giving value of attributes fit the table
	//Return true if all is right
	bool JudgeAttrValue(string Table_name, vector<string> insert_vector);

	//Judge whether attributes fit the table
	//Return 0 if not
	int JudgeAttrSelect(string Table_name, vector<string> Attr_judge);
	
	
//---------------------------------------------------------------------------------------------------------------//
	
	//Operations for indexs
	
	//!*********If you want, every module can call this function************!
	//This function will find all indexs in this table, and return the values
	vector<string> FindIndex(string Table_name);
	
	//!*********If you want, every module can call this function************!
	//This function will show all index in this table to users
	bool ShowIndex(string Table_name);
	
	//!*********If you want, every module can call this function************!
	//This function will judge whether there's an index of the giving attributes
	//Return true if exists
	bool WhetherIndex(string Table_name, string FindInIndex);
	
	//!*********API call this function**************************************!
	//This function will add index record into "Index"
	//Return true if add succeed
	bool AddIndex(string Table_name, string AttrForIndex);
	
	//!*********API call this function**************************************!
	//This function will drop index record in "Index"
	//Return true if drop succeed
	bool DeleteIndex(string Table_name, string AttrForIndex);


	//insert_vector are values of attribute to be inserted
	//Return the attribute witch has index and its value
	map<string, string> JudgeInsertIndex(string Table_name, vector<string> insert_vector);


	
	
};

#endif
