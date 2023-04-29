/*****************************************************************************
 *  Include Files
 *****************************************************************************/
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include "parser.h"
#include <unistd.h>
/******************************************************************************
*   Local Macro Definitions
*******************************************************************************/
#define LISTEN_BACKLOG 1
#define RETURN_ERROR -1

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

/******************************************************************************
*   Local Type Definitions
******************************************************************************/

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
static char *fifoName = NULL;
/****************************************************************************
*   Local Function Declarations
*****************************************************************************/
static void sig_handler(int sig);
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

/***********************************************************/
/*! \fn int bindMode
 *  \brief connect and start listen socket
 *  \param char* - path to the socket/FIFO
 *  \return RETURN_ERROR if socket problem, otherwise 0
 ***********************************************************/
int bindMode(char *mcuName)
{
    int sfd = -1;
    int cfd = -1;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_size;

    fifoName = mcuName;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( RETURN_ERROR == sfd )
    {
        perror("socket");
    }
    else
    {
        memset(&my_addr, 0x00, sizeof(struct sockaddr_un));

        my_addr.sun_family = AF_UNIX;
        (void*) strncpy(my_addr.sun_path, mcuName, sizeof(my_addr.sun_path) - 1);

        if ( RETURN_ERROR == bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) )
        {
            perror("bind");
        }
        else
        {
            signal(SIGSEGV, sig_handler);
            signal(SIGTERM, sig_handler);
#ifndef UNITTEST
            while(1)
#endif
            {
                if ( RETURN_ERROR == listen(sfd, LISTEN_BACKLOG) )
                {
                    perror("listen");
                }
                else
                {
                    peer_addr_size = sizeof(struct sockaddr_un);
                    cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
                    if ( RETURN_ERROR == cfd )
                    {
                        perror("accept");
                    }
                    else
                    {
                        /* Code to deal with incoming connection(s)... */
                        if ( RETURN_ERROR == parseData(&cfd) )
                        {
#ifndef UNITTEST
                            break;
#endif
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void sig_handler(int sig) {
    switch (sig) {
    case SIGSEGV:
    case SIGTERM:
        unlink(fifoName);
#ifndef UNITTEST
        exit(1);
#endif
        break;
    default:
        perror("wasn't expecting that!\n");
#ifndef UNITTEST
        exit(1);
#endif
    }

}