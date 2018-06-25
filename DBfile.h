//
// Created by bob on 2018/6/13.
//

#ifndef MINISQL_DBFILE_H
#define MINISQL_DBFILE_H

#include <fstream>
#include <direct.h>
#include <io.h>
#include <string>

#include <Windows.h>                 //Windows API   FindFirstFile  
#include <Shlwapi.h>  
#pragma comment(lib, "shlwapi.lib")  //Windows API   PathFileExists 

using namespace std;
class DBfile {
public:
    DBfile() = default;
    explicit DBfile(const string& DB);
    //ofstream& open(int index, char mode);
    void setDB(const string& DB_) { Database = DB_; };
    bool setTable(const string& T_) {
        table = T_;
        const char* path = get_path().c_str();
        if(PathFileExists(path)){
            _mkdir(path);
            return false;
        }
        return true;
    };
    string get_table() const { return table; }
    string get_path(int index) const;
    string get_path() const;
    string get_index_path() const;
    string get_index_path(int index) const;
private:
    string Database;
    string table;
};


#endif //MINISQL_DBFILE_H
