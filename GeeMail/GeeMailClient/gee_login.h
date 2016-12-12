#ifndef GEE_LOGIN_H
#define GEE_LOGIN_H

#include <string>
#include "networking.h"

enum class LoginMsg
{
    USERNAME_REQUIRED,
    PASSWORD_REQUIRED,
    LOGIN_FAILED,
    LOGIN_SUCCEED,
};

enum class RegisterMsg
{
    USERNAME_REQUIRED,
    PASSWORD_REQUIRED,
    REGISTER_SUCCEED,
    REGISTER_NAME_EXIST,
};

class Gee_Login
{
public:
    Gee_Login();
    ~Gee_Login();
    LoginMsg login(std::string username, std::string password);
    RegisterMsg reg(std::string username_reg, std::string password_reg);

private:
    CNetworkingClient * ClientLogin;
};

#endif // GEE_LOGIN_H
