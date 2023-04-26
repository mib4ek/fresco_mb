#include <gtest/gtest.h>

using namespace ::testing;

TEST(TEST,VAL)
{
    ASSERT_EQ(1,1);
}


int main(int argc, char **argv)
{
   InitGoogleTest();
   int result = RUN_ALL_TESTS();

   std::cout.flush();
   std::cerr.flush();
   return result;
}