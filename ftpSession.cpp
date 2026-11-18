#include <string>
#include <iostream>
#include "ftpSession.h"

using namespace std;

ftpSession::ftpSession(TcpSocket& slave, ServerConfig* conf) : Session(slave, conf) {
    val = new ftpSessionVal();
    val->status = STATUS_INIT;
    val->username = "";
    response = "220 FTP Server ready\r\n";
    slave.send(response);
}

ftpSession::~ftpSession() {
    delete val;
}

void ftpSession::reset() {
    val->status = STATUS_INIT;
    val->username = "";
}

void ftpSession::doUnknown(string cmd_argv[], int cmd_argc) {
    response = "500 Unknown command\r\n";
    slave.send(response);
}

void ftpSession::doLogin(string cmd_argv[], int cmd_argc) {
    if(cmd_argc < 4){
        response ="501 Syntax error\r\n";
        slave.send(response);
        return;
    }
    if(val->status == STATUS_LOGGEDIN){
        response = "530 Already logged in\r\n";
        slave.send(response);
        return;
    }
    string username = cmd_argv[2];
    if(conf == nullptr || !conf->isValidUser(username)){
        response ="530 User not found\r\n";
        slave.send(response);
        return;
    }
    string password = cmd_argv[3];
    if(conf == nullptr ||!conf->authenticate(username,password)){
        val->status = STATUS_INIT;
        val->username = "";
        val->password = "";
        response = "530 Login incorrect\r\n";
        slave.send(response);
        return;
    }
    val->username = username;
    val->password = password;
    val->status = STATUS_LOGGEDIN;
    response = "230 Login successful\r\n";
    slave.send(response); 
}
