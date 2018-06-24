//
// Created by bob on 2018/6/24.
//

#include "recordmanager.h"
#include <cassert>

class recordmanager {
private:
	buffermanager bm;
public:
	void changeDB(const string& a) { bm.changeDB(a); }
	void changeTB(const string& a);
	recordmanager() = default;
	~recordmanager() {
		bm.Allwrite2disk();
	}
	bool create(const std::string &table,
		vector<Attribute> vector_Attribute);
	string find_attr(int block_index, int record_index, int attr_index);
	int insert(const string& record);
};

bool recordmanager::create
        (const std::string &table,
         vector<Attribute> vector_Attribute) {
    //table should not already exist
    assert(!bm.changeTB(table));
    //fetch information of new table
    int attr_num = vector_Attribute.size();
    auto attr_len = new int[attr_num];
    for (int i = 0; i < attr_num; ++i) {
        attr_len[i] = vector_Attribute[i].get_length();
    }
    //auto iter = Buffer.begin();
    int i = 0;
    //initialize all block in the Buffer at once
    while (i < 100){
        auto ptr = new block(i, attr_num, attr_len);
        ptr->write2buffer(&bm);
        i++;
//        ptr->setDirty();
//        ptr->write2disk(File_Manager);
//        (*iter) = ptr;
//        iter++;
//        i++;
    }

    return true;
}

string recordmanager::find_attr(int block_index, int record_index, int attr_index) {
    block ai(block_index, bm.get_table(), bm);
    return ai.get_attr(record_index, attr_index);
}

void recordmanager::changeTB(const string &a) {
    bm.changeTB(a);
    bm.read_disk();
}
int recordmanager::insert(const string& record) {
//    block* model = *Buffer.begin();
//    //create before insert
//    assert(model);
    int i = 1;
    int target_index = bm.find_free();
    if(!target_index){
        //?
    }
    int index = 0;
    block* target = new block(target_index, bm.get_table(), bm);

    int pos = target->insert_record(record);
    target->write2buffer(&bm);
    return pos;
}