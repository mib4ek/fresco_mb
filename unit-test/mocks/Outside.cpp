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

int scanfUT(const char *format, char* buf)
{
    return mock->scanfUT(format, buf);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen )
{
    return mock->connect(sockfd, addr, addrlen);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
    return mock->send(sockfd, buf, len, flags);
}

sighandler_t signal(int signum, sighandler_t handler)
{
    return mock->signal(signum, handler );

}