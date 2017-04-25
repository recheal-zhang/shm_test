#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>


#include <iostream>
#include <string>

#include "ShareMemory.h"
#include "ShmQueue.h"
#include "Define.h"

using namespace std;

void do_epoll(int shmid);

void add_event(int epollfd, int fd, int state);

void modify_event(int epollfd, int fd, int state);

void delete_event(int epollfd, int fd, int state);

void handle_events(int epollfd, struct epoll_event *events,
        int num, int listenfd);


CShareMemory shareMemory;

int main(){
    std::cout << "hello worker" << std::endl;

    shareMemory.createShareMem();

    do_epoll(shareMemory.getShmid());

//    SBufferNode *node = NULL;
//    while(1){
//        node = shareMemory.workerRecvData();
//        if(node != NULL){
//            cout << node->dataLen << "\t" << node->data << endl;
//        }
//
//    }
    shareMemory.deleteShareMem();
    return 0;
}

void do_epoll(int listenfd){
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;

    epollfd = epoll_create(FDSIZE);

    add_event(epollfd, listenfd, EPOLLIN);

    while(true){
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd);
    }

    close(epollfd);
}

void handle_events(int epollfd, struct epoll_event *events,
        int num, int listenfd){
    int i;
    int fd;

    for(i = 0; i < num; i++){
        fd = events[i].data.fd;

        if((fd == listenfd) && (events[i].events & EPOLLIN)){
            SBufferNode *node = shareMemory.workerRecvData();
            if(node != NULL){
                cout << node->dataLen << "\t" << node->data << endl;
            }

        }
    }
}

void add_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void delete_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void modify_event(int epollfd, int fd, int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}



