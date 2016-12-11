#include "networking.h"
#include "rsa_server.h"
#include <cstring>
#include <vector>

#include <iostream>

CNetworkingServer::CNetworkingServer()
{
    isock = 0;
    iaddrinfo = 0;
    iaccepted_sock = 0;
    strport_no = "7724";

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addr_size = sizeof(client_addr);
}

int CNetworkingServer::blisten()
{
    if ((iaddrinfo = getaddrinfo(NULL, strport_no.c_str(), &hints, &server)) != 0)
    {
        //std::cerr << "Error: get address info error: " << gai_strerror(iaddrinfo) << std::endl;
        return 1;
    }

    auto p = server;
    for (p; p != NULL; p = p->ai_next)
    {
        if ((isock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            //std::perror ("Socket create failed: ");
            continue;
        }

        if (bind(isock, p->ai_addr, p->ai_addrlen) < 0)
        {
            std::perror ("Binding socket failed: ");
            close(isock);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        //std::cerr << "Fail to bind socket." << std::endl;
        return 2;
    }

    listen(isock, 5);
    return 0;
}

int CNetworkingServer::baccept()
{
    if ((iaccepted_sock = accept(isock, &client_addr, &addr_size)) < 0)
    {
        iaccepted_sock = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

std::string CNetworkingServer::receive(const int MAX_BUFF_LENGTH)
{
    int irecv_buff_length = 0;
    std::vector <char> buff(MAX_BUFF_LENGTH);
    std::string RecvMsg;

    std::memset(buff.data(), 0, buff.size());
    while ((irecv_buff_length = recv(iaccepted_sock, buff.data(), buff.size(), 0)) > 0)
    {
        RecvMsg.append(buff.begin(), buff.begin() + irecv_buff_length);
        std::cout.write(buff.data(), irecv_buff_length);
        if (RecvMsg.find("CLIENTEND\a\a") != std::string::npos)
        {
            RecvMsg = RecvMsg.substr(0, (RecvMsg.size() - std::string("CLIENTEND\a\a").size()));
            break;
        }
    }

    return RecvMsg;
}

int CNetworkingServer::send(std::string SendMsg)
{
    if (iaccepted_sock == 0)
        return 1;
    RSA_Server rsa_msg(SendMsg);
    rsa_msg.EncodeMsg();
    ::send(iaccepted_sock, SendMsg.c_str(), SendMsg.size(), 0);
    return 0;
}
