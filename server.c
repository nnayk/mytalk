/* 
 * Nakul Nayak
 * CPE 357
 * Description: 
 */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include "safeSys.h"
#include "talk.h"

/* macros, if any */
#define DEFAULT_BACKLOG 1 /* only 1 connection allowed */
#define MAXLEN 1000 /* max message length */
#define LOCAL 0 /* index of local fd */
#define REMOTE (LOCAL+1) /* index of local fd */

/* function prototypes */

/* global vars, if any */
extern int port;

/* main function */
void server()
{
        struct sockaddr_in sa;
        ssize_t len;
        int serverSock, talkSock;
        char buff[MAXLEN+1]={0};
        /*char *message = "Hello client\n";*/
        struct pollfd fds[REMOTE+1];
        fds[LOCAL].fd = STDIN_FILENO;
        fds[LOCAL].events = POLLIN;
        fds[LOCAL].revents = 0;
        fds[REMOTE] = fds[LOCAL];
        
        serverSock = safeSocket(AF_INET,SOCK_STREAM,0);


        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(serverSock,(struct sockaddr *)&sa,sizeof(sa));

        safeListen(serverSock,DEFAULT_BACKLOG);

        /*printf("DONE LISTENING\n");*/

        talkSock = safeAccept(serverSock,NULL,NULL);
        fds[REMOTE].fd = talkSock;
        
        /*
        printf("MESSAGE = %s\n",message);
        len = safeRecv(talkSock,buff,sizeof(buff),0);
        printf("RECEIVED %s %d\n",buff,(int)len);
        len = safeWrite(STDOUT_FILENO,buff,(int)len);
        
        len = safeSend(talkSock,message,strlen(message),0);
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
                                len = safeSend(talkSock,buff,len,0);
                        }
                        
                }

                if(fds[REMOTE].revents & POLLIN)
                {
                        len = safeRecv(talkSock,buff,sizeof(buff),0);
                        buff[len]='\0';
                        write_to_output(buff,len);
                }
        }while(1);

        close(talkSock);
        close(serverSock);
}



