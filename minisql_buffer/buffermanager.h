//
// Created by bob on 2018/6/10.
//

#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <memory.h>
#include <map>
#include <fstream>
#include "DBfile.h"
#include <memory>
static const int MAX_SIZE = 4096;
static const int BUF_FOR_TB = 100;
class buffermanager {
public:
    class block;

public:
    buffermanager();
    explicit buffermanager(const std::string& DB_);
    virtual ~buffermanager() = default;
    void changeDB(const std::string& DB_);
    //You should not change the content in the found block
    //You should only read from it
    const block& get_block() { return get_block(0); }
    const block& get_block(int index);
    const block& get_block(const std::string& table, int index) {
        changeTb(table);
        return get_block(index);
    }
    const block& get_next_block();
    bool insert(const std::string& table, const std::string& record);
    void add_block(int index);
    void changeTb(const std::string& TB);
private:
    block const* cur_block = nullptr;
    DBfile File_Manager;
    std::string Database;
    //Buffer[table offset + block address] = block
    std::vector<block*> Buffer;
    //store offset of each table
    std::map<std::string,size_t> table_offset;
    block& get_vBlock(int index);
    block& get_next_vBlock();
    block* LRU(std::vector<block*>::iterator begin, std::vector<block*>::iterator end);
    std::vector<block*>::iterator table_begin
            (const std::string& table);
    std::vector<block*>::iterator table_end
            (const std::string& table);
};

/*
 * Data will be organized by follow formation:
 * |size|
 * |empty bitmap|
 * |attribute nums|
 * |attribute length| + |attribute offset|
 * |attribute i's data, store in array|
 * @design follows the paper "Weaving Relations for Cache Performance"
 */
class buffermanager::block{
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
    block(int index_,const DBfile& DB);
    //constructor:construct from existed block
    block(int index_, int size_, int attr_n, attribute info);
    //constructor:extremly novel
    block(int index_, int attr_n, int *attr_len);
    ~block(){ delete [] attr_info; }
    void setDirty(){ dirty = true; }
    int get_attr_num() const { return attr_num; }
    int get_size() const { return size; }
    bool is_dirty() const { return dirty; }
    int get_index() const { return index; }
    void freq_add() { freq++; }
    int get_freq() const { return freq; }
    bool write2disk(const DBfile& File_manager);
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
    bool is_full() const { return isFull; }
public:
    string get_attr(int record_index, int attr_index) const;
    void show_record(int record_index) const;
    bool check_record(int record_index) const { return !bitmap[record_index]; }
private:
    bool isFull;
    bool isFree;
    bool *bitmap;
    //for LRU method
    int freq = 0;
    //block index in the DB/table/ folder
    int index;
    //dirty bit, tell us if the block had been modified
    bool dirty;
    //block data
    string data_;
    //basic attributes of the block
    int size;
    int attr_num;
    //offset of each attribute
    attribute attr_info;
private:
    void insert_attr(int attr_index, int record_index, const string& record);
};



#endif //MINISQL_BUFFERMANAGER_H
