#include "recordmanager.h"
#include <direct.h>
#include <cassert>
#include <list>

//Return 1 if drop a table successfully, while return 0 if fails.
int RecordManager::DropTable(string tableName)
{
	// 如果不存在table，这一步据说在API里做了
	/*if (!bm.changeTB(tableName))
		return false;
	*/
	string dirName = tableName;
	rmdir(dirName.c_str());
	return true;
}

//Select if all attributes in condition have index
//Return 0 if select failed
int RecordManager::Select(string tableName, vector<RecordLocation> Index_find)
{
	changeTB(tableName);
	for (auto index : Index_find)
	{
		for (int i = 0; ; i++)
		{
			try
			{
				cout << find_attr(index.block_index, index.offset, i) << "\t|\t";
			}
			catch (exception &ex)
			{
				break;
			}
		}
	}
	// TODO: ??? unique怎么处理？
	cout << endl;
}

//Select without any index
//Return 0 if select failed
int RecordManager::Select(string Table_name, vector<condition> Condition_no_index)
{
	changeTB(Table_name); // attribute's num is destined
	// 内层：按次序的每个记录的属性，外层：所有的记录
	list<vector<string>> selected_records;
	for (int i = 0; i < get_size(); i++)
	{
		for (int j = 0; j < current_attribute_num; j++)
		{
			vector<string> tmp;
			// TODO: 这一行的调用还有点问题（ValueStruct)
			tmp[j] = find_attr(? , i, j);
			selected_records.push_back(tmp);
		}
	}

	for (auto i : Condition_no_index)
	{
		// C#会有一个foreach的bug，不清楚C++有没有，小心为上
		for (vector<string> record : selected_records)
		{
			if (!i.compare(record[? ]))
				selected_records.remove(record);
		}
	}

	cout << selected_records.size() << " records found:\n";
	for (auto record : selected_records)
	{
		for (auto attri_value : record)
		{
			cout << attri_value << "\t|\t";
		}
		cout << endl;
	}
}

//Select with some index, conditino without index is in Condition_no_index
//Return 0 if select failed
int RecordManager::Select(string Table_name, vector<RecordLocation> Index_find, vector<condition> Condition_no_index)
{
	// 基本等于上两个之和，到时候合并就好了
}

//Select without any condition
//Return 0 if select failed
int RecordManager::Select(string Table_name)
{
	changeTB(Table_name); // ???原先是不知道一共有多少个block的
	list<vector<string>> selected_records;
	for (int i = 0; i < get_size(); i++)
	{
		for (int j = 0; j < current_attribute_num; j++)
		{
			vector<string> tmp;
			// TODO: 这一行的调用还有点问题（ValueStruct)
			tmp[j] = find_attr(? , i, j);
			selected_records.push_back(tmp);
		}
	}
	
	cout << selected_records.size() << " records found:\n";
	for (auto record : selected_records)
	{
		for (auto attri_value : record)
		{
			cout << attri_value << "\t|\t";
		}
		cout << endl;
	}

	/*
	for (int i = 0; i < value; i++)
	{
		block ai(i, bm.get_table(), bm);
		for (int i = 0; i < ai.get_size(); i++) // 这里也不知道有多少个tuple
		{
			for (int j = 0; j < ai.get_attr_num(); j++)
				cout << ai.get_attr(i, j) << "\t|\t";
			cout << endl;
		}
	}
	*/
}

//Insert some tuples into a table;
//We use a map<string, string> to save a tuple;
//The combination of tuples are in a vector<map>;
//Return 1 if insert successfully, while return 0 if fails.
int RecordManager::Insert(string tableName, vector<ValueStruct> insert_vector)
{
	string record;
	for (auto i : insert_vector)
	{
		record += i.PadToString();
	}

	changeTB(tableName);
	int target_index = bm.find_free();
	if (!target_index) {
		return 0;
	}
	block* target = new block(target_index, bm.get_table(), bm);
	assert(target->get_size() == record.size());
	int pos = target->insert_record(record);
	target->write2buffer(&bm);
	return 0;
}

//Delete all tuples in this table;
//Return 1 if delete successfully, while return 0 if fails;
int RecordManager::Delete(string tableName)
{
	string dirName = tableName;
	string dos = "del " + dirName + "\\*.db";
	system(dos.c_str());
	return true;
}

//Delete if all attributes in condition have index
//Return the number of tuples you delete
//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
int RecordManager::Delete(string tableName, vector<ValueStruct> Index_find, map<int, int> &Delete_record)
{
	int value = changeTB(tableName);
	for (auto i : Index_find)
	{
	}
}

//Delete without any condition
//Return the number of tuples you delete
//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
int Delete(string Table_name, vector<condition> Condition_no_index, map<int, int> &Delete_record);

//Delete with some index, condition without index is in "Condition_no_index"
//Return the number of tuples you delete
//For each tuple you delete, you need add its location(index of block and this record's offset) to map "Delete_record"
int Delete(string Table_name, map<int, int> Index_find, vector<condition> Condition_no_index, map<int, int> &Delete_record);

// 以下为郑成博提供的

bool RecordManager::CreateTable(const std::string &tableName, vector<Attribute> vector_Attribute) {
	//fetch information of new table
	int attr_num = vector_Attribute.size();
	auto attr_len = new int[attr_num];
	for (int i = 0; i < attr_num; ++i) {
		attr_len[i] = vector_Attribute[i].get_length();
	}
	//auto iter = Buffer.begin();
	int i = 0;
	//initialize all block in the Buffer at once
	block* ptr;
	while (i < 100) {
		ptr = new block(i, attr_num, attr_len);
		ptr->write2buffer(&bm);
		i++;
	}
	delete ptr;
	delete attr_len;
	return true;
}

string RecordManager::find_attr(int block_index, int record_index, int attr_index) {
	block ai(block_index, bm.get_table(), bm);
	return ai.get_attr(record_index, attr_index);
}

int RecordManager::changeTB(const string &a) {
	bm.changeTB(a);
	bm.read_disk();
	return 99999;
}

int RecordManager::insert(const string& record) {
	//    block* model = *Buffer.begin();
	//    //create before insert
	//    assert(model);
	int i = 1;
	int target_index = bm.find_free();
	if (!target_index) {
		//?
	}
	int index = 0;
	block* target = new block(target_index, bm.get_table(), bm);

	int pos = target->insert_record(record);
	target->write2buffer(&bm);
	return pos;
}