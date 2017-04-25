#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "ShareMemory.h"
#include "ShmQueue.h"

using namespace std;



int main(){
    std::cout << "hello worker" << std::endl;

    CShareMemory shareMemory;
    shareMemory.createShareMem();

    SBufferNode *node = NULL;
    while(1){
        node = shareMemory.workerRecvData();
        if(node != NULL){
            cout << node->dataLen << "\t" << node->data << endl;
        }

    }
    shareMemory.deleteShareMem();
    return 0;
}
