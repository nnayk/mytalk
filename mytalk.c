/* 
 * Nakul Nayak
 * CPE 357
 * Description: mytalk chat application 
 */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "server.h"
#include "client.h"
#include <talk.h>

/* macros, if any */
#define V_OPT 1 << 0
#define A_OPT 1 << 1
#define N_OPT 1 << 2
#define DECIMAL 10

/* function prototypes */
void usage();
void displayOptions(char *);

/* global vars, if any */
uint8_t options; /* contains a bitwise or of given options */
char *host="(none)"; /*input hostname*/
short port; /* input port */
int MODE = 0; /* 0 if in client mode (default), 1 for server mode */

/* main function */
int main (int argc, char * argv[])
{
       /*local vars*/
       /* used for option processing */
       extern int getopt(int, char * const [], const char *);
       extern int optind; 

       char * errptr; /* used when determining desired mode */
       char *user; /* whether client or server is running */

       
       /*get first option */
       int option = getopt(argc,argv,"vaN");
       
        
       /* parse options */
       while(option != -1) 
       {
               if(option=='v')
               {
                       options |= V_OPT;
                       printf("V SET\n");
               }
               else if(option=='a')
               {
                       options |= A_OPT;
                       printf("A SET\n");
               }
               else if(option=='N')
               {
                       options |= N_OPT;
                       printf("N SET\n");
               }
               else
               {
                       fprintf(stderr,"unknown option: -?\n");
                       usage();
               }
              
               option = getopt(argc,argv,"vaN");
       }
       
       /* no hostname or port specified */
       if(optind >= argc)
       {
                usage();
       }
       
       printf("OPTIND = %d %s\n",optind,argv[optind]);
       strtol(argv[optind],&errptr,DECIMAL); /* try to grab port number */
       if(*errptr) 
       {
               user = "client";
               port = strtol(argv[optind+1],NULL,DECIMAL);
       }
       else 
       {
               user = "server";
               port = strtol(argv[optind],NULL,DECIMAL);
       }

       if(options & V_OPT) displayOptions(user);

       
       if(*errptr)        
       {
               MODE = 1;
               host = argv[optind];
               printf("PORT = %d hostname = %s\n",port,host);
               client(argv[optind]);
       }

       else
       {
               server(argv[optind]);     
       }
       
       return 0;
}

void usage()
{
        fprintf(stderr,
                "usage: ./mytalk [-a] [-N] [-v] [hostname] port\n");
        exit(EXIT_FAILURE);
}

void displayOptions(char *user)
{
        int verbose=0;
        int accept = 0;
        int windows = 0;

        if(options & V_OPT) verbose = 1;
        if(options & A_OPT) accept  = 1;
        if(options & N_OPT) windows = 1;
        
        printf("Options:\n");
        printf("  %s opt_verbose = %d\n","int",verbose);
        printf("  talkmode opt_mode = %s\n",user);
        printf("  int opt_port = %d\n",port);
        printf("  char *opt_host = %s\n",host);
        printf("  int opt_accept = %d\n",accept);
        printf("  int opt_windows = %d\n",windows);
}
