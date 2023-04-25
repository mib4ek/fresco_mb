/*****************************************************************************
 *  Include Files
 *****************************************************************************/
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/******************************************************************************
*   Local Macro Definitions
*******************************************************************************/
#define RETURN_ERROR -1
#define MAX_BUFFER_SIZE 80
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
/*! \fn int sendMode
 *  \brief connect to socket and send data
 *  \param char* - path to socket/fifo file
 *  \return RETURN_ERROR if socket error, otherwise 0
 ***********************************************************/
int sendMode(char *mcuName)
{
    char    readbuf[80];
    int     sfd     = 0;
    int     cfd     = 0;
    int     retVal  = 0;
    struct sockaddr_un my_addr      = {0};
    struct sockaddr_un peer_addr    = {0};
    socklen_t peer_addr_size        = 0;

    memset(&my_addr, 0, sizeof(struct sockaddr_un)); /* Clean the structure memory pool*/

    printf("Input message (q to exit): %s",readbuf);
    scanf ("%s",readbuf);

    if (0 != strncmp (readbuf,"q",strlen(readbuf) ) )
    {

        printf("STR: %s",readbuf);

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if ( RETURN_ERROR == sfd )
        {
            perror("socket");
        }
        else
        {
            my_addr.sun_family = AF_UNIX;

            strncpy(my_addr.sun_path, mcuName, sizeof(my_addr.sun_path) - 1);

            retVal = connect(sfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
            if( RETURN_ERROR == retVal ) {
                perror("[-]Error in socket");
            }
            else
            {
                printf("[+]Connected to Server.\n");

                if( send(sfd, readbuf, MAX_BUFFER_SIZE, 0) < 0)
                {
                    printf("send failed");
                }
            }
        }

        printf("[+]Closing the connection.\n");
        close(sfd);
    }

    return 0;
}