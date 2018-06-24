//
// Created by bob on 2018/6/24.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "block.h"
#include "buffermanager.h"
#include "Attribute.h"


class recordmanager {
private:
    buffermanager bm;
public:
    void changeDB(const string& a){bm.changeDB(a);}
    void changeTB(const string& a);
    recordmanager() = default;
    ~recordmanager(){
        bm.Allwrite2disk();
    }
    bool create(const std::string &table,
             vector<Attribute> vector_Attribute);
    string find_attr(int block_index, int record_index, int attr_index);
    int insert(const string& record);

};


#endif //MINISQL_RECORDMANAGER_H
