#ifndef FTPSERVERCONFIG_H_
#define FTPSERVERCONFIG_H_

#include "severSocket/serverconfig.h"

class ftpServerConfig : public ServerConfig {
    public:
        ftpServerConfig();
        ~ftpServerConfig();
        bool loadAccountsFromFile(const string& filename) override;
};
#endif // FTPSERVERCONFIG_H_