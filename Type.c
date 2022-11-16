/*=======================================================*/
/*Program file: Type.c                                   */
/*Brief: Generates an answer according to the            */
/*       question made.                                  */
/*                                                       */
/*Description: This program file is responsible for      */
/*             analysing the answer to the questions     */
/*             made with the different types of          */
/*             options available in the text files       */
/*             (A0 to A6).                               */                            
/*                                                       */
/*Dependencies: Maze should be created and with the      */
/*              information stored.                      */                          
/*                                                       */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC */
/*                                                       */
/*                                      Last mod:07/11/21*/
/*=======================================================*/

#include "Type.h"

/*******************************************************************************
* Function name: type_analysis()
*
* Arguments: char type - Type of awnser asked (A0 to A6)
*            int **maze - The maze itself stored in a matrix
*            int destination[] - Cell to be analyzed
*            int destination2nd[] - Cell to be analyzed in case the answer is A6
*            int totallines - Total number of lines the matrix has
*            int totalcolumns - Total number of columns the matrix has
*           
* Return: int - Answer to the question asked
*
* Side-effects: none to report
*
* Description:  This function receives the type of question asked by the user
*               in the file and calling the functions that execute the
*               procedures resposible for generating the answer.
*
*******************************************************************************/
int type_analysis(char type, int **maze, int destination[], int destination2nd[], int totallines, int totalcolumns){

    int result=0;
    
    //If it´s a position outside the boundaries of the maze, returns -2.
    if(celloutofmaze(destination[0],destination[1],totallines,totalcolumns)==1)     
                return -2;
    if(type=='6')//In the A6 mode we need to analyze the 2nd set of coordinates, returning -2 if it's outside the maze
        if(celloutofmaze(destination2nd[0],destination2nd[1],totallines,totalcolumns)==1)  
                return -2;

    switch(type){//Finds what Type of question was made by the user in the text file
        case '1': //A1 - Find what the type of cell
            result=maze[destination[0]-1][destination[1]-1];
        break;
        case '2': //A2 - Find if any neighbours cell is white
            result=neighbours(destination,0,maze,totallines,totalcolumns);
        break;
        case '3': //A3 - Find if any neighbours cell is gray
            result=neighbours(destination,1,maze,totallines,totalcolumns);
        break;
        case '4': //A4 - Find if any neighbours cell is black
            result=neighbours(destination,-1,maze,totallines,totalcolumns);
        break;
        case '5': //A5 - Find if the cell is gray, if it is, is it breakable?
            result=iscellbreakable(destination[0], destination[1],maze,totallines,totalcolumns);
        break;
        case '6': //A6 - Find if the 2 set of coordinates given are in the same room (should be both white)
            result=startA6(maze, destination, destination2nd,totallines,totalcolumns,-2,1);
        break;
    }
    return result;//Returns the result to be printed in the file 
return 0;
}

/***********************************************************************************************
* Function name: neighbours()
*
* Arguments: int analyze[] - Coordinates to be analyzed 
*            int type - Type of question asked 
*                       -1 for black, 0 for white, 1 for grey
*            int** maze - The maze itself stored in a matrix
*            int maxline - Total number of lines the matrix has
*            int maxcolumn - Total number of columns the matrix has      
*           
* Return: 1 for a positive answer or 0 for a negative one
*
* Side-effects: none to report
*
* Description: This function is responsible for analyzing the neighbours of a 
*              specific cell. Said neighbours will be stored in a vector wich
*              will be compared with the type of question asked.
* 
*************************************************************************************************/
int neighbours(int analyze[], int type, int** maze, int maxline, int maxcolumn){
    
    int line=analyze[0]-1, column=analyze[1]-1;
    int cells[4]={-2,-2,-2,-2};//Vector that will contain the 4 neighbours analyzed
    maxline-=1; // The matrix allocated in memory starts in 0, and the maze in 1, hence the need to remove 1 to the total value
    maxcolumn-=1;
    int i=0;

    //There is a need to specify each case of analyze so we dont try to acess any invalid place of the matrix                                                           
    if(line!=0)
        cells[0]=maze[line-1][column];//Analzes the upper neighbouring cell      
    if(column!=0)
        cells[1]=maze[line][column-1];//Analzes the left neighbouring cell 
    if(column!=maxcolumn)
        cells[2]=maze[line][column+1];//Analzes the right neighbouring cell 
    if(line!=maxline)
        cells[3]=maze[line+1][column];//Analzes the bottom neighbouring cell      

    //We now analyze all the neigbour cells and see if at least one has what we were looking for
    //If so, we simply return the result
    for(i=0;i<4;i++){
        if(cells[i]==type)//We have a cell that can be equal to 0 (white) or -1 (black)
            return 1;
        else if((cells[i]>0)&&(type!=-1)&&(type!=0))//Gray cells have any value thats greater than 0
            return 1;
    }
    return 0;//No cell fitting the citeria was found
}

