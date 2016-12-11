#ifndef GEEMAILSERVER_H
#define GEEMAILSERVER_H

#include "networking.h"
#include <sqlite3.h>
#include <string>

enum class ClientRequestType
{
    ClientRequestError,
    ClientGetMail,
    ClientSendMail,
};

class CGeeMailServer
{
public:
    CGeeMailServer();
    ~CGeeMailServer();
    bool SetupListeningSocket();
    bool WaitingConnection();
    void ReceiveAndParseRequest();
    void ClientLogin();
    void ClientRegister();
    void ClientGetMail(std::string username);
    void ClientSendMail(std::string username);

private:
    CNetworkingServer * MailServer;
    std::string ClientRequest;
    int ErrorHandle;
    sqlite3 *db;
};

#endif // GEEMAILSERVER_H
