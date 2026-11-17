#include "ftptcpserver.h"
#include <iostream>
#include <string>
using namespace std;

FtpTcpServer::FtpTcpServer(unsigned short port):TCPServer(port){

}
FtpTcpServer::~FtpTcpServer(){}
void FtpTcpServer::startNewSession(TcpSocket slave){
    cout<<"[CONNECT] Client connected:"<<slave.getRemoteAddress()<<":"<<slave.getRemotePort()<<endl;
    string welMesg = "220 Service ready\r\n";
    slave.send(welMesg.c_str(), (int)welMesg.length());
    bool session = true;
    while(session){
        int a = doFtp(slave);
        if(a<=0){
            session=false;
        }
    }
    slave.close();
    cout<<"Client disconnected"<<endl;
}
int FtpTcpServer::doFtp(TcpSocket& slave){
    char buffer[1024];
    int byte_recv;
    try{
        byte_recv = slave.recv(buffer,1024);
    }
    catch(SocketException& e){
        byte_recv=-1;
        cerr<<e.what()<<"in doFtp()"<<endl;
    }
    return byte_recv;
}

