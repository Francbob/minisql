#include <iostream>
#include "../recordmanager.h"
using namespace std;

int main(){
//    buffermanager bm;
//    bm.changeDB("DB");
//    bm.changeTB("test3");
//    bm.read_disk();
    recordmanager rm;
//    vector<Attribute> vector_Attribute;
////    Attribute ai("a",0,4,0);
////    Attribute bi("b",4,2,0);
////    Attribute ci("c",6,3,0);
////    Attribute di("d",8,4,0);
////    vector_Attribute.push_back(ai);
////    vector_Attribute.push_back(bi);
////    vector_Attribute.push_back(ci);
////    vector_Attribute.push_back(di);
    //对于从来没有建立过的表需要先create,传入的格式是 string 表格名， vector<Attribute>向量
    //如果表格已经建立过，用这个函数：bool changeTb(const std::string& TB);
    //对于已经建立过的，会报错
    int a0 = 1111;
    int b0 = 00;
    int c0 = 111;
    int d0 = 1000;
    rm.changeDB("DB");
    rm.changeTB("test3");
       for (int i = 0; i < 10; ++i) {

        rm.insert(to_string(a0+i) + to_string(b0 ) + to_string(c0 + i*10)+to_string(d0));
    }
//    for (int j = 0; j < bi.get_attr_num(); ++j) {
//        cout << "Attr " << j << ":";
//        for (int i = 0; i < 10; ++i) {
//            cout << bi.get_attr(i,j) << " ";
//        }
//        cout << endl;
//    }
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 4; ++k) {
                cout<<rm.find_attr(1, j, k) << endl;
            }
        }
    return 0;
}



//
// Created by bob on 2018/6/24.
//

