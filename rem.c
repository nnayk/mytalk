/* 
 * Nakul Nayak
 * CPE 357
 * Description: 
 */

/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* macros, if any */
#define V_OPT 1 << 0
#define A_OPT 1 << 1
#define N_OPT 1 << 2

/* function prototypes */

/* global vars, if any */
uint8_t options; /* contains a bitwise or of given options */

/* main function */
int main (int argc, char * argv[])
{
       /*local vars*/
       extern int getopt(int, char * const [], const char *);
       extern int optind;
       int option = getopt(argc,argv,"vaN");
       
       /* default is 10 words */
       while(option != -1) 
       {
               if(option=='v')
               {
                       options |= V_OPT; 
               }
               else if(option=='a')
               {
                       options |= A_OPT:
               }
               else if(options=='N')
               {
                       options |= N_OPT:
               }
               else
               {
                       fprintf(stderr,
                       "/mytalk: invalid option -- '%s'\n",
                       argv[optind]);

                       frpintf(stderr,"unknown option: -?\n");

                       fprintf(stderr,
                       "usage: ./mytalk [-a] [-N] [-v] [hostname] port\n");
                       exit(EXIT_FAILURE);
               }
               
               option = getopt(argc,argv,"vaN");
       }

        return 0;
}



