#include "CatalogManager.h"
#include <fstream>
#include <sstream>
#include <typeinfo>

using namespace std;

int CatalogManager::to_int(const string& str) {
	stringstream temp;
	temp << str;
	int i;
	temp >> i;
	return i;
}

string CatalogManager::to_string(const int& i) {
	stringstream temp;
	temp << i;
	return temp.str();
}

void CatalogManager::Close() {
	int i, j;
	/*DB_file************************************************
	i = remove("DB");
	ofstream file_DB("DB",ios::out|ios::trunc);
	*********************************************************/
	i = remove("Table_index");
	ofstream file_TableIndex("Table_index", ios::out | ios::trunc);
	i = remove("Table_info");
	ofstream file_TableInfo("Table_info", ios::out | ios::trunc);
	/*DB_file*************************************************
	int size  = DB_array.size();
	for(i=0; i<size; i++){
	string temp;
	temp += DB_array[i].DB_name;
	temp += ' ';
	temp += DB_array[i].Table_name;
	temp += ' ';
	temp += to_string(DB_array[i].Record_num);
	temp += '\n';
	file_DB<<temp;
	}
	***********************************************************/
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		string temp;
		temp += TableInfo_array[i].Table_name;
		temp += ' ';
		temp += to_string(TableInfo_array[i].Record_num);
		temp += ' ';
		temp += to_string(TableInfo_array[i].Attr_num);
		temp += ' ';
		for (j = 0; j<TableInfo_array[i].Attr_num; j++) {
			temp += TableInfo_array[i].Attributes[j].name;  // Attribute name
			temp += ' ';
			temp += to_string(TableInfo_array[i].Attributes[j].type); // Attribute type
			temp += ' ';
			temp += to_string(TableInfo_array[i].Attributes[j].unique);// Whether unique
			temp += ' ';
		}
		temp += to_string(TableInfo_array[i].Primary_num);
		temp += ' ';
		for (j = 0; j<TableInfo_array[i].Primary_num; j++) {
			temp += TableInfo_array[i].Primary[j];
			temp += ' ';
		}
		temp += to_string(TableInfo_array[i].Block_num);
		temp += '\n';
		file_TableInfo << temp;
	}

	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		string temp;
		temp += TableIndex_array[i].Table_name;
		temp += ' ';
		temp += to_string(TableIndex_array[i].Index_num);
		temp += ' ';
		map<string, string>::iterator iter;
		map<string, string>::iterator iter_end;
		iter = TableIndex_array[i].N_and_A.begin();
		iter_end = TableIndex_array[i].N_and_A.end();
		iter_end--;
		while (iter != TableIndex_array[i].N_and_A.end()) {
			temp += iter->first;
			temp += ' ';
			if (iter != iter_end) {
				temp += iter->second;
				temp += ' ';
			}
			else {
				temp += iter->second;
			}
			iter++;
		}
		temp += '\n';
		file_TableIndex << temp;
	}
	file_TableIndex.close();
	file_TableInfo.close();
	return;
}

