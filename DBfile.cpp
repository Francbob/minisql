//
// Created by bob on 2018/6/13.
//

#include "DBfile.h"

DBfile::DBfile(const string& DB)
    : Database(DB)
{

}

string DBfile::get_path(int index) const{
    string path(Database + "\\" + table + "\\" + to_string(index) + ".db");
    return path;
}

string DBfile::get_index_path() const {
    string path(Database + "\\" + table + "\\index");
    return path;
}

string DBfile::get_index_path(int index) const {
    return get_index_path() + "\\" + to_string(index) + ".db";
}

string DBfile::get_path() const{
    string path(Database + "\\" +table);
    return path;
}
