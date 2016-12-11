#include "rsa_server.h"

RSA_Server::RSA_Server()
{

}

RSA_Server::RSA_Server(std::string Msg)
{
    OriginMsg = Msg;
    setup();
}

void RSA_Server::setup()
{
    prime1 = 13;
    prime2 = 29;

    long long N = prime1 * prime2;
    long long fn = (prime1 - 1) * (prime2 - 1);

    long long e = 3;
    long long d;

    for (long long i = 2; ; ++i)
    {
        if ((e * i) % fn == 1)
        {
            d = i;
            break;
        }
        break;
    }

    PublicKey = std::make_pair(N, e);
    PrivateKey = std::make_pair(N, d);
}

std::string RSA_Server::EncodeMsg()
{
    EncodedMsg = OriginMsg.at(0) * PublicKey.first % PublicKey.second;
    return EncodedMsg;
}