void CatalogManager::Init() {
	/*DB_file**************************************************
	ifstream file_DB("DB", ios::in);
	***********************************************************/
	ifstream file_TableInfo("Table_info", ios::in);
	ifstream file_TableIndex("Table_index", ios::in);
	string readline;
	int j;
	/*DB_file***************************************************
	while(getline(file_DB, readline)){
	struct DB_file DB_struct;
	int length = readline.size();
	int i=0;
	string temp;
	while(readline[i]!=' '){
	temp += readline[i];
	i++;
	}
	DB_struct.DB_name = temp;
	i++;
	temp.clear();
	while(readline[i]!=' '){
	temp += readline[i];
	i++;
	}
	DB.struct.Table_name = temp;
	i++;
	temp.clear();
	while(i<length){
	temp += readline[i];
	i++;
	}
	DB_struct.Record_num = to_int(temp);
	DB_array.push_back(DB_struct);
	}

	readline.clear();
	*****************************************************************/

	while (getline(file_TableInfo, readline)) {
		struct Table_info_file TableInfo_struct;
		int length = readline.size();
		int i = 0;
		string temp;
		//Table name
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableInfo_struct.Table_name = temp;
		i++;
		temp.clear();

		//Record_num
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableInfo_struct.Record_num = to_int(temp);
		i++;
		temp.clear();

		//Attruibute number
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableInfo_struct.Attr_num = to_int(temp);

		//Attrubutes
		for (j = 0; j<TableInfo_struct.Attr_num; j++) {
			struct For_attr Attribute_temp;
			i++;
			temp.clear();
			while (readline[i] != ' ') {
				temp += readline[i];
				i++;
			}
			Attribute_temp.name = temp;
			i++;
			temp.clear();
			while (readline[i] != ' ') {
				temp += readline[i];
				i++;
			}
			Attribute_temp.type = to_int(temp);
			i++;
			temp.clear();
			while (readline[i] != ' ') {
				temp += readline[i];
				i++;
			}
			Attribute_temp.unique = to_int(temp);
			TableInfo_struct.Attributes.push_back(Attribute_temp);
		}
		i++;
		temp.clear();

		//PrimaryKey number
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableInfo_struct.Primary_num = to_int(temp);

		//Primary key
		for (j = 0; j<TableInfo_struct.Primary_num; j++) {
			i++;
			temp.clear();
			while (readline[i] != ' ') {
				temp += readline[i];
				i++;
			}
			TableInfo_struct.Primary.push_back(temp);
		}
		i++;
		temp.clear();
		//Block number
		while (i<length) {
			temp += readline[i];
			i++;
		}
		TableInfo_struct.Block_num = to_int(temp);
		TableInfo_array.push_back(TableInfo_struct);
	}
	readline.clear();
	while (getline(file_TableIndex, readline)) {
		struct Table_index_file TableIndex_struct;
		int length = readline.size();
		int i = 0;
		string temp;

		//Table name
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableIndex_struct.Table_name = temp;
		i++;
		temp.clear();

		//Index Number
		while (readline[i] != ' ') {
			temp += readline[i];
			i++;
		}
		TableIndex_struct.Index_num = to_int(temp);

		//Indexs
		for (j = 0; j<TableIndex_struct.Index_num; j++) {
			i++;
			temp.clear();
			while (readline[i] != ' ') {
				temp += readline[i];
				i++;
			}
			string IndexName = temp;
			string IndexAttr;
			i++;
			temp.clear();
			if (j != TableIndex_struct.Index_num - 1) {
				while (readline[i] != ' ') {
					temp += readline[i];
					i++;
				}
				IndexAttr = temp;
			}
			else {
				while (i<length) {
					temp += readline[i];
					i++;
				}
				IndexAttr = temp;
			}
			TableIndex_struct.N_and_A.insert(pair<string, string>(IndexName, IndexAttr));
		}
		TableIndex_array.push_back(TableIndex_struct);
	}
	file_TableIndex.close();
	file_TableInfo.close();
	return;
}

int CatalogManager::GetBlockNum(string tableName) {
	int i;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == tableName) return TableInfo_array[i].Block_num;
	}
}

int CatalogManager::GetAttrIndex(string tableName, string attrName) {
	int i, j;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == tableName) {
			size = TableInfo_array[i].Attr_num;
			for (j = 0; j<size; j++) {
				if (TableInfo_array[i].Attributes[j].name == attrName) {
					return j;
				}
			}
		}
	}
}

bool CatalogManager::Createtable(string Table_name, vector<Attribute> set_Attribute, vector<string> PrimaryKey) {
	int i;
	/*DB_file*********************************************
	struct DB_file DB_struct;
	******************************************************/
	struct Table_info_file TableInfo_struct;
	struct Table_index_file TableIndex_struct;

	/*DB_file*********************************************
	DB_struct.Table_name = Table_name;
	DB_struct.DB_name = DB;
	DB_struct.Record_num = 0;
	DB_array.push_back(DB_struct);
	*****************************************************/

	TableInfo_struct.Table_name = Table_name;
	TableInfo_struct.Record_num = 0;
	TableInfo_struct.Attr_num = set_Attribute.size();
	int size = set_Attribute.size();
	for (i = 0; i<size; i++) {
		struct For_attr Attr_temp;
		Attr_temp.name = set_Attribute[i].get_name();
		Attr_temp.type = set_Attribute[i].get_type();
		Attr_temp.unique = set_Attribute[i].get_ifunique();
		TableInfo_struct.Attributes.push_back(Attr_temp);
	}
	size = PrimaryKey.size();
	TableInfo_struct.Primary_num = size;
	for (i = 0; i<size; i++) {
		TableInfo_struct.Primary.push_back(PrimaryKey[i]);
	}
	TableInfo_struct.Block_num = 100;
	TableInfo_array.push_back(TableInfo_struct);

	TableIndex_struct.Table_name = Table_name;
	TableIndex_struct.Index_num = PrimaryKey.size();
	//默认主键的index名为Table_属性名
	for (i = 0; i<size; i++) {
		string Index_name = "Table_";
		Index_name += PrimaryKey[i];
		TableIndex_struct.N_and_A.insert(pair<string, string>(Index_name, PrimaryKey[i]));
	}
	TableIndex_array.push_back(TableIndex_struct);
	return true;
}

