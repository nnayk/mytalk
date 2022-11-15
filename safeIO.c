/* 
 * Nakul Nayak
 * CPE 357
 * Description: secure IO functions. Exit on failure.
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
#include "talk.h"
#include "safeIO.h"

/*macros*/
#define MAXLEN 1000 /* max message length */
#define LOCAL 0 /* index of local fd */
#define REMOTE (LOCAL+1) /* index of local fd */
#define RESPONSE 4
#define V_OPT 1 << 0
#define A_OPT 1 << 1
#define N_OPT 1 << 2

int safeSocket()
{
        int sock;
        if((sock = socket(AF_INET,SOCK_STREAM,0))==-1)
        {
                perror("socket");
                exit(EXIT_FAILURE);
        }

        return sock;
}

ssize_t safeSend(int sockd,const void *buf,size_t len,int flags)
{
        ssize_t actual = send(sockd,buf,len,flags);

        if(actual==-1)
        {
                perror("send");
                exit(EXIT_FAILURE);
        }

        return actual;
}

ssize_t safeRecv(int sockd,void *buf,size_t len,int flags)
{
        ssize_t actual = recv(sockd,buf,len,flags);
        if(actual==-1)
        {
                perror("recv");
                exit(EXIT_FAILURE);
        }

        return actual;
}

void safeBind(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
        if(bind(sockfd,addr,addrlen))
        {
                perror("bind");
                exit(EXIT_FAILURE);
        }
}

void safeListen(int sockd,int backlog)
{
        if(listen(sockd,backlog))
        {
                perror("listen");
                exit(EXIT_FAILURE);
        }
}

int safeAccept(int sockfd,struct sockaddr *addr, socklen_t *addrlen)
{
        int newFd = accept(sockfd,addr,addrlen);
        if(newFd==-1)
        {
                perror("accept");
                exit(EXIT_FAILURE);
        }
        
        return newFd;
}

void safeConnect(int sockfd,const struct sockaddr *addr, socklen_t addrlen)
{
        if(connect(sockfd,addr,addrlen)==-1)
        {
                perror("connect");
                exit(EXIT_FAILURE);
        }
}

ssize_t safeRead(int fd,void *buf,size_t count)
{
        ssize_t actual = read(fd,buf,count);

        if(actual==-1)
        {
                perror("read");
                exit(EXIT_FAILURE);
        }

        return actual;
}

ssize_t safeWrite(int fd, const void *buf, size_t count)
{
        ssize_t actual = write(fd,buf,count);

        if(actual==-1)
        {
                perror("write");
                exit(EXIT_FAILURE);
        }

        return actual;
}

void terminateMsg()
{
        char *output="Connection closed. ^C to terminate.";
        int rvalue = write_to_output(output,strlen(output));
        
        if(rvalue==ERR)
        {
                perror("write_to_output");
                exit(EXIT_FAILURE);
        }
}

void communicate(int sockd,char *buff)
{
        ssize_t len;
        extern uint8_t options;
        struct pollfd fds[REMOTE+1];
        fds[LOCAL].fd = STDIN_FILENO;
        fds[LOCAL].events = POLLIN;
        fds[LOCAL].revents = 0;
        fds[REMOTE] = fds[LOCAL];
        fds[REMOTE].fd = sockd;
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
                                len = safeSend(sockd,buff,len,0);
                                if(options & N_OPT) break;
                        }
                        
                        if(has_hit_eof())
                        {
                                buff[0]='\n';
                                buff[1]='\0';
                                safeSend(sockd,buff,strlen(buff),0);
                                if(!(options & N_OPT)) stop_windowing();
                                else printf("%s","\n");
                                break;
                        }
                }

                if(fds[REMOTE].revents & POLLIN)
                {
                        len = safeRecv(sockd,buff,sizeof(buff),0);
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
}
