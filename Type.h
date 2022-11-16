/*========================================================*/
/*Program file: Type.h                                    */
/*Brief: Type header file                                 */
/*                                                        */
/*Dependencies: None to report                            */
/*                                                        */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC  */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC  */
/*                                                        */
/*                                       Last mod:08/10/21*/
/*========================================================*/
#ifndef Type_included
#define Type_included

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Header.h"

int neighbours(int analyze[], int type, int** maze, int maxline, int maxcolumn);
int celloutofmaze(int a, int b,int maxlines, int maxcollumns);

#endif //Type_included