bool CatalogManager::FindTable(string Table_name) {
	int i;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) return true;
	}
	return false;
}

bool CatalogManager::InsertRecord(string Table_name) {
	int i;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			TableInfo_array[i].Record_num++;
			return true;
		}
	}
	return false;
}

bool CatalogManager::DeleteRecord(string Table_name, int count) {
	int i;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			TableInfo_array[i].Record_num -= count;
			return true;
		}
	}
	return false;
}

bool CatalogManager::DeleteRecord(string Table_name) {
	int i;
	int size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			TableInfo_array[i].Record_num = 0;
			return true;
		}
	}
	return false;
}

bool CatalogManager::DropTable(string Table_name) {
	int i;
	int size;
	/*DB_file****************************************
	for(i=0; i<size; i++){
	if(DB_array.Table_name == Table_name){
	DB_array.erase(DB_array.begin() + i);
	}
	}
	*************************************************/
	size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			TableInfo_array.erase(TableInfo_array.begin() + i);
		}
	}
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			TableIndex_array.erase(TableIndex_array.begin() + i);
			return true;
		}
	}
	return false;
}

int CatalogManager::ShowRecordNumber(string Table_name) {
	int i, size;
	size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			return TableInfo_array[i].Record_num;
		}

	}
}

int CatalogManager::ReturnType(string test) {
	int int_test;
	float float_test;
	string string_test;
	if (typeid(test).name() == typeid(int_test).name()) {
		return 0;
	}
	else if (typeid(test).name() == typeid(float_test).name()) {
		return 256;
	}
	else if (typeid(test).name() == typeid(string_test).name()) {
		return test.size();
	}
}

bool CatalogManager::JudgeAttrValue(string Table_name, vector<string> insert_vector) {
	int i, size, j;
	size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			int size_2 = TableInfo_array[i].Attr_num;
			if (insert_vector.size() != size_2) return false;
			for (j = 0; j<size_2; j++) {
				int temp = ReturnType(insert_vector[j]);
				if (temp == 0 || temp == 256) {
					if (temp != TableInfo_array[i].Attributes[j].type) {
						return false;
					}
				}
				else {
					if (TableInfo_array[i].Attributes[j].type == 0 || TableInfo_array[i].Attributes[j].type == 256) {
						return false;
					}
				}
			}
			break;
		}
	}
	return true;
}

int CatalogManager::JudgeAttrSelect(string Table_name, vector<string> Attr_judge) {
	int i, j, k;
	int size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			int size_2 = Attr_judge.size();
			for (j = 0; j<size_2; j++) {
				int flag = 0;
				map<string, string>::iterator iter;
				iter = TableIndex_array[i].N_and_A.begin();
				while (iter != TableIndex_array[i].N_and_A.end()) {
					if (iter->second == Attr_judge[j]) {
						flag = 1;
						break;
					}
					iter++;
				}
				if (flag == 0) return 0;
			}
		}
	}
	return 1;
}

vector<string> CatalogManager::FindIndex(string Table_name) {
	int i, j, size;
	vector<string> result;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			int size_2 = TableIndex_array[i].Index_num;
			map<string, string>::iterator iter;
			iter = TableIndex_array[i].N_and_A.begin();
			while (iter != TableIndex_array[i].N_and_A.end()) {
				result.push_back(iter->first);
				iter++;
			}
		}
	}
	return result;
}

