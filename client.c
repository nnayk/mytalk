/* 
 * Nakul Nayak
 * CPE 357
 * Description: 
 */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "safeSys.h"
#include "talk.h"

/* macros, if any */
#define MAXLEN 1000 /* max message length */
#define LOCAL 0 /* index of local fd */
#define REMOTE (LOCAL+1) /* index of remote fd */

/* function prototypes */

/* global vars, if any */
extern char * host;
extern short port;

void client()
{
        struct sockaddr_in sa;
        struct hostent *hostent;
        ssize_t len;
        /*char *message = "Hi server\n";*/
        char buff[MAXLEN+1]={0};
        int clientSock;
        struct pollfd fds[REMOTE+1];
        fds[LOCAL].fd = STDIN_FILENO;
        fds[LOCAL].events = POLLIN;
        fds[LOCAL].revents = 0;
        fds[REMOTE] = fds[LOCAL];

        clientSock = safeSocket();
        
        fds[REMOTE].fd = clientSock;

        hostent = gethostbyname(host);

        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = *(uint32_t *)hostent->h_addr_list[0];

        connect(clientSock,(struct sockaddr *)&sa,sizeof(sa));
        
        /*
        len = safeSend(clientSock,message,strlen(message),0);
        
        len = safeRecv(clientSock,buff,sizeof(buff),0);

        safeWrite(STDOUT_FILENO,buff,len);
        */

        do
        {
                poll(fds,sizeof(fds)/sizeof(struct pollfd),-1);
                if(fds[LOCAL].revents & POLLIN)
                {
                        update_input_buffer();
                        
                        if(has_whole_line())
                        {
                                /*printf("WRITING LINE\n");*/
                                len = read_from_input(buff,MAXLEN);
                                len = safeSend(clientSock,buff,len,0);
                        }
                }

                if(fds[REMOTE].revents & POLLIN)
                {
                        /*printf("SERVER SENT\n");*/

                        len = safeRecv(clientSock,buff,sizeof(buff),0);
                        buff[len]='\0';
                        write_to_output(buff,len);
                }
        }while(1);

        close(clientSock);
}


