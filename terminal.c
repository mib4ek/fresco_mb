/*****************************************************************************
 *  Include Files
 *****************************************************************************/
#include "terminal.h"
#include "fifoServ.h"
#include "fifoClient.h"

/******************************************************************************
*   Local Macro Definitions
*******************************************************************************/
#define TERMINAL_ARGUMENTS  3
#define TERMINAL_MCU_POS    2
#define TERMINAL_ACTION_POS 1

#define TERMINAL_BIND       "bind"
#define TERMINAL_SEND       "send"

/******************************************************************************
*   Local Type Definitions
******************************************************************************/

/****************************************************************************
*   Local Function Declarations
*****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

/***********************************************************/
/*! \fn void terminalMenu
 *  \brief select listen/sending mode
 *  \param int - number of arguments
 *  \param char* - arguments data
 *  \return void
 ***********************************************************/
void terminalMenu(int argc, char *argv[])
{
    if ( TERMINAL_ARGUMENTS != argc ) 
    {
        printf("Error: number of arguments not equal %d \n",TERMINAL_ARGUMENTS);
    }
    else
    {
        if ( 0 == strncmp( TERMINAL_BIND, argv[TERMINAL_ACTION_POS], sizeof(TERMINAL_BIND) ) )
        {
            bindMode(argv[TERMINAL_MCU_POS]);
        }
        else if ( 0 == strncmp( TERMINAL_SEND, argv[TERMINAL_ACTION_POS], sizeof(TERMINAL_SEND)  ) )
        {
            sendMode(argv[TERMINAL_MCU_POS]);
        }
    }
}