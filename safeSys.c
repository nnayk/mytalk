/* 
 * Nakul Nayak
 * CPE 357
 * Description: secure system calls. Exit on failure.
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
