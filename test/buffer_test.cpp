#include <iostream>
#include "../buffermanager.h"
#include "../Attribute.h"
#include <string>

using namespace std;

//int main(){
//    buffermanager buffer("DB");
//    vector<Attribute> vector_Attribute;
//    Attribute ai("a",0,4,0);
//    Attribute bi("b",4,2,0);
//    Attribute ci("c",6,3,0);
//    Attribute di("d",8,4,0);
//    vector_Attribute.push_back(ai);
//    vector_Attribute.push_back(bi);
//    vector_Attribute.push_back(ci);
//    vector_Attribute.push_back(di);
//    //对于从来没有建立过的表需要先create,传入的格式是 string 表格名， vector<Attribute>向量
//    //如果表格已经建立过，用这个函数：bool changeTb(const std::string& TB);
//    //对于已经建立过的，会报错
//    buffer.create("test3", vector_Attribute);
//    for (int i = 0; i < 1000; ++i) {
//        int a = 1234+i;
//        string b = "ab";
//        int c = i;
//        string d = "abcd";
//        //建立好后就可以插入数据，格式是把所有的属性的值按顺序拼起来成一个字符串
//        buffer.insert(to_string(a) + b + to_string(c) + d);
//    }
//    for (int j = 0; j < 4; ++j) {
//        //block的主要操作：string get_attr(int record_index, int attr_index) const;
//                //void show_record(int record_index) const;
//        //获取数据，格式是传入block的编号，编号的管理应该从catalog获取
//        buffer.get_block(j).show_record(0);
//    }
//    cout << buffer.get_buffer_size() << endl;
//    //强行写入硬盘，这是使用buffer后必须有的
//    buffer.write2disk();
//    return 0;
//}

