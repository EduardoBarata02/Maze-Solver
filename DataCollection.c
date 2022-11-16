/*========================================================*/
/*Program file: DataCollection.c                          */
/*Brief: Collects the data from the file and stores it    */
/*                                                        */
/*Description: This file is responsible for opening the   */
/*             text file and storing the contained        */
/*             information. Calling a function to analyze */
/*             the answer to the question asked.          */
/*                                                        */
/*Dependencies: None to report                            */
/*                                                        */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC  */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC  */
/*                                                        */
/*                                       Last mod:08/11/21*/
/*========================================================*/

#include "DataCollection.h"

/*********************************************************************************
* Function name: fileopener()
*
* Arguments: char *inputname - name of the input file
*            int flag        - variable that determines wich type of file (.in1 
*                              or .in) wewill be solving
*
* Return: void
*
* Side-effects: none to report
*
* Description: Opens the input file, analyses if it is type .in1 (1st submission),
*              or type .in (final submission) and creates the output file. 
*              It will, then, collect the information from the text file and 
*              dynamicly allocate memory for the matrix that represents the maze.
*              This will be done as long as there is information in the file
*              and this function will write the anwser in sayed output file.
*
**********************************************************************************/
void fileopener(char *inputname, int flag){
    FILE *fp=NULL, *outfp=NULL;
    char *inputcheck=NULL,*outputname=NULL, type[2]="\0";
    int tl=0, tc=0, l=0, c=0, aux=0, value=0, des[2]={0}, des1[2]={-1,-1}, answer=0, count;
    int **maze = NULL;
    

    if((outputname=(char *) calloc(1,strlen(inputname)+2))==NULL)//Allocates memory for the output file name
        notenoughmemory();          
    strcpy(outputname,inputname);//Copies the inputname to the outputname variable
    inputcheck=strrchr(outputname,'.');//Finds where the last dot is
    if(inputcheck==NULL){//No dot was found
        free(outputname);
        exit(0);
    }
    if(flag){
        if(strcmp(inputcheck,".in1")!=0){//Finds if the last dot (correspondent to the file type) is of the type .in1   
            free(outputname);
            exit(0);
        }
        strcpy(inputcheck,".sol1");//Overwrites the output file extension
    }
    else{
        if(strcmp(inputcheck,".in")!=0){//Finds if the last dot (correspondent to the file type) is of the type .in   
            free(outputname);
            exit(0);
        }
        strcpy(inputcheck,".sol");//Overwrites the output file extension
    }
    
    if((fp=fopen(inputname,"r"))==NULL){//Opens the input file if it exists            
        free(outputname);
        exit(0);
    }

    if((outfp=fopen(outputname,"w"))==NULL){//Creates an output file
        free(outputname);
        exit(0);
    }
 
    //Reads the six first elements of the text file. As long as there is information, the cicle
    //will continue
    while (fscanf(fp,"%d %d %d %d",&tl,&tc,&des[0],&des[1])==4){
        if(flag){
            if(fscanf(fp," %c %c", &type[0],&type[1])!=2)
                exit(0); 
            if(type[1]=='6')//If the user request the A6 analyzes we need to collect one more pair of coordinates
                if(fscanf(fp,"%d %d",&des1[0],&des1[1])!=2)
                    exit(0);
        }
        if(fscanf(fp,"%d",&aux)!=1)
            exit(0);
        if(celloutofmaze(des[0],des[1],tl,tc)||((type[1]=='6')&&celloutofmaze(des1[0],des1[1],tl,tc))){
            if(flag)
                fprintf(outfp,"-2\n\n");
            else
                fprintf(outfp,"-1\n\n\n");
            while(aux>0){
                if(fscanf(fp,"%d %d %d",&l,&c,&value)!=3)
                    exit(0);
                aux--;
            }
        }
        else{
            maze=NULL;
            if((maze = (int **) malloc(tl * sizeof(int *)))==NULL)//Allocates the lines of the matrix that represents the maze
                notenoughmemory();   
            for(count = 0; count < tl; count++)//Allocates each column for each line of the matrix that represents the maze 
                if((maze[count] = (int*) calloc (tc, sizeof(int)))==NULL)
                    notenoughmemory();  

            while(aux>0){//While there are cells to be processed
                if(fscanf(fp,"%d %d %d",&l,&c,&value)!=3)//Collects the coordinates and values of the black and gray cells
                    exit(0);
                maze[l-1][c-1]= value;//Assigns the value to the correct location
                aux--;//-1 cell to be processed
            }
            if(flag){
                answer = type_analysis(type[1],maze,des,des1,tl,tc);//Gets an answer for the question the user asked
                fprintf(outfp,"%d\n\n",answer);//Prints sayed answer in the file
            }
            else{
                if((maze[0][0]!=0)||(maze[des[0]-1][des[1]-1]!=0))
                    fprintf(outfp,"-1\n\n\n");
                else                  
                    solve(maze,tl,tc,des,outfp);
            }
            clearMemory(maze,tl);//Clears maze from memory
        }
    }
    free(outputname);//Free all memory allocated
    fclose(outfp);
    fclose(fp);

}

/*******************************************************************************
* Function name: clearMemory()
*
* Arguments: char **Free - Matrix maze to be freed from memory
*            int lines - Total number of lines the matrix has
*           
* Return: void
*
* Side-effects: Maze matrix will be wipped from memory
*
* Description: This function will clear the matrix that is storing the 
*              maze created to store the information from the text file.
*
*******************************************************************************/
void clearMemory (int **Free, int lines){
    int i=0;
    for( ;i<lines;i++)//Frees all the collumns allocated
        free(Free[i]);
    free(Free);//Frees the pointer to the lines
}

/*******************************************************************************
* Function name: notenoughmemory()
*
* Arguments: none
*           
* Return: void
*
* Side-effects: none to report
*
* Description: This function will display an error message in the terminal 
*              and will exit the program.
*
*******************************************************************************/
void notenoughmemory(){
    printf("Not enough memory...\n");
    exit(0);
}