#include "gee_main.h"
#include <iostream>
#include <QMessageBox>

Gee_Main::Gee_Main()
{
    MailClient = new CNetworkingClient;
}

Gee_Main::~Gee_Main()
{
    delete MailClient;
}

std::vector <std::string> Gee_Main::split(std::string s, std::string delimit)
{
    std::vector <std::string> vector_word;
    std::size_t pos;

    while ((pos = s.find(delimit)) != std::string::npos)
    {
        std::string reststring = s.substr(pos);
        std::string substring = s.substr(0, (s.size() - reststring.size()));
        if (!substring.empty())
            vector_word.push_back(substring);

        s = reststring.substr(delimit.size());
    }
    if (!s.empty())
        vector_word.push_back(s);

    return vector_word;
}

std::vector <std::string> Gee_Main::GetMail(std::string username)
{
    std::vector <std::string> MailList;
    std::string strRequest = "GETMAIL\a\a" + username + "CLIENTEND\a\a";
    // send request to server
    if (MailClient->connect() != 0)
    {
        return MailList;
    }
    MailClient->send(strRequest);

    // receive mail from server
    std::string strReceive;
    strReceive = MailClient->receive(500);

    MailList = split(strReceive, "ROW\t");
    // close connection
    MailClient->close();

    return MailList;
}

void Gee_Main::SendMail(std::string username, std::string Recipient, std::string Title, std::string BodyMessage)
{
    std::string strRequest = "SENDMAIL\a\a" + username + "CLIENTEND\a\a";
    QMessageBox SendMailErrMsg;
    // send request to server
    if (MailClient->connect() != 0)
    {
        //std::cerr << "Error: Client connect to server error!" << std::endl;
        SendMailErrMsg.setText("Error: Client connect to server error!");
        SendMailErrMsg.exec();
        return;
    }
    MailClient->send(strRequest);

    // receive confirmation from server
    std::string strReceive;
    strReceive = MailClient->receive(500);
    if (strReceive == "OK")
    {
        std::string strMail;
        strMail += "Recipient: " + Recipient + "Title: " + Title + "Body: " + BodyMessage + "CLIENTEND\a\a";
        MailClient->send(strMail);
        MailClient->close();
        return;
    }
    else
    {
        SendMailErrMsg.setText("Send Mail Error!");
        SendMailErrMsg.exec();
        return;
    }
}
