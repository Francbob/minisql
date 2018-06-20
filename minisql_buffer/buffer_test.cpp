#include <iostream>
#include "buffermanager.h"

using namespace std;

int main(){
    buffermanager buffer("DB");
    const buffermanager::block ai = buffer.get_block("test",0);

    for (int i = 0; i < ai.get_size(); ++i) {
        if (ai.check_record(i))
            ai.show_record(i);
    }
    return 0;
}

