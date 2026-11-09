#include "severSocket/cli.h"
#include "ftptcpserver.h"
class ftpServerCLI : public CmdLineInterface{
    private:
    FtpTcpServer* ftpServer;
    public:
    ftpServerCLI();
    ~ftpServerCLI();
    protected:
    void initCmd();
    private:
    void doStart(string cmd_argv[], int cmd_ardc);
    void doStop(string cmd_argv[], int cmd_ardc);
    void doStatus(string cmd_argv[], int cmd_ardc);
    void doHelp(string cmd_argv[], int cmd_ardc);
};