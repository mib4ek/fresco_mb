#include <gtest/gtest.h>
#include "Outside.h"
    #include "fifoServ.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

static char Mcu[]="MCU";

using ::testing::Test;
using namespace ::testing;


class Fresco : public ::testing::Test
{
public:
    Fresco()
    {
        outside_setMock(outMock);

    }
    ~Fresco()
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {

    }

    MockOutside outMock;
};

TEST_F(Fresco,bindMode_socketError)
{
    EXPECT_CALL(outMock, socket(_,_,_)).Times(1)
        .WillOnce(Return( -1 ) );

    bindMode(Mcu);
}

TEST_F(Fresco,bindMode_bindError)
{
    EXPECT_CALL(outMock, socket(_,_,_))
        .WillOnce(Return( 0 ) );

    EXPECT_CALL(outMock, bind(_,_,_))
        .WillOnce(Return( -1 ) );

    bindMode(Mcu);
}


int main(int argc, char **argv)
{
   InitGoogleTest();
   int result = RUN_ALL_TESTS();

   std::cout.flush();
   std::cerr.flush();
   return result;
}