bool CatalogManager::ShowIndex(string Table_name) {
	int i, j, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			cout.setf(ios::left);
			cout.width(30);
			cout << "Index name";
			cout << "|";
			cout.setf(ios::left);
			cout.width(30);
			cout << "Attribute name";
			cout << "|" << endl;
			map<string, string>::iterator iter;
			iter = TableIndex_array[i].N_and_A.begin();
			while (iter != TableIndex_array[i].N_and_A.end()) {
				cout.setf(ios::left);
				cout.width(30);
				cout << iter->first;
				cout << "|";
				cout.setf(ios::left);
				cout.width(30);
				cout << iter->second;
				cout << "|" << endl;
				iter++;
			}
		}
	}
	return true;
}

bool CatalogManager::WhetherIndex(string Table_name, vector<string> Attributes) {
	int i, j, k, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			int size_2 = Attributes.size();
			for (j = 0; j<size_2; j++) {
				int flag = 0;
				map<string, string>::iterator iter;
				iter = TableIndex_array[i].N_and_A.begin();
				while (iter != TableIndex_array[i].N_and_A.end()) {
					if (iter->second == Attributes[j]) {
						flag = 1;
						break;
					}
					iter++;
				}
				if (flag == 0) {
					return false;
				}
			}
		}
	}
	return true;
}

bool CatalogManager::WhetherIndex(string Index_name) {
	int i, j, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		map<string, string>::iterator iter;
		iter = TableIndex_array[i].N_and_A.begin();
		while (iter != TableIndex_array[i].N_and_A.end()) {
			if (iter->first == Index_name) {
				return true;
			}
			iter++;
		}
	}
	return false;
}

bool CatalogManager::AddIndex(string Table_name, string Attribute_name, string Index_name) {
	int i, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			TableIndex_array[i].Index_num++;
			TableIndex_array[i].N_and_A.insert(pair<string, string>(Index_name, Attribute_name));
			return true;
		}
	}
	return false;
}

bool CatalogManager::DeleteIndex(string Index_name) {
	int i, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		map<string, string>::iterator iter;
		iter = TableIndex_array[i].N_and_A.begin();
		while (iter != TableIndex_array[i].N_and_A.end()) {
			if (iter->first == Index_name) {
				TableIndex_array[i].N_and_A.erase(iter);
				TableIndex_array[i].Index_num--;
				return true;
			}
			iter++;
		}
	}
	return false;
}

string CatalogManager::FindAttrName(string Table_name, int location) {
	int i, size;
	size = TableInfo_array.size();
	for (i = 0; i<size; i++) {
		if (TableInfo_array[i].Table_name == Table_name) {
			return TableInfo_array[i].Attributes[location].name;
		}
	}
}

bool CatalogManager::IndexMapFindAttr(string Table_name, string Attribute) {
	int i, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			map<string, string>::iterator iter;
			iter = TableIndex_array[i].N_and_A.begin();
			while (iter != TableIndex_array[i].N_and_A.end()) {
				if (iter->second == Attribute) {
					return true;
				}
				iter++;
			}
		}
	}
	return false;
}

string CatalogManager::IndexMapFindKey(string Table_name, string Attribute) {
	int i, size;
	size = TableIndex_array.size();
	for (i = 0; i<size; i++) {
		if (TableIndex_array[i].Table_name == Table_name) {
			map<string, string>::iterator iter;
			iter = TableIndex_array[i].N_and_A.begin();
			while (iter != TableIndex_array[i].N_and_A.end()) {
				if (iter->second == Attribute) return iter->first;
				iter++;
			}
		}
	}
}

map<string, struct ValueStruct> CatalogManager::JudgeInsertIndex(string Table_name, vector<struct ValueStruct> insert_vector) {
	int i, size;
	map<string, struct ValueStruct> result;
	size = insert_vector.size();
	for (i = 0; i<size; i++) {
		string Attribute_name = FindAttrName(Table_name, i);
		if (IndexMapFindAttr(Table_name, Attribute_name)) {
			result.insert(pair<string, struct ValueStruct>(IndexMapFindKey(Table_name, Attribute_name), insert_vector[i]));
		}
	}
	return result;
}