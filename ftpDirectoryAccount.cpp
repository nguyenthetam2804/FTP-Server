#include "ftpDirectoryAccount.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

string ftpDirectoryAccount::initUserDirectory(const string& username){
    string userRoot = "./FTP_ROOT/" + username;
    try{
        if(!fs::exists(userRoot)){
            fs::create_directories(userRoot + "/Documents");
            fs::create_directories(userRoot + "/Images");
            fs::create_directories(userRoot + "/Uploads");
            cout<<"Created user root:"<<userRoot<<endl;
        }
    } catch(const exception& e){
        cerr<<"Error initUserDirectionary:"<<e.what()<<endl;
    }
    return userRoot;
}

string ftpDirectoryAccount::getDirectoryList(const string& userRoot){
    string result = "";
    try{
        if(fs::exists(userRoot) && fs::is_directory(userRoot)){
            for(const auto& entry : fs::directory_iterator(userRoot)){
                if(entry.is_directory()){
                    result += entry.path().filename().string() + "/\r\n";
                }
            }
        }
    } catch(const exception& e){
        cerr<< "Error getDirectoryList:"<< e.what() << endl;
    }
    return result;
}

bool ftpDirectoryAccount::isSafePath(const string& subPath){
    return subPath.find("..") == string::npos;
}