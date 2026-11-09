#ifndef _FTP_TCP_SERVER_H_
#define _FTP_TCP_SERVER_H_
#include "tcpserver.h"
class FtpTcpServer : public TCPServer{
    public:
    FtpTcpServer(unsigned short port = 21);
    ~FtpTcpServer();
    protected:
    void startNewSession(TcpSocket slave) override;
    int doFtp(TcpSocket& slave);
};
#endif