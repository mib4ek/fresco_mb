#include "Outside.h"

static MockOutside * mock;

void outside_setMock(MockOutside &out_mock){
	mock = &out_mock;
}

int socket(int domain, int type, int protocol)
{
    return mock->socket(domain,type,protocol);
}

int listen(int sockfd, int backlog)
{
    return mock->listen(sockfd,backlog);
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return mock->bind(sockfd, addr, addrlen);
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return mock->accept(sockfd, addr, addrlen);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
    return mock->recv(sockfd, buf, len, flags);
}