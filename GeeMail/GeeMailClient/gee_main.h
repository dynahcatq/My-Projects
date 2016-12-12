#ifndef GEE_MAIN_H
#define GEE_MAIN_H

#include "networking.h"
#include <vector>

class Gee_Main
{
public:
    Gee_Main();
    ~Gee_Main();
    std::vector <std::string> split(std::string s, std::string delimit);
    std::vector <std::string> GetMail(std::string username);
    void SendMail(std::string username, std::string Recipient, std::string Title, std::string BodyMessage);
private:
    CNetworkingClient * MailClient;
};

#endif // GEE_MAIN_H
