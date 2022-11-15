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
#include "safeIO.h"
#include "talk.h"

/* macros, if any */
#define MAXLEN 10 /* max message length */
#define LOCAL 0 /* index of local fd */
#define REMOTE (LOCAL+1) /* index of remote fd */
#define V_OPT 1 << 0
#define A_OPT 1 << 1
#define N_OPT 1 << 2

/* function prototypes */

/* global vars, if any */
extern char * host;
extern short port;
extern uint8_t options;

void client(char *hostname)
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

        if(!hostent)
        {
                perror("gethostbyname");
                exit(EXIT_FAILURE);
        }

        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = *(uint32_t *)hostent->h_addr_list[0];

        safeConnect(clientSock,(struct sockaddr *)&sa,sizeof(sa));

        printf("Waiting for response from %s.\n",hostname);
        len = safeRecv(clientSock,buff,sizeof(buff),0);
        buff[len]='\0';
        if(strcmp("ok",buff))
        {
                printf("%s declined connection\n",hostname);
                close(clientSock);
                return;
        }

        if(!(options & N_OPT)) start_windowing();
        
        communicate(clientSock,buff);
        
        /*
        len = safeSend(clientSock,message,strlen(message),0);
        
        len = safeRecv(clientSock,buff,sizeof(buff),0);

        safeWrite(STDOUT_FILENO,buff,len);
        */

        /*
        do
        {
                poll(fds,sizeof(fds)/sizeof(struct pollfd),-1);
                if(fds[LOCAL].revents & POLLIN)
                {
                        update_input_buffer();
                        
                        while(has_whole_line())
                        {
                                len = read_from_input(buff,MAXLEN);
                                if(len==ERR)
                                {
                                        perror("read_from_input");
                                        exit(EXIT_FAILURE);
                                }
                                len = safeSend(clientSock,buff,len,0);
                                printf("SENDING %s\n",buff);
                                if(options & N_OPT) break;
                        }
                        
                        if(has_hit_eof())
                        {
                                buff[0]='\n';
                                buff[1]='\0';
                                safeSend(clientSock,buff,strlen(buff),0);
                                if(!(options & N_OPT)) stop_windowing();
                                break;
                        }
                }

                if(fds[REMOTE].revents & POLLIN)
                {
                        len = safeRecv(clientSock,buff,sizeof(buff),0);
                        if(!len)
                        {
                                terminateMsg();
                                pause();
                                break;
                        }
                        buff[len]='\0';
                        if(write_to_output(buff,len)==ERR)
                        {
                                perror("write_to_output");
                                exit(EXIT_FAILURE);
                        }
                }
        }while(1);
        */

        printf("SERVER QUIT\n");
        close(clientSock);
}
