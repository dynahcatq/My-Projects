#ifndef RSA_H
#define RSA_H

#include <string>

class RSA
{
public:
    RSA();
    RSA(std::string Msg);
private:
    std::string EncodedMsg;
};

#endif // RSA_H
