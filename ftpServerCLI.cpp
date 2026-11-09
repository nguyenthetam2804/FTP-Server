#include <iostream>
#include "ftpServerCLI.h"
#include "ftptcpserver.h"
using namespace std;

ftpServerCLI::ftpServerCLI():CmdLineInterface("Ftp>"){
    ftpServer = new FtpTcpServer();
    initCmd();
}
ftpServerCLI::~ftpServerCLI(){
}
void ftpServerCLI::initCmd(){
    addCmd("start",CLI_CAST(&ftpServerCLI::doStart));
    addCmd("stop",CLI_CAST(&ftpServerCLI::doStop));
    addCmd("status",CLI_CAST(&ftpServerCLI::doStatus));
    addCmd("quit",CLI_CAST(&ftpServerCLI::doQuit));
    addCmd("help",CLI_CAST(&ftpServerCLI::doHelp));
}
void ftpServerCLI::doStart(string cmd_argv[], int cmd_argc){
    if(cmd_argc == 1){
        if(ftpServer->isRunning()){
            cout << "Ftp Server is running \n";
        }else{
            if(ftpServer->start()){
                cout << "Ftp Server started running \n";
            }else{
                cout << "Ftp Server failed to start \n";
            }
        }
    }else{
        cout<<"Invalid Command \n";
    }
}
void ftpServerCLI::doStop(string cmd_argv[], int cmd_argc){
    if(cmd_argc == 1){
        ftpServer->stop();
        cout<<"Ftp Server stopped \n";
    }else{
        cout<<"Invalid Command \n";
    }
}
void ftpServerCLI::doStatus(string cmd_argv[], int cmd_argc){
    if(cmd_argc == 1){
        if(ftpServer->isRunning()){
            cout<<"Ftp Server is running \n";
        }else{
            cout<<"Ftp Server is not running \n";
        }
    }else{
        cout<<"Invalid Command \n";
    }
}
void ftpServerCLI::doHelp(string cmd_argv[], int cmd_argc){
    if(cmd_argc == 1 ){
        cout<<"1.Start \n"
            <<"2.Stop \n"
            <<"3.Status \n"
            <<"4.Quit \n"
            <<"5.Help \n";
    }
}
