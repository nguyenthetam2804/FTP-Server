#include <string>
#include <iostream>
#include "ftpSession.h"
#include "ftpDirectoryAccount.h"
#include <algorithm>

using namespace std;

ftpSession::ftpSession(TcpSocket& slave, ServerConfig* conf) : Session(slave, conf) {
    currentUserRoot = "";
    dataMaster = nullptr;
    isPasvMode = false;
    val = new ftpSessionVal();
    val->status = STATUS_INIT;
    val->username = "";
    string welMesg = "220 FTP Server ready\r\n";
    slave.send(welMesg.c_str(),(int)welMesg.length());
}

ftpSession::~ftpSession() {
    delete val;
    if(dataMaster != nullptr){
        dataMaster->close();
        delete dataMaster;
        dataMaster = nullptr;
    }
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
    this->currentUserRoot = ftpDirectoryAccount::initUserDirectory(val->username);
    string mesg = "230 Login successful\r\n";
    slave.send(mesg.c_str(),(int)mesg.length());
}
void ftpSession::doPasv(string cmd_argv[], int cmd_argc){
    if(val->status != STATUS_LOGGEDIN){
        string mesg = "530 Please login first\r\n";
        slave.send(mesg.c_str(), (int)mesg.length());
        return;
    }

    if (dataMaster != nullptr) {
        dataMaster->close();
        delete dataMaster;
        dataMaster = nullptr;
    }

    try{
        dataMaster = new TcpServerSocket();
        dataMaster->setListen(0);
        unsigned short dataPort = dataMaster->getLocalPort();
        int p1 = dataPort / 256;
        int p2 = dataPort % 256;
        string ip = slave.getLocalAddress();
        replace(ip.begin(), ip.end(), '.', ',');
        string mesg = "227 Entering Passive Mode (" + ip + "," + to_string(p1) + "," + to_string(p2) + ")\r\n";
        cout<<"PASV"<<mesg;
        isPasvMode = true;
        slave.send(mesg.c_str(), (int)mesg.length());
    }catch (SocketException& e) {
        cerr << "[ERROR] doPasv: " << e.what() << endl;
        isPasvMode = false;
        string mesg = "425 Can't open passive connection\r\n";
        slave.send(mesg.c_str(), (int)mesg.length());
    }
}

    void ftpSession::doList(string cmd_argv[], int cmd_argc){
            if (val->status != STATUS_LOGGEDIN) {
        string mesg = "530 Please login first\r\n";
        slave.send(mesg.c_str(), (int)mesg.length());
        return;
    }
    if (!isPasvMode || dataMaster == nullptr) {
        string mesg = "425 Use PASV first\r\n";
        slave.send(mesg.c_str(), (int)mesg.length());
        return;
    }

    try {
        string msg150 = "150 Opening data connection\r\n";
        slave.send(msg150.c_str(), (int)msg150.length());

        TcpSocket dataSlave = dataMaster->accept();
        cout << "[LIST] Data connection established" << endl;

        string dirList = ftpDirectoryAccount::getDirectoryList(this->currentUserRoot);
        if (!dirList.empty()) {
            dataSlave.send(dirList.c_str(), (int)dirList.length());
        }

        dataSlave.close();
        dataMaster->close();
        delete dataMaster;
        dataMaster = nullptr;
        isPasvMode = false;

        string msg226 = "226 Transfer complete\r\n";
        slave.send(msg226.c_str(), (int)msg226.length());
        cout << "[LIST] 226 Transfer complete" << endl;
    } catch (SocketException& e) {
        cerr << "[ERROR] doList: " << e.what() << endl;
        string msg426 = "426 Connection closed; transfer aborted\r\n";
        slave.send(msg426.c_str(), (int)msg426.length());
    }
    }
