/*****************************************************************************
 *  Include Files
 *****************************************************************************/
#include "parser.h"
#include "fifoServ.h"
#include "crc16.h"
#include <string.h>
#include <sys/socket.h>
/******************************************************************************
*   Local Macro Definitions
*******************************************************************************/
/*  Max defined size of income str  */
#define MAX_BUFFER_SIZE 80
/* Max payload size */
#define MAX_PAYLOAD_SIZE  ( MAX_BUFFER_SIZE - 7) /* Full size minus headers and crc */
/*      Little endian switching     */
#define LE(left,right)   ( (uint16_t)left<<8)|right;

/******************************************************************************
*   Local Type Definitions
******************************************************************************/
#pragma pack(push,1)
typedef struct
{
    uint8_t dp_id;
    uint8_t dp_type;
    union
    {
        uint8_t value[MAX_PAYLOAD_SIZE];
        int16_t valueInt;
        uint8_t valueU8;
    };
    
}MSG_PAYLOAD;

typedef struct
{
    uint8_t     id;
    uint16_t    payloadSize;
    MSG_PAYLOAD payload;
    uint16_t    crc16;
}MSG_DATA;
#pragma pack(pop)

typedef enum
{
    PAYLOAD_UINT8 = 0,
    PAYLOAD_INT16,
    PAYLOAD_STR
}PAYLOAD_TYPE;

/****************************************************************************
*   Local Function Declarations
*****************************************************************************/
static void printReceivedData(MSG_DATA *msg, uint16_t crcCalc);

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

/***********************************************************/
/*! \fn int parseData
 *  \brief receive and parcing income data
 *  \param int* - accepted socket pointer
 *  \return void
 ***********************************************************/
int parseData(int *cfd)
{
    MSG_DATA    msg                         = {0};
    uint8_t     readbuf[MAX_BUFFER_SIZE];
    uint8_t     output[MAX_BUFFER_SIZE];
    uint8_t     crcPos                      = 0;
    uint16_t    crc16V                      = 0;
    uint8_t     dpIdPos                     = sizeof(msg.id) + sizeof(msg.payloadSize);
    uint8_t     payloadPos                  = dpIdPos + sizeof(msg.payload.dp_id) + sizeof(msg.payload.dp_type);

    /*      Clean structures     */
    memset(msg.payload.value,0x00,sizeof(msg.payload.value) );
    memset(readbuf,0x00,sizeof(readbuf) );

    /*  Check received string   */
    if( recv(*cfd, readbuf , MAX_BUFFER_SIZE , 0) <= 0)
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < (strlen( (const char*)readbuf) / 2); i++)
        {
            /*  Interpretation string as a byte array   */
            sscanf( (const char*)readbuf + 2*i, "%02X", (int*) &output[i]);
        }

        msg.id              = output[0];
        msg.payloadSize     = LE(output[sizeof(msg.id)+1],output[sizeof(msg.id)]);

        crcPos              = sizeof(msg.id) + sizeof(msg.payloadSize) + msg.payloadSize;
        msg.crc16           = LE(output[ crcPos + 1],output[crcPos]);
        crc16V              = crc_16(output,crcPos);

        msg.payload.dp_id   = output[dpIdPos];
        msg.payload.dp_type = output[dpIdPos + sizeof(msg.payload.dp_id)];

        switch (msg.payload.dp_type)
        {
            case PAYLOAD_UINT8:
                msg.payload.valueU8 = output[payloadPos];
                break;
            case PAYLOAD_INT16:
                msg.payload.valueInt = LE(output[payloadPos+1],output[payloadPos]);
                break;
            case PAYLOAD_STR:
                memcpy(msg.payload.value,output+payloadPos+1, ( output[payloadPos] < MAX_BUFFER_SIZE) ? output[payloadPos] : MAX_BUFFER_SIZE ); // first byte of payload - str length
                break;
            default:
                break;
        }
        printReceivedData(&msg, crc16V);
        
    }

    return 0;
}

/***********************************************************/
/*! \fn void printReceivedData
 *  \brief use for print-out result information
 *  \param MSG_DATA - data pointer for information which should be printed
 *  \param uint16_t - crc from local calculation for error handling
 *  \return void
 ***********************************************************/
static void printReceivedData(MSG_DATA *msg, uint16_t crcCalc )
{
    if (crcCalc != msg->crc16)
    {
        printf("INVALID CRC! ");
    }
    
    switch (msg->payload.dp_type)
    {
    case PAYLOAD_UINT8:
        printf ("Received: CRC %04X | DP_ID %d |\t DP_TYPE %d |\t DP_VALUE %d  |\t CRC_IN %04X \n", crcCalc, msg->payload.dp_id, msg->payload.dp_type, msg->payload.valueU8, msg->crc16);
        break;
    case PAYLOAD_INT16:
        printf ("Received: CRC %04X | DP_ID %d |\t DP_TYPE %d |\t DP_VALUE %d |\t CRC_IN %04X \n", crcCalc, msg->payload.dp_id, msg->payload.dp_type, msg->payload.valueInt, msg->crc16);
        break;
    case PAYLOAD_STR:
        printf ("Received: CRC %04X | DP_ID %d |\t DP_TYPE %d |\t DP_VALUE %s |\t CRC_IN %04X \n", crcCalc, msg->payload.dp_id, msg->payload.dp_type, msg->payload.value, msg->crc16);
        break;
    default:
        printf("Received: UNKNOWN type \n");
        break;
    }
    fflush(NULL);

}