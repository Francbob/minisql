//
// Created by bob on 2018/6/23.
//

#ifndef MINISQL_BUFFERFORINDEX_H
#define MINISQL_BUFFERFORINDEX_H

#include <string>
#include <vector>
#include "DBfile.h"
#include "test/testCM.h"

using namespace std;
class buffermanager {
public:
    buffermanager() = default;
    string get_block(int index);
    string get_next_block(int index);
    int find_free(){
        for (int i = 0; i < Buffer.fullMap.size(); ++i) {
            if(!Buffer.fullMap[i])
                return Buffer.name[i];
        }
        return 0;
    }
    int find_name(int name);
    int insert_block(int index, const string& block);
    int insert_block(string block);
    int insert_piece(string piece);
    int read_disk();
    bool write2disk(int index);
    bool Allwrite2disk();
    bool changeTB(string TB);
    bool changeDB(string DB);
    string get_table() const{ return table; }
private:
    struct B{
        vector<int> name;
        vector<int> frequency;
        vector<string> buffer;
        vector<bool> blockMap;
        vector<bool> fullMap;
        vector<int> dirty;
    } Buffer;
    string table;
    string database;
    DBfile File_manager;
    int LRU();
    string get_e_block(int index){
        ifstream fptr;
        string result;
        auto path = File_manager.get_path(index);
        fptr.open(path);
        string line;
        while (!fptr.eof()){
            getline(fptr,line,' ');
            result += line;
            result += ' ';
        }

        return result;
    }
};


#endif //MINISQL_BUFFERFORINDEX_H