/******************************************************************************************
* Function name: iscellbreakable()
*
* Arguments: int analyzel - Line to be analyzed
*            int analyzec - Column to be analyzed
*            char** maze - The maze itself stored in a matrix
*            int maxline - Total number of lines the matrix has
*            int maxcolumn - Total number of columns the matrix has      
*           
* Return: -1 for a black or white cell, 0 for unbrakable grey cell and 1 for breakable cell
*
* Side-effects: none to report
*
* Description:  Analyses if a cell is unbreakable or not (A5)
*
********************************************************************************************/
int iscellbreakable(int analyzel, int analyzec, int **maze, int maxline, int maxcolumn){
    int line=analyzel-1, column=analyzec-1;
    maxcolumn-=1;
    maxline-=1;

    if((maze[line][column]==0) || (maze[line][column]==-1))//We are in the presence of a white or black cell 
        return -1;
    if((line!=0) && (line!=maxline))//We can analyze the top and lower adjancent cell
        if(((maze[line-1][column]==0)||(maze[line-1][column]<-1)) && ((maze[line+1][column]==0)||(maze[line+1][column]<-1)))//They both are a white cell
            return 1;//Breakable
    if((column!=0)&&(column!=maxcolumn))//We can analyze the left and right adjancent cell   
        if(((maze[line][column-1]==0)||(maze[line][column-1]<-1)) && ((maze[line][column+1]==0)||(maze[line][column+1]<-1)))
            return 1;//Breakable
    if(((line==0)||(line==maxline)) && (column!=0)&&(column!=maxcolumn))//If we are on the 1st or last line we can still analyze the right and left cell
        if(((maze[line][column-1]==0)||(maze[line][column-1]<-1)) && ((maze[line][column+1]==0)||(maze[line][column+1]<-1)))//they both are a white cell
            return 1;//Breakable
    if(((column==0)||(column==maxcolumn)) && (line!=0)&&(line!=maxcolumn))//If we are on the 1st or last column we can still analyze the upper and lower cell
        if(((maze[line-1][column]==0)||(maze[line-1][column]<-1)) && ((maze[line+1][column]==0)||(maze[line+1][column]<-1)))//They both are a white cell
            return 1;//Breakable
    return 0;//No more options, unbrakable
}

/******************************************************************************
* Function name: celloutofmaze()
*
* Arguments: int a - line coordinate to be analyzed
*            int b - column coordinate to be analyzed
*            int maxlines - Total number of lines the matrix has
*            int maxcolumns - Total number of columns the matrix has
*           
* Return: 1 - cell is out of maze, 0 - cell is not out of maze
*
* Side-effects: none to report
*
* Description: This function will analyze if a cell is out of the maze
*
*******************************************************************************/
int celloutofmaze(int a, int b,int maxlines, int maxcollumns){
    if((a<=0)|| (a>maxlines)|| (b>maxcollumns)||(b<=0)){//Coordenates are inferior/superior to the minimum/max value
            return 1;//Is out of the maze
        }
return 0;//Isn't out of the maze
}