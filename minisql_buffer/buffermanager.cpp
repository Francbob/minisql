//
// Created by bob on 2018/6/10.
//

#include "buffermanager.h"
#include <cassert>
#include <random>
static buffermanager::block* nullblock = new buffermanager::block();
/*
 * Constructor for new block
 * !**********stupid design, I think it will be modified letter**********!
 */
buffermanager::block::block(int index_, int size_, int attr_n, buffermanager::block::attribute info)
    : dirty(false), index(index_), size(size_), attr_num(attr_n), isFree(false), isFull(false)
{
    bitmap = new bool[size];
    for (int i = 0; i < size; ++i) {
        //at the very beginning, all empty
        bitmap[i] = true;
    }

    attr_info = new ATTR[attr_num];
    for (int j = 0; j < attr_num; ++j) {
        attr_info[j] = ATTR(info[j].attr_len, info[j].attr_offset);
    }

    unsigned int data_space = 0;
    for (int k = 0; k < attr_num; ++k) {
        data_space += attr_info[k].attr_len;
    }
    data_space *= size;
    data_.append(data_space,'0');
}
/*
 * Constructor to read data from existed block
 */
buffermanager::block::block(int index_, const DBfile &DB)
        : dirty(false),index(index_),isFree(false),isFull(false)
{
    ifstream fptr;
    string line;
    fptr.open(DB.get_path(index));
    assert(fptr);

    //analyse data scale in the block file
    getline(fptr,line);
    size = stoi(line);

    //get the empty bitmap
    bitmap  = new bool[size];
    char c;
    for (int j = 0; j < size; ++j) {
        fptr >> c;
        switch (c){
            case '0': bitmap[j] = false;
                break;
            case '1': bitmap[j] = true;
                break;
            default:
                break;
        }
    }
    fptr >> attr_num;
    getline(fptr,line);
    //attr_num = stoi(line);
    //analyse attribute information
    attr_info = new ATTR[attr_num];
    for (int i = 0; i < attr_num; ++i) {
        getline(fptr, line, ' ');
        attr_info[i].attr_len = stoi(line);
        getline(fptr, line);
        attr_info[i].attr_offset = stoi(line);
    }

    //read the content data
    while (!fptr.eof()){
        fptr >> c;
        data_ += c;
    }

    fptr.close();
}

buffermanager::block::block(int index_, int attr_n, int *attr_len)
    : index(index_),attr_num(attr_n),isFree(false), dirty(false), isFull(false)
{
    //calculate the size
    int space_per_record = sizeof(char);
    for (int i = 0; i < attr_n; ++i) {
        space_per_record += attr_len[i];
    }
    int data_space = space_per_record-1;

    int free_space = MAX_SIZE - sizeof(int)*(2 + attr_n*2);
    //roughly calculating
    size = free_space/space_per_record;
    //??????????????????????????????????
    data_.append(data_space*size, '0');
    //space allocate
    bitmap = new bool[size];
    attr_info = new ATTR[attr_n];
    for (int j = 0; j < size; ++j) {
        bitmap[j] = true;
    }
    int base_index = 0;
    for (int k = 0; k < attr_n; ++k) {
        attr_info[k].attr_len = attr_len[k];
        attr_info[k].attr_offset = base_index;
        base_index += attr_info[k].attr_len*size;
    }
}

buffermanager::buffermanager()
    : File_Manager()
{

}

buffermanager::buffermanager(const std::string& DB_)
    : Database(DB_),File_Manager(DB_)
{

}

void buffermanager::changeDB(const std::string& DB_)
{
    File_Manager.setTable(DB_);
}
void buffermanager::block::show_record(int record_index) const {
    assert(!bitmap[record_index]);

    for (int i = 0; i < attr_num; ++i) {
        cout << get_attr(record_index, i) << " | ";
    }
    cout << endl;
}
string buffermanager::block::get_attr(int record_index, int attr_index) const
{
    assert(!bitmap[record_index]);
    auto pos = (unsigned)(attr_info[attr_index].attr_offset
              + record_index*attr_info[attr_index].attr_len);
    auto len = (unsigned)(attr_info[attr_index].attr_len);
    string ai = data_.substr(pos,len);
    return ai;
}

bool buffermanager::block::insert_record(const string& record)
{
    //find a place to insert
    int free_index = -1;
    for (int i = 0; i < size; ++i) {
        if (bitmap[i]){
            free_index = i;
            bitmap[i] = false;
            break;
        }
    }
    if (free_index == -1) {
        isFull = true;
        return false;
    }
    setDirty();
    for (int j = 0; j < attr_num; ++j) {
        insert_attr(j, free_index, record);
    }
    return true;
}

