//
// Created by bob on 2018/6/20.
//

#ifndef MINISQL_BPLUSTREE_H
#define MINISQL_BPLUSTREE_H

#include "buffermanager.h"
#include "DBfile.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
static const int ATTR_INDEX = 0;
static const int ATTR_OFFSET = 1;
static const int ATTR_KEY = 3;

class Node;
class Leave;

class BplusTree {
public:
    BplusTree() = default;
    ~BplusTree() = default;
    bool Delete(const string& key);
    bool Insert(const string& key, int key_size, int index, int offset);
    map<int, int> Find(const string& key);
    const string& get_attr() const { return attribute; }
    const string& get_table() const { return Table; }
private:
    string attribute;
    string Table;
    Node* Root;
};

class Node : public buffermanager::block{
public:

};

class Leave : public Node{

};


#endif //MINISQL_BPLUSTREE_H
