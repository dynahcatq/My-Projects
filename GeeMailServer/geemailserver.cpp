#include "GeeMailServer.h"
#include <iostream>
#include <vector>

std::vector <std::string> split(std::string s, std::string delimit)
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

CGeeMailServer::CGeeMailServer()
{
    MailServer = new CNetworkingServer;
    if (sqlite3_open("server_db", &db))
    {
        std::cerr << "Error: Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    ErrorHandle = 0;
}

CGeeMailServer::~CGeeMailServer()
{
    delete MailServer;
    sqlite3_close(db);
}

bool CGeeMailServer::SetupListeningSocket()
{
    if ((ErrorHandle = MailServer->blisten()) != 0)
    {
        std::cerr << "listening error: " << ErrorHandle << std::endl;
        return false;
    }
    else
        return true;
}

bool CGeeMailServer::WaitingConnection()
{
    if ((ErrorHandle = MailServer->baccept()) != 0)
    {
        std::cerr << "accepting socket error: " << ErrorHandle << std::endl;
        return false;
    }
    else
        return true;
}

void CGeeMailServer::ReceiveAndParseRequest()
{
    ClientRequest.clear();
    ClientRequest = MailServer->receive(500);
    std::string username;

    if (ClientRequest.empty())
        return ;

    if (ClientRequest.find("GETMAIL") == 0)
    {
        username = ClientRequest.substr(std::string("GETMAIL\a\a").size());
        ClientGetMail(username);
    }
    else if (ClientRequest.find("SENDMAIL") == 0)
    {
        username = ClientRequest.substr(std::string("SENDMAIL\a\a").size());
        ClientSendMail(username);
    }
    else if (ClientRequest.find("REGISTER") == 0)
    {
        ClientRegister();
    }
    else if (ClientRequest.find("LOGIN") == 0)
    {
        ClientLogin();
    }
}

void CGeeMailServer::ClientRegister()
{
    MailServer->send("OKSERVEREND\a\a");
    std::string strRegisterInfo;
    strRegisterInfo = MailServer->receive(500);

    std::size_t posUsernameReg = strRegisterInfo.find("UsernameReg: ");
    std::size_t posPasswordReg = strRegisterInfo.find("PasswordReg: ");

    std::string UsernameReg = strRegisterInfo.substr(posUsernameReg + std::string("UsernameReg: ").size(), posPasswordReg - posUsernameReg - std::string("UsernameReg: ").size());
    std::string PasswordReg = strRegisterInfo.substr(posPasswordReg + std::string("PasswordReg: ").size());

    sqlite3_stmt *statement;
    std::string query = std::string("insert into account (account_ID, username, password) values (1, ") + "\"" + UsernameReg + "\", " + "\"" + PasswordReg + "\")";
    std::cout << query << std::endl;

    int sqlite_error = 0;
    if ((sqlite_error = sqlite3_prepare(db, query.c_str(), -1, &statement, 0)) == SQLITE_OK)
    {
        sqlite_error = sqlite3_step(statement);
        if (sqlite_error == SQLITE_ERROR)
        {
            std::cerr << "Error: sql execute query error: " << sqlite_error << std::endl;
            MailServer->send("NAME_DUPLICATEDSERVEREND\a\a");
            return;
        }
        sqlite3_finalize(statement);
        MailServer->send("OKSERVEREND\a\a");
        return;
    }
    else
    {
        std::cerr << "Error: sql prepare query error: " << sqlite_error << std::endl;
        MailServer->send("NAME_DUPLICATEDSERVEREND\a\a");
        return;
    }
}

void CGeeMailServer::ClientLogin()
{
    MailServer->send("OKSERVEREND\a\a");
    std::string strLoginInfo;
    strLoginInfo = MailServer->receive(500);

    std::size_t posUsername = strLoginInfo.find("Username: ");
    std::size_t posPassword = strLoginInfo.find("Password: ");

    std::string Username = strLoginInfo.substr(posUsername + std::string("Username: ").size(), posPassword - posUsername - std::string("Username: ").size());
    std::string Password = strLoginInfo.substr(posPassword + std::string("Password: ").size());

    sqlite3_stmt *statement;
    std::string query = std::string("select account_ID from account where username = ") + "\"" + Username + "\" and password = " + "\"" + Password + "\"";
    std::cout << query << std::endl;
    int sqlite_error = 0;

    if ((sqlite_error = sqlite3_prepare(db, query.c_str(), -1, &statement, 0)) == SQLITE_OK)
    {
        sqlite_error = sqlite3_step(statement);
        if (sqlite_error != SQLITE_ROW)
            // login failed
        {
            std::cerr << "Error: sql execute query error: " << sqlite_error << std::endl;
            MailServer->send("ACCOUNT_INFO_ERRORSERVEREND\a\a");
            return;
        }
        else
        {
            MailServer->send("LOGIN_SUCCEEDSERVEREND\a\a");
            return;
        }
    }
    else
    {
        std::cerr << "Error: sql prepare query error: " << sqlite_error << std::endl;
        MailServer->send("ACCOUNT_INFO_ERRORSERVEREND\a\a");
        return;
    }
}

void CGeeMailServer::ClientGetMail(std::string username)
{
    std::vector <std::string> mail_row;
    sqlite3_stmt *statement;
    std::string query = "select * from mail where receiver = \"" + username + "\"";
    int sqlite_error = 0;

    if ((sqlite_error = sqlite3_prepare(db, query.c_str(), -1, &statement, 0)) == SQLITE_OK)
    {
        int iTotalColumn = sqlite3_column_count(statement);
        int iResult = 0;

        while (1)
        {
            iResult = sqlite3_step(statement);

            if (iResult == SQLITE_ROW)
            {
                std::string str_row;
                for (int i = 0; i < iTotalColumn; ++i)
                {
                    std::string str = (char*) sqlite3_column_text(statement, i);
                    str_row += (str + '\a');
                }
                str_row = "ROW\t" + str_row;
                std::cout << str_row << iTotalColumn;
                mail_row.push_back(str_row);

                if (MailServer->send(str_row) != 0)
                    std::cerr << "Error: Server send mail error!" << std::endl;
            }

            if (iResult == SQLITE_DONE || iResult == SQLITE_ERROR)
                break;
        }
        if (MailServer->send("SERVEREND\a\a") != 0)
            std::cerr << "Error: Server send mail error!" << std::endl;
    }
    else
        std::cerr << "Error: sql prepare query error: " << sqlite_error << std::endl;
}

void CGeeMailServer::ClientSendMail(std::string username)
{
    MailServer->send("OKSERVEREND\a\a");
    std::string MailFromClient;
    MailFromClient = MailServer->receive(500);

    std::size_t posRecipient = MailFromClient.find("Recipient: ");
    std::size_t posTitle = MailFromClient.find("Title: ");
    std::size_t posBody = MailFromClient.find("Body: ");

    std::string Recipient = MailFromClient.substr(posRecipient + std::string("Recipient: ").size(), posTitle - posRecipient - std::string("Recipient: ").size());
    std::string Title = MailFromClient.substr(posTitle + std::string("Title: ").size(), posBody - posTitle - std::string("Title: ").size());
    std::string Body = MailFromClient.substr(posBody + std::string("Body: ").size());
    sqlite3_stmt *statement;
    std::string query = std::string("insert into mail (mail_ID, account_ID, receiver, sender, title, body, timestamp, read) values (1, 1, ") + "\"" + Recipient + "\"," + "\"" + username + "\"," + "\"" + Title + "\"," + "\"" + Body + "\", 0, 0)";
    std::cout << query << std::endl;

    int sqlite_error = 0;
    if ((sqlite_error = sqlite3_prepare(db, query.c_str(), -1, &statement, 0)) == SQLITE_OK)
    {
        sqlite_error = sqlite3_step(statement);
        sqlite3_finalize(statement);
    }
    else
        std::cerr << "Error: sql prepare query error: " << sqlite_error << std::endl;
}
