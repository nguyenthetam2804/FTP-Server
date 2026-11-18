#include <string>
#include <iostream>
#include "ftpSession.h"

using namespace std;

ftpSession::ftpSession(TcpSocket& slave, ServerConfig* conf) : Session(slave, conf) {
    val = new ftpSessionVal();
    val->status = STATUS_INIT;
    val->username = "";
    string welMesg = "220 FTP Server ready\r\n";
    slave.send(welMesg.c_str(),(int)welMesg.length());
}

ftpSession::~ftpSession() {
    delete val;
}

void ftpSession::reset() {
    val->status = STATUS_INIT;
    val->username = "";
}

void ftpSession::doUnknown(string cmd_argv[], int cmd_argc) {
    string mesg = "500 Unknown command\r\n";
    slave.send(mesg.c_str(),(int)mesg.length());
}

void ftpSession::doUser(string cmd_argv[], int cmd_argc) {
    if(cmd_argc < 2){
        string mesg = "501 Syntax error\r\n";
        slave.send(mesg.c_str(),(int)mesg.length());
        return;
    }
    if(val->status == STATUS_LOGGEDIN){
        string mesg = "530 Already logged in\r\n";
        slave.send(mesg.c_str(),(int)mesg.length());
        return;
    }
    string username = cmd_argv[1];
    
    if(conf == nullptr || !conf->isValidUser(username)){
        string mesg = "530 User not found\r\n";
        slave.send(mesg.c_str(),(int)mesg.length());
        return;
    }
    val->username = username;
    cout << "[AUTH] USER " << username << endl;
    string mesg = "331 Username okay\r\n";
    slave.send(mesg.c_str(), (int)mesg.length());
}
void ftpSession::doPass(string cmd_argv[], int cmd_argc){
    cout<<"[AUTH] PASS ******" << endl;
    if(cmd_argc < 2){
        string mesg = "501 Syntax error\r\n";
        slave.send(mesg.c_str(),(int)mesg.length());
        return;
    }
    string password = cmd_argv[1];
    if(conf == nullptr ||!conf->authenticate(val->username,password)){
        val->status = STATUS_INIT;
        val->username = "";
        val->password = "";
        string mesg = "530 Login incorrect\r\n";
        slave.send(mesg.c_str(),(int)mesg.length());
        return;
    }
    val->status = STATUS_LOGGEDIN;
    string mesg = "230 Login successful\r\n";
    slave.send(mesg.c_str(),(int)mesg.length());
}
