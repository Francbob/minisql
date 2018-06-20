//
// Created by bob on 2018/6/17.
//
#include <iostream>
#include <string>
#include "buffermanager.h"
#include "DBfile.h"
//#define QUANTITY_TEST
using namespace std;

//int main(){
//    int attr_len[3] = {1,5,8};
//    DBfile DB("DB","test");
//    //buffermanager::block bi(0,3,attr_len);
//    buffermanager::block bi(0,DB);
//
//    int a0 = 0;
//    int b0 = 10000;
//    int c0 = 12345678;
//#ifdef QUANTITY_TEST
//    for (int i = 0; i < 10; ++i) {
//        bi.insert_record(to_string(a0+i) + to_string(b0 + i*101) + to_string(c0 + i*10010));
//    }
//    for (int j = 0; j < bi.get_attr_num(); ++j) {
//        cout << "Attr " << j << ":";
//        for (int i = 0; i < 10; ++i) {
//            cout << bi.get_attr(i,j) << " ";
//        }
//        cout << endl;
//    }
//#endif
//    //bi.write2disk(DB);
//
//    return 0;
//}
