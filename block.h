//
// Created by bob on 2018/6/23.
//

#ifndef MINISQL_BLOCK_H
#define MINISQL_BLOCK_H

#include "buffermanager.h"
#include <string>
#include "DBfile.h"

using namespace std;

class block {
public:
    //attributes are stored in order
    //attribute's name should be refereed to meta data
    typedef struct ATTR{
        ATTR() = default;
        ATTR(int l, int off) : attr_len(l),attr_offset(off){}
        int attr_len;
        int attr_offset;
    }* attribute;
public:
    block() : index(-1) {}
    //constructor:read existed block
    block(int index_,const string& TB,buffermanager& bm);
    //constructor:construct from existed block
    block(int index_, const string& TB, const string& get);
    block(int index_, int size_, int attr_n, attribute info);
    //constructor:extremly novel
    block(int index_, int attr_n, int *attr_len);
    ~block(){ delete [] attr_info;}
    attribute get_attr_info() { return attr_info; }
    int get_attr_num() const { return attr_num; }
    int get_size() const { return size; }
    int get_index() const { return index; }
    /*
     * New record should be organized into a std::string
     * eq: #for     attr | name | age | gender
     *         record a | bob  | 20  | male
     *     #insert record a:
     *         insert_record("bob" + to_string(20) + "male");
     * @return true: succeed
     * @return false: fail
     */
    bool insert_record(const std::string& record);
    int write2buffer(buffermanager* bm);
public:
    string get_attr(int record_index, int attr_index) const;
    void show_record(int record_index) const;
    bool check_record(int record_index) const { return !bitmap[record_index]; }
private:
    void insert_attr(int attr_index, int record_index, const string& record);
    bool *bitmap;
    //block index in the DB/table/ folder
    int index;
    //block data
    string data_;
    //basic attributes of the block
    int size;
    int attr_num;
    //offset of each attribute
    attribute attr_info;
};


#endif //MINISQL_BLOCK_H