void buffermanager::block::insert_attr(int attr_index, int record_index, const string& record)
{
    int attr_begin = 0;
    for (int i = 0; i < attr_index; ++i) {
        attr_begin += attr_info[i].attr_len;
    }
    auto begin_pos = (unsigned)(attr_info[attr_index].attr_offset
                         + attr_info[attr_index].attr_len*record_index);
    auto len = (unsigned)attr_info[attr_index].attr_len;
    auto substring = record.substr(attr_begin, attr_info[attr_index].attr_len);
    data_.replace(begin_pos, len, substring);
}

//write the block to the disk
bool buffermanager::block::write2disk(const DBfile& File_manager)
{
    if(!dirty)
        return false;
    string path = File_manager.get_path(index);
    ofstream fptr;
    fptr.open(path);

    fptr << size << endl;
    for (int i = 0; i < size; ++i) {
        if (bitmap[i])
            fptr << '1';
        else
            fptr << '0';
    }
    fptr << endl;
    fptr << attr_num << endl;
    for (int j = 0; j < attr_num; ++j) {
        fptr << attr_info[j].attr_len << ' ' << attr_info[j].attr_offset << endl;
    }
    fptr << data_;

    fptr.close();

    return true;
}

const buffermanager::block& buffermanager::get_block(int index) {
    return get_vBlock(index);
}

buffermanager::block& buffermanager::get_next_vBlock() {
    int index = cur_block->get_index() + 1;
    fstream fptr;
    fptr.open(File_Manager.get_path(index));
    if(!fptr)
        return (*nullblock);
    block& result = get_vBlock(index);
    cur_block = &result;
    assert(cur_block->get_index() == index);
    return result;
}

const buffermanager::block& buffermanager::get_next_block() {
    return get_next_vBlock();
}

void buffermanager::changeTb(const std::string &TB) {
    File_Manager.setTable(TB);

    int max_offset = -MAX_SIZE;
    auto iter = table_offset.begin();
    //if there are some data of the TB in the buffer
    while (iter != table_offset.end()){
        if (iter->first == TB){
            return;
        }
        if(iter->second > max_offset)
            max_offset = iter->second;
    }
    //move from disk to buffer
    table_offset.insert({TB, max_offset+MAX_SIZE});
    for (int i = 0; i < MAX_SIZE; ++i) {
        //if there is no block(i)
        fstream fptr;
        fptr.open(File_Manager.get_path(i), ios::in);
        if (!fptr) {
            fptr.close();
            for (int j = i; j < MAX_SIZE; ++j) {
                Buffer.push_back(nullptr);
            }
            break;
        }
        fptr.close();

        //add to Buffer
        auto bi = new block(i, File_Manager);
        Buffer.push_back(bi);
    }
}

buffermanager::block& buffermanager::get_vBlock(int index) {
    vector<block*>::iterator iter = table_begin(File_Manager.get_table());
    auto end_point = table_end(File_Manager.get_table());
    while ((*iter) && iter!=end_point){
        block* debug = (*iter);
        if (index == (*iter)->get_index()) {
            (*iter)->freq_add();
            cur_block = (*iter);
            return (**iter);
        }
        iter++;
    }
    //LRU
    return (*LRU(table_begin(File_Manager.get_table()), end_point));
}

bool buffermanager::insert(const string& table, const string& record) {
    changeTb(table);
    block& target = get_vBlock(0);
    while (target.is_full()){
        target = get_next_vBlock();
        if (target.get_index() == -1){
            //target =
        }
    }
}

// @return: a no-pt pointer
buffermanager::block* buffermanager::LRU
        (std::vector<block*>::iterator begin, std::vector<block*>::iterator end){
    //any way, just want to reduce the search time
    int ran = rand()/100;
    begin += ran;
    auto min = begin;
    while ((++begin) != end){
        if ((*begin)->get_freq()<(*min)->get_freq())
            min = begin;
    }
    (*min)->write2disk(File_Manager);
    delete (*min);
    return (*min);
}

//table should be exist in the current block vector
std::vector<buffermanager::block*>::iterator
buffermanager::table_begin(const std::string& table){
    auto iter = Buffer.begin();
    int offset = table_offset[table];
    iter += offset;
    return iter;
}

std::vector<buffermanager::block*>::iterator
buffermanager::table_end(const std::string& table){
    auto iter = table_begin(table);
    iter += MAX_SIZE;
    return iter;
}