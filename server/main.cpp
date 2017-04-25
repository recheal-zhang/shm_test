#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "ShareMemory.h"
#include "ShmQueue.h"

using namespace std;

int main(){
    std::cout << "hello world" << std::endl;

    CShareMemory shareMemory;
    shareMemory.createShareMem();

    int count = 0;
    string tmpStr = "hello";
    while(1){
        SBufferNode tmpNode;
        tmpNode.dataLen = count++;
        if(count == 10){
            count = 0;
        }

        memcpy(tmpNode.data,
                tmpStr.c_str(),
                tmpStr.size());

        cout << tmpNode.dataLen << "\t" << tmpNode.data << endl;
        shareMemory.svrSendData(&tmpNode);

    }

    shareMemory.deleteShareMem();
    shareMemory.ctlDeleteShareMem();
    return 0;
}
