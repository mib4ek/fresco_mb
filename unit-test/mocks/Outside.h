#ifndef RSTP_MOCK_H
#define RSTP_MOCK_H
#include <gmock/gmock.h>
#include <vector>
#include <sys/socket.h>

class MockOutside;

void outside_setMock(MockOutside &out_mock);
class MockOutside{
public:
	MOCK_METHOD2(listen, int (int, int) );
	MOCK_METHOD3(socket, int (int, int, int) );
	MOCK_METHOD3(bind, int (int, const struct sockaddr*, socklen_t) );
	MOCK_METHOD3(accept, int (int, struct sockaddr*, socklen_t*) );
	MOCK_METHOD4(recv, ssize_t (int, void*,size_t, int) );
};

#endif