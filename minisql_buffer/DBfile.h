//
// Created by bob on 2018/6/13.
//

#ifndef MINISQL_DBFILE_H
#define MINISQL_DBFILE_H

#include <fstream>
#include <direct.h>
#include <io.h>
#include <string>
using namespace std;
class DBfile {
public:
    DBfile() = default;
    explicit DBfile(const string& DB);
    //ofstream& open(int index, char mode);
    void setDB(const string& DB_) { Database = DB_; };
    void setTable(const string& T_) {
        table = T_;
        const char* path = get_path().c_str();
        if(access(path, F_OK))
            mkdir(path);
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
