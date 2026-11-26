#ifndef FTPSESSION_H
#define FTPSESSION_H
#include "ftpServerConfig.h"
#include "severSocket/session.h"
#include "severSocket/tcpserversocket.h"
#include <string>
#include <cstdint>

using namespace std;

#define STATUS_INIT 0
#define STATUS_LOGGEDIN 1

struct ftpSessionVal {
    uint8_t status;
    string username;
    string password;
};

class ftpSession : public Session {
private:
    ftpSessionVal* val;
    string currentUserRoot; // Đường dẫn thư mục riêng của user đang đăng nhập
    TcpServerSocket* dataMaster; // Socket lắng nghe kết nối dữ liệu (PASV)
    bool isPasvMode;

public:
    ftpSession(TcpSocket& slave, ServerConfig* conf);
    ~ftpSession();
    void reset() override;
    void doUnknown(string cmd_argv[], int cmd_argc) override;
    void doUser(string cmd_argv[], int cmd_argc);
    void doPass(string cmd_argv[], int cmd_argc);
    void doPasv(string cmd_argv[], int cmd_argc);
    void doList(string cmd_argv[], int cmd_argc);
};

#endif // FTPSESSION_H
