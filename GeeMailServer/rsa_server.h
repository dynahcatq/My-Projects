#ifndef RSA_SERVER_H
#define RSA_SERVER_H

#include <string>

class RSA_Server
{
public:
    RSA_Server();
    RSA_Server(std::string Msg);
    std::string EncodeMsg();
private:
    long long prime1, prime2;
    std::pair <long long, long long> PublicKey;
    std::pair <long long, long long> PrivateKey;
    std::string OriginMsg;
    std::string EncodedMsg;

    void setup();
};

#endif // RSA_SERVER_H
