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

/* global vars, if any */
uint8_t options; /* contains a bitwise or of given options */
char *host; /*input hostname*/
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

       
       /*get first option */
       int option = getopt(argc,argv,"vaN");
        
       start_windowing();
       
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

       strtol(argv[optind],&errptr,DECIMAL);
       
       if(*errptr)        
       {
               MODE = 1;
               printf("HOSTNAME = %s\n",argv[optind]);
               host = argv[optind];
               port = strtol(argv[optind+1],NULL,DECIMAL);
               client();
       }

       else
       {
               port = strtol(argv[optind],NULL,DECIMAL);
               server();     
       }
       
       return 0;
}

void usage()
{
        fprintf(stderr,
                "usage: ./mytalk [-a] [-N] [-v] [hostname] port\n");
        exit(EXIT_FAILURE);
}


