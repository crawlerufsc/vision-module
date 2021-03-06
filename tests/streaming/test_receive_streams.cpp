#include "../../log/logger.h"
#include "../../communication/stream_requester_client.h"
#include <iostream>
#include <string>

extern Logger *NewDebugLoggerInstance();

int main (int argc, char **argv) {
    if(argc < 4)
    {
        cerr<<"Syntax : ./client <host> <port> <local_port>"<<endl;
        return 0;
    }
    
    int serverPort = atoi(argv[2]);
    
    if((serverPort > 65535) || (serverPort < 2000))
    {
        cerr<<"Please enter port number between 2000 - 65535"<<endl;
        return 0;
    }       

    int localPort = atoi(argv[3]);
    
    if((localPort > 65535) || (localPort < 2000))
    {
        cerr<<"Please enter local_port number between 2000 - 65535"<<endl;
        return 0;
    }       

    Logger *logger = NewDebugLoggerInstance();
    StreamClient *client = new StreamClient(logger, std::string(argv[1]), serverPort, localPort);
    client->RequestStreamAccess();
}