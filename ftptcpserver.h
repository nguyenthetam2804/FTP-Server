#ifndef _FTP_TCP_SERVER_H_
#define _FTP_TCP_SERVER_H_
#include "severSocket/tcpserver.h"
class FtpTcpServer : public TCPServer{
    public:
    FtpTcpServer(unsigned short port = 21);
    ~FtpTcpServer();
    protected:
    void startNewSession(TcpSocket slave) override;
    void initCmd() override;
    int doFtp(TcpSocket& slave);
    private:
    int readCmd(TcpSocket& slave, string& cmdLine);
    unsigned short parseCmd(const string& cmdLine, string cmd_argv[], int& cmd_argc);
};
#endif