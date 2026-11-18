#include "ftptcpserver.h"
#include "ftpServerConfig.h"
#include "ftpSession.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

FtpTcpServer::FtpTcpServer(unsigned short port):TCPServer(port){
    this -> conf = new ftpServerConfig();
    this-> conf -> loadAccountsFromFile("ftpAccount.conf");
    initCmd();
}
FtpTcpServer::~FtpTcpServer(){
    delete conf;
}
void FtpTcpServer::initCmd(){
    addCmd("USER",FUNC_CAST(&ftpSession::doUser));
    addCmd("PASS",FUNC_CAST(&ftpSession::doPass));
}
int FtpTcpServer::readCmd(TcpSocket& slave, string& cmdLine){
    try{
        char cmdBuffer[SERVER_CMD_BUF_LEN];
        int byteRead = slave.recvLine(cmdBuffer, SERVER_CMD_BUF_LEN);
        if(byteRead >0){
            while (byteRead > 0 && (cmdBuffer[byteRead-1] == '\r' || cmdBuffer[byteRead-1] == '\n')){
                cmdBuffer[--byteRead] = '\0';
            }
            cmdLine=cmdBuffer;
        }
        return byteRead;
    }
    catch(SocketException& e){
        cerr<<e.what()<<endl;
        return -1;
    }
}
unsigned short FtpTcpServer::parseCmd(const string& cmdLine, string cmd_argv[], int& cmd_argc){
    cmd_argc =0;
    istringstream iss(cmdLine); //tương tự với strtok
    while (cmd_argc < SERVER_CMD_ARG_NUM && iss >> cmd_argv[cmd_argc]){
        cmd_argc++;
    } // >> đọc hết thì trả về false
    if(cmd_argc > 0 && !cmd_argv[0].empty()){
        for(int i =0; i< numCmd; i++){
            if(cmd_argv[0] == cmdNameList[i]){
                return i;
            }
        }
    }
    return SERVER_CMD_UNKNOWN;

}
void FtpTcpServer::startNewSession(TcpSocket slave){
    cout<<"[CONNECT] Client connected:"<<slave.getRemoteAddress()<<":"<<slave.getRemotePort()<<endl;
    ftpSession session(slave, this->conf);
    string cmdLine;
    string cmd_argv[SERVER_CMD_ARG_NUM];
    int cmd_argc =0;
    while(!session.isQuit()){
        int byteRead = readCmd(slave, cmdLine);
        if(byteRead <=0){
            break;
        }
        if(cmdLine.empty()){
            continue;
        }
    unsigned short cmdId = parseCmd(cmdLine, cmd_argv, cmd_argc);
    doCmd(&session, cmdId, cmd_argv, cmd_argc);
    }
    slave.close();
    cout<<"Client disconnected"<<endl;
}

