#include "gee_login.h"

Gee_Login::Gee_Login()
{
    ClientLogin = new CNetworkingClient;
}

Gee_Login::~Gee_Login()
{
    delete ClientLogin;
}

LoginMsg Gee_Login::login(std::string username, std::string password)
{
    if (username.empty())
        return LoginMsg::USERNAME_REQUIRED;
    if (password.empty())
        return LoginMsg::PASSWORD_REQUIRED;

    std::string strRequest = "LOGIN\a\aCLIENTEND\a\a";
    // send request to server
    if (ClientLogin->connect() != 0)
    {
        return LoginMsg::LOGIN_FAILED;
    }
    ClientLogin->send(strRequest);

    // receive confirmation from server
    std::string strReceive;
    strReceive = ClientLogin->receive(500);
    if (strReceive == "OK")
    {
        std::string strRegisterInfo;
        strRegisterInfo += "Username: " + username + "Password: " + password + "CLIENTEND\a\a";
        ClientLogin->send(strRegisterInfo);

        // check if request premit
        strReceive = ClientLogin->receive(500);
        if (strReceive != "LOGIN_SUCCEED")
        {
            ClientLogin->close();
            return LoginMsg::LOGIN_FAILED;
        }
        else
        {
            ClientLogin->close();
            return LoginMsg::LOGIN_SUCCEED;
        }
    }
    else
    {
        return LoginMsg::LOGIN_FAILED;
    }
}

RegisterMsg Gee_Login::reg(std::string username_reg, std::string password_reg)
{
    if (username_reg.empty())
        return RegisterMsg::USERNAME_REQUIRED;
    if (password_reg.empty())
        return RegisterMsg::PASSWORD_REQUIRED;

    std::string strRequest = "REGISTER\a\aCLIENTEND\a\a";
    // send request to server
    if (ClientLogin->connect() != 0)
    {
        return RegisterMsg::REGISTER_NAME_EXIST;
    }
    ClientLogin->send(strRequest);

    // receive confirmation from server
    std::string strReceive;
    strReceive = ClientLogin->receive(500);
    if (strReceive == "OK")
    {
        std::string strRegisterInfo;
        strRegisterInfo += "UsernameReg: " + username_reg + "PasswordReg: " + password_reg + "CLIENTEND\a\a";
        ClientLogin->send(strRegisterInfo);

        // check if request premit
        strReceive = ClientLogin->receive(500);
        if (strReceive != "OK")
        {
            ClientLogin->close();
            return RegisterMsg::REGISTER_NAME_EXIST;
        }
        else
        {
            ClientLogin->close();
            return RegisterMsg::REGISTER_SUCCEED;
        }
    }
    else
    {
        return RegisterMsg::REGISTER_NAME_EXIST;
    }
}
