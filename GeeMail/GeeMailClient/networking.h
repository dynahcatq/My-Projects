#ifndef NETWORKING_H
#define NETWORKING_H


#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>

class CNetworkingServer
{
public:
    CNetworkingServer();
    int blisten();
    int baccept();
    std::string receive(const int MAX_BUFF_LENGTH);
    int send(std::string SendMsg);

private:
    int isock, iaddrinfo, iaccepted_sock;
    std::string strport_no;
    addrinfo hints, *server;
    sockaddr client_addr;
    socklen_t addr_size;
};

class CNetworkingClient
{
public:
    CNetworkingClient();
    int connect();
    int send(std::string SendMsg);
    std::string receive(const int MAX_BUFF_LENGTH);
    void close() { ::close(isock); }

private:
    int isock;
    addrinfo hints, *server_info;
    std::string ServerIPAddress, strport_no;
};


#endif // NETWORKING_H
