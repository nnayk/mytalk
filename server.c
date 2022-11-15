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
#include <pwd.h>
#include "safeIO.h"
#include "talk.h"

/* macros, if any */
#define DEFAULT_BACKLOG 1 /* only 1 connection allowed */
#define MAXLEN 1000 /* max message length */
#define LOCAL 0 /* index of local fd */
#define REMOTE (LOCAL+1) /* index of local fd */
#define RESPONSE 4
#define V_OPT 1 << 0
#define A_OPT 1 << 1
#define N_OPT 1 << 2

/* function prototypes */
int proposeConnection();
int approveConnection(char *);
int readConnection();

/* global vars, if any */
extern int port;
extern uint8_t options;

/* main function */
void server(char *hostname)
{
        struct sockaddr_in sa;
        int serverSock, talkSock;
        char buff[MAXLEN+1]={0};
        int connRequest;
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
        safeBind(serverSock,(struct sockaddr *)&sa,sizeof(sa));

        safeListen(serverSock,DEFAULT_BACKLOG);

        talkSock = safeAccept(serverSock,NULL,NULL);
        connRequest = proposeConnection(hostname);
        if(options & A_OPT)
        {
                printf("%s ","Accepting (opt_accept)\n");
        }

        if(connRequest)
        {
                buff[0] =  'o';
                buff[1] =  'k';
                buff[2] =  '\0';
                if(!(options & N_OPT)) start_windowing();

        }

        else
        {
                buff[0] = 'n';
                buff[1] = 'o';
                buff[2] = '\0';
        }

        safeSend(talkSock,buff,strlen(buff),0);
        printf("SENT %s\n",buff);

        if(!connRequest)
        {
                close(serverSock);
                close(talkSock);
                return;
        }

        fds[REMOTE].fd = talkSock;
        
        communicate(talkSock,buff);
        /*
        do
        {
                poll(fds,sizeof(fds)/sizeof(struct pollfd),-1);
                if(fds[LOCAL].revents & POLLIN)
                {
                        update_input_buffer();
                        

                        while(has_whole_line())
                        {
                                printf("WRITING LINE\n");
                                len = read_from_input(buff,MAXLEN);
                                printf("DONE WRITING\n");
                                if(len==ERR)
                                {
                                        perror("read_from_input");
                                        exit(EXIT_FAILURE);
                                }
                                len = safeSend(talkSock,buff,len,0);
                                
                                if(options & N_OPT) break;
                        }

                        
                        if(has_hit_eof())
                        {
                                buff[0]='\n';
                                buff[1]='\0';
                                safeSend(talkSock,buff,strlen(buff),0);
                                if(!(options & N_OPT)) stop_windowing();
                                break;
                        }
                }

                if(fds[REMOTE].revents & POLLIN)
                {
                        len = safeRecv(talkSock,buff,sizeof(buff),0);
                        if(!len)
                        {
                                printf("GOT NOTHING\n");
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

        close(talkSock);
        close(serverSock);
}

int proposeConnection(char *hostname)
{
        uid_t uid = getuid();
        struct passwd * userInfo = getpwuid(uid);
        char * username = userInfo->pw_name;

        printf("Mytalk request from %s@%s. Accept (y/n)? ",username,hostname);
        fflush(stdout);

        if(!(options & A_OPT)) return readConnection();
        return 1;
}

int readConnection()
{
        char response[MAXLEN];
        safeRead(STDIN_FILENO,response,MAXLEN);
        
        if(approveConnection(response)) return 1;
        return 0;
}

int approveConnection(char *response)
{
        
        if(response[0]=='y' && response[1]=='\n') return 1;

        if((response[0]=='y' || response[0]=='Y') 
           && (response[1]=='e' || response[1]=='E')
           && (response[2]=='s' || response[1]=='S')
           && (response[3]=='\n')) return 1;

        return 0;
}
