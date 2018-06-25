#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "block.h"
#include "buffermanager.h"
#include "Attribute.h"
#include "CatalogManager.h"
#include <map>
#include <sstream>
#include "Condition.h"
#include "API.h"
#include <string>

class RecordManager {
private:
	CatalogManager *cm;
	buffermanager bm;
	int block_size;
	int current_attribute_num;

	std::string PadStringToOutput(std::string padString);

public:
	RecordManager() {};
	~RecordManager() {
		bm.Allwrite2disk();
	}

	// 郑成博给的部分

	void changeDB(const string& a) { bm.changeDB(a); }
	int changeTB(const string& a);

	//get block size;
	int get_size() { return block_size; }
	/*
	* return string:
	* @nullptr: record index is not valid, choose a valid one
	* @else: result
	*/
	std::string find_attr(int block_index, int record_index, int attr_index);

	bool delete_record(int block_index, int record_index);

	bool CreateTable(const std::string &table,
		vector<Attribute> vector_Attribute);
	// API 提供的接口
	//Return 1 if drop a table successfully, while return 0 if fails.
	int DropTable(std::string tableName);

	//Select if all attributes in condition have index
	//Return 0 if select failed
	int Select(std::string Table_name, vector<struct RecordLocation> Index_find);

	//Select without any index
	//Return 0 if select failed
	int Select(std::string Table_name, vector<condition> Condition_no_index);

	//Select with some index, conditino without index is in Condition_no_index
	//Return 0 if select failed
	int Select(std::string Table_name, vector<struct RecordLocation> Index_find, vector<condition> Condition_no_index);

	//Select without any condition
	//Return 0 if select failed
	int Select(std::string Table_name);

	//Insert some tuples into a table;
	//We use a map<std::string, std::string> to save a tuple;
	//The combination of tuples are in a vector<map>;
	//Return 1 if insert successfully, while return 0 if fails.
	int Insert(const std::string Table_name, std::vector<ValueStruct> insert_vector, RecordLocation &location);

	//Delete all tuples in this table;
	//Return 1 if delete successfully, while return 0 if fails;
	int Delete(std::string Table_name);

	//Delete if all attributes in condition have index
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(std::string tableName, vector<struct RecordLocation> Index_find, map<int, vector<std::string>> &del_vector);

	//Delete without any condition
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(std::string Table_name, std::vector<condition> Condition_no_index);

	//Delete with some index, condition without index is in "Condition_no_index"
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(std::string Table_name, std::vector<RecordLocation> Index_find, std::vector<condition> Condition_no_index, map<int, vector<std::string>> &del_vector);
	
	typedef struct
	{
		std::string value;
		int block_index;
		int record_index;
	} del_info, find_info;
};

#endif //MINISQL_RECORDMANAGER_H
