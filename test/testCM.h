//
// Created by bob on 2018/6/23.
//

#ifndef MINISQL_TESTCM_H
#define MINISQL_TESTCM_H

#include <fstream>
#include <string>
using namespace std;

class testCM {
public:
    testCM(){
        fstream fptr;
        fptr.open("testCM");
        string get;
        getline(fptr,get);
        block_num = stoi(get);
        fptr.close();
    }
    void add() {block_num++;}
    int get() { return block_num; }
    ~testCM() {
        ofstream fptr;
        fptr.open("testCM");
        fptr << block_num;
        fptr.close();
    }
private:
    int block_num;
};


#endif //MINISQL_TESTCM_H
