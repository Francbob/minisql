#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "block.h"
#include "buffermanager.h"
#include "Attribute.h"
#include <map>
#include "Condition.h"
#include "API.h"

class RecordManager {
private:
	buffermanager bm;
	int current_attribute_num;
public:
	RecordManager() {};
	~RecordManager() {
		bm.Allwrite2disk();
	}

	// 郑成博给的部分
	void changeDB(const string& a) { bm.changeDB(a); }
	int changeTB(const string& a);

	bool CreateTable(const std::string &table,
		vector<Attribute> vector_Attribute);
	string find_attr(int block_index, int record_index, int attr_index);
	int insert(const string& record);

	// API 提供的接口
	//Return 1 if drop a table successfully, while return 0 if fails.
	int DropTable(string tableName);

	//Select if all attributes in condition have index
	//Return 0 if select failed
	int Select(string Table_name, vector<RecordLocation> Index_find);

	//Select without any index
	//Return 0 if select failed
	int Select(string Table_name, vector<condition> Condition_no_index);

	//Select with some index, conditino without index is in Condition_no_index
	//Return 0 if select failed
	int Select(string Table_name, vector<RecordLocation> Index_find, vector<condition> Condition_no_index);

	//Select without any condition
	//Return 0 if select failed
	int Select(string Table_name);

	//Insert some tuples into a table;
	//We use a map<string, string> to save a tuple;
	//The combination of tuples are in a vector<map>;
	//Return 1 if insert successfully, while return 0 if fails.
	int Insert(string Table_name, vector<ValueStruct> insert_vector);

	//Delete all tuples in this table;
	//Return 1 if delete successfully, while return 0 if fails;
	int Delete(string Table_name);

	//Delete if all attributes in condition have index
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(string Table_name, std::vector<RecordLocation> Index_find, map<int, int> &Delete_record);

	//Delete without any condition
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(string Table_name, std::vector<condition> Condition_no_index, map<int, int> &Delete_record);

	//Delete with some index, condition without index is in "Condition_no_index"
	//Return the number of tuples you delete
	//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
	int Delete(string Table_name, std::vector<RecordLocation> Index_find, std::vector<condition> Condition_no_index, std::map<int, int> &Delete_record);
	

};

#endif //MINISQL_RECORDMANAGER_H
