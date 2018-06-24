//
// Created by bob on 2018/6/23.
//

#include <iostream>
#include "../buffermanager.h"
using namespace std;

int main(){
    buffermanager ai;
    ai.changeDB("DB");
    ai.changeTB("test5");
    for (int i = 0; i < 4; ++i) {
        string ne;
        for (int j = 0; j < 100; ++j) {
            ne.append(to_string(j));
        }
        ai.insert_block(ne);
    }


    ai.Allwrite2disk();
    return 0;
}