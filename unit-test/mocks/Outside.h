#ifndef RSTP_MOCK_H
#define RSTP_MOCK_H
#include <gmock/gmock.h>

class MockOutside{
public:
	MOCK_METHOD0(Switch_Panel_Messsages__getPendingMsgId, uint32_t());
};

#endif