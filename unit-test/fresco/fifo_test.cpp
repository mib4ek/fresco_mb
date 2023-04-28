#include <gtest/gtest.h>
#include "Outside.h"
#include "fifoServ.h"
#include "terminal.h"
#include "crc16.h"
#define RETURN_ERROR -1

static char Mcu[]="MCU";
static char testValueStr[]="000E0003020B68656C6C6F20776F726C642B48";
static char testValueStrCrcErr[]="000E0003040B68656C6C6F20776F726C643B48";
static char testValueStrU8[]="0003000300122B48";
static char testValueStrI16[]="0003000301F0FF2B48";
void (*testFunc)(int);

using ::testing::Test;
using namespace ::testing;
using ::testing::HasSubstr;

sighandler_t signalTest(int signum, sighandler_t handler);
class Fresco : public ::testing::Test
{
public:
    Fresco()
    {
        outside_setMock(outMock);
        argvTest = new char*[5];
        for (int i =0; i<5;i++)
        {
            argvTest[i] = new char(5);
        }
    }
    ~Fresco()
    {
    }

    void SetUp() override
    {

    }

    void TearDown() override
    {
        delete[] argvTest;
    }

    MockOutside outMock;
    char **argvTest;
};

sighandler_t signalTest(int signum, sighandler_t handler)
{
    testFunc = handler;
    testFunc(SIGSEGV);
    testFunc(SIGTERM);
    testFunc(16);
    testFunc = nullptr;
    return nullptr;
}
static ssize_t recvStrErr(int sockfd, void *buf, size_t len, int flags)
{
    memset(buf,0x00,sizeof(buf));
    memcpy(buf,testValueStrCrcErr,sizeof(testValueStr));

    return 1;
}

static ssize_t recvStr(int sockfd, void *buf, size_t len, int flags)
{
    memset(buf,0x00,sizeof(buf));
    memcpy(buf,testValueStr,sizeof(testValueStr));

    return 1;
}

static ssize_t recvU8(int sockfd, void *buf, size_t len, int flags)
{
    memset(buf,0x00,sizeof(buf));
    memcpy(buf,testValueStrU8,sizeof(testValueStrU8));

    return 1;
}

static ssize_t recvI16(int sockfd, void *buf, size_t len, int flags)
{
    memset(buf,0x00,sizeof(buf));
    memcpy(buf,testValueStrI16,sizeof(testValueStrI16));

    return 1;
}

int inputExit(const char *format, char* buf)
{
    memcpy(buf,"q",2);
    return 1;
}

int inputStr( const char *format, char* buf)
{
    memset (buf,0x00,sizeof(buf));
    memcpy(buf,testValueStr,sizeof(testValueStr));
    return 1;
}


TEST_F(Fresco,bindMode_socketError)
{
    EXPECT_CALL(outMock, socket(_,_,_)).Times(1)
        .WillOnce(Return( RETURN_ERROR ) );

    bindMode(Mcu);
}

TEST_F(Fresco,bindMode_bindError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    bindMode(Mcu);
}

TEST_F(Fresco,bindMode_listenError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    bindMode(Mcu);
}

TEST_F(Fresco,bindMode_acceptError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    bindMode(Mcu);
}

TEST_F(Fresco,parseData_recvError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, recv(_,_,_,0))
        .WillOnce(Return( RETURN_ERROR ) );

    bindMode(Mcu);
}

TEST_F(Fresco,parseData_crcError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, recv(_,_,_,0))
        .WillOnce( recvStrErr );

    bindMode(Mcu);
}

TEST_F(Fresco,parseData_strOK)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, recv(_,_,_,0))
        .WillOnce( recvStr );

    bindMode(Mcu);
}

TEST_F(Fresco,parseData_u8_OK)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, recv(_,_,_,0))
        .WillOnce( recvU8 );

    bindMode(Mcu);
}

TEST_F(Fresco,parseData_i16_OK)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, accept(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, recv(_,_,_,0))
        .WillOnce( recvI16 );

    bindMode(Mcu);
}

TEST_F(Fresco,terminalMenu_argumentsError)
{
    terminalMenu(1,argvTest);
}

TEST_F(Fresco,terminalMenu_argumentsDatarror)
{
    memcpy(argvTest[2],"some",5);

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,terminalMenu_bindMode)
{
    memcpy(argvTest[1],"bind",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,sendMode_inputError)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce(Return( RETURN_ERROR ) );

    terminalMenu(3,argvTest);
}


TEST_F(Fresco,sendMode_InputExit)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce( inputExit );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,fifoCLient_socketError)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce( inputStr );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,fifoCLient_connectError)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, connect(_,_,_))
        .WillOnce(Return( RETURN_ERROR ) );

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce( inputStr );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,fifoCLient_sendError)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, connect(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, send(_,_,_,_))
        .WillOnce(Return( -1 ) );

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce( inputStr );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,fifoCLient_sendStrOK)
{
    memcpy(argvTest[1],"send",5);
    memcpy(argvTest[2],"MCU",5);

    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, connect(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, send(_,_,_,_))
        .WillOnce( Return(1) );

    EXPECT_CALL(outMock, scanfUT(_,_) )
        .WillOnce( inputStr );

    terminalMenu(3,argvTest);
}

TEST_F(Fresco,sigHandler)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, listen(_,_))
        .WillOnce(Return( -1 ) );

    EXPECT_CALL(outMock, signal(_,_))
        .WillRepeatedly(signalTest);

    bindMode(Mcu);
}

TEST_F(Fresco,crc_16_nullptrError)
{
    crc_16(nullptr,0);
}