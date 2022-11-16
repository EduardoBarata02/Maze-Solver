/*========================================================*/
/*Program file: Commandline.c                             */
/*Brief: Main() Reads command line arguments              */
/*                                                        */
/*Description: This file is responsible for receiving all */
/*             the command line options and checking if   */
/*             they are correct. It will then analyze the */
/*             mazes in case the answser is positive.     */
/*                                                        */
/*Dependencies: None to report                            */
/*                                                        */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC  */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC  */
/*                                                        */
/*                                       Last mod:08/10/21*/
/*========================================================*/
#include "Commandline.h"

/************************************************************************************
* Function name: main()
*
* Arguments: int argc - number of strings pointed to by argv[]
*            char *argv[] - argument vector
*
* Return: int - program exit code
*
* Side-effects: none to report
*
* Description: This function reads the arguments typed by the user in the
*              terminal it will confirm if there are exactly 3 arguments
*              and if the first one is -s. It will then call fileopener to
*              open the text files and analyzing the mazes. With the last submission
*              it is only needed 2 arguments, excluding the -s argument.
*
*************************************************************************************/
int main(int argc, char *argv[]){
    if ((argc==3) && (strcmp(argv[1],"-s")==0))//First argument typed by the user should be -s
        fileopener(argv[2], 1);//Opens the file
    if(argc==2)
        fileopener(argv[1],0);


    return 0;
}