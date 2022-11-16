/*========================================================*/
/*Program file: DataCollection.h                          */
/*Brief: Data Collection header file                      */
/*                                                        */
/*Dependencies: None to report                            */
/*                                                        */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC  */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC  */
/*                                                        */
/*                                       Last mod:12/10/21*/
/*========================================================*/
#ifndef DataCollection_included
#define DataCollection_included

#include "Header.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void clearMemory (int **Free, int lines);
void notenoughmemory();

#endif // DataCollection_included