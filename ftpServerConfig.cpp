#include "ftpServerConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

ftpServerConfig::ftpServerConfig(){
}

ftpServerConfig::~ftpServerConfig(){
}

bool ftpServerConfig::loadAccountsFromFile(const string& fileName){
    ifstream f(fileName);
    if(!f.is_open()){
        cerr << "Error: Unable to open account file: " << fileName << endl;
        return false;
    }

    string line;
    int accountCount = 0;
    while (getline(f, line)){
        if(line.empty()){
            continue;
        }
        if(line.back() == '\r'){
            line.pop_back();
        }
        istringstream acc(line);
        string user, pass;
        if(acc >> user >> pass){
            Account* newAcc = new Account(user);
            newAcc->setPassword(pass);
            this->addAccount(newAcc);
            accountCount++;
        }
        
    }
    f.close();
    return true;
    
}