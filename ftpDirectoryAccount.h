#ifndef FTP_DIRECTORY_ACCOUNT_H_
#define FTP_DIRECTORY_ACCOUNT_H_

#include <string>
using namespace std;

class ftpDirectoryAccount{
    public:
    static string initUserDirectory(const string& username);
    static string getDirectoryList(const string& userRoot);
    static bool isSafePath(const string& subPath);
};
#endif