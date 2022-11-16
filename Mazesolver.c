/*============================================================*/
/*Program file: Mazesolver.c                                  */
/*Brief: File responsible for creating the rooms in the maze  */
/*                                                            */
/*Description: This file will go through all the maze and,    */
/*             when it is in the presence of a white cell     */
/*             it will "paint" it with a negative number      */
/*             representing a room. It will then see how      */
/*             these rooms are connected with each other      */
/*             creating and adjacency list for a futher       */
/*             connectivity analyzis.                         */
/*                                                            */
/*Dependencies: The maze should be represented in a 2d matrix */
/*                                                            */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC      */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC      */
/*                                                            */
/*                                           Last mod:11/11/21*/
/*============================================================*/

#include "Mazesolver.h"

/******************************************************************************
* Function name: printAdj() - Support function
*
* Arguments: int **maze - 2D matrix with the maze
*            int edges - Total number of edges
*            AdjacencyList **print - Adjacency List 
*
* Return: void
*
* Side-effects: Stdout messages
*
* Description: Support function that prints to the stdout the Adjacency List
*             
*******************************************************************************/
void printAdj(int **maze, int edges, AdjacencyList **print){
    int a=0,b=0,i;
    AdjacencyList *helper=NULL;

    //Cycle to print the Adjacency List
    for(i=0;i<edges;i++){
        helper=print[i];
        printf("edge %d ",i);
        while(helper!=NULL){
            a = helper->adj; 
            b = helper->cost;
            printf("%d:%d ", a,b); //Adjacency and the respective cost
            //printf(":[%d %d] ",helper->wall[0],helper->wall[1]);
            helper=helper->next;   
        }
        printf("-1\n");//Print that represents the end of the list (pointer to NULL)
    }
}

/************************************************************************************
* Function name: printer() - Support function
*
* Arguments: int **maze - 2D matrix with the maze
*            int tl - Total lines
*            int tc - Total columns
*            AdjacencyList **print - Adjacency List
*            int edges - Total number of edges
*
* Return: void
*
* Side-effects: Stdout messages
*
* Description: Support function that prints to the stdout the 2D matrix with the maze
*             
*************************************************************************************/
void printer(int **maze, int tl, int tc, AdjacencyList **print, int edges){
    printf("\n");
    int i=0, l=0, c=0;

    //Print of the maze
    for(l=0;l<tl;l++){
        for(c=0;c<tc;c++){
            if((maze[l][c])==-1) 
                printf("B");
            else if(maze[l][c]==0) //Helpfull to check if there are rooms not visited
                printf("a");
            else if(maze[l][c]<-1)
                printf("%c",('a')-maze[l][c]-2);//Replace with V for larger mazes
            else
                printf("%d",maze[l][c]);
            if(maze[l][c]!=0)
                i++;
            }
            printf("\n");
        }
    printAdj(maze,edges,print);
}

/******************************************************************************
* Function name: clearmemory()
*
* Arguments: int **maze - 2D matrix with the maze
*            int tl - Total lines
*            int tc - Total columns
*            AdjacencyList **freeItem - Ajancency list 
*            int edges - Total number of edges
*
* Return: void
*
* Side-effects: none to report
*
* Description: Clears the memory allocated for all of the procedures made with
*              the intent of reaching the end goal of the project
*******************************************************************************/
void clearmemory(int **maze, int tl, int tc, AdjacencyList **freeItem, int edges){
    int i;
    AdjacencyList *helper=NULL, *templist=NULL;
    
    for(i=0;i<edges;i++){//Iterates through the adjacency list vector
        helper=freeItem[i];
        while(helper!=NULL){//Iterates through the list associated in the position 'i' of the vector
            templist=helper->next;
            free(helper);
            helper=templist;   
        }        
    }
    free(freeItem);
}

/***************************************************************************************
* Function name: whatway()
*
* Arguments: int **maze - 2D matrix with the maze
*            int **coord - Coordinates to be analyzed
*            int tl - Total lines
*            int tc - Total columns
*            int ignore - value of a room that should be ignore
*            int flag - In case we are analyzing a wall we can transverse in 2 ways (horizontal or vertical)  
* 
* Return: int - 1 when it is possible to move
*             - 0 when it is impossible to move
*
* Side-effects: Alters value of coord
*
* Description: This function will analyze how the wall can be broken, giving information
*              on how to connect 2 rooms
*
***************************************************************************************/
int whatway(int **maze, int **coord, int tl, int tc, int ignore, int flag){
    int l=(*coord)[0],c=(*coord)[1];
    if(flag==0){ //Analyze the vertical movement
        if((l!=0) && (l!=tl)) //Not the first or last line
        //Analyze freely
            if((maze[l-1][c]<-1) && (maze[l+1][c]<-1)){//This cell is breable
                //If the bottom cell is the room we already know or the top cell is not already a room we know
                if((maze[l-1][c]==ignore) || (maze[l+1][c]!=ignore)){
                    (*coord)[0]+=1; //move down
                    return 1;
                }
                //If the top cell is the room we already know or the bottom cell is not already a room we know 
                if((maze[l+1][c]==ignore) ||  (maze[l-1][c]!=ignore)){
                    (*coord)[0]-=1; //move up
                    return 1;
                }
            }
    }
    else if(flag==1){ //Analyze horizontal movement
        if((c!=0) && (c!=tc)) //Not the first or last line
        //Analyze freely
            if((maze[l][c-1]<-1) && (maze[l][c+1]<-1)){//This cell is breable
                //If the left cell is the room we already know or the right cell is not already a room we know
                if((maze[l][c-1]==ignore) || (maze[l][c+1]!=ignore)){
                    (*coord)[1]+=1; //move right
                    return 1;
                }
                //If the right cell is the room we already know or the left cell is not already a room we know
                if((maze[l][c+1]==ignore) || (maze[l][c-1]!=ignore)){
                    (*coord)[1]-=1; //move left
                    return 1;
                }
            }
        
    }
return 0; //No possible way to move
}

/***************************************************************************************
* Function name: addnewedge()
*
* Arguments: AdjacencyList ***find - adjacency list vector
*            int edgea - 1st edge to be added
*            int edgeb - 2nd edge to be added
*            int cost - cost of the connection of the edges
*            int wall[] - What wall connects them
* 
* Return: void
*
* Side-effects: none to report
*
* Description: This function will create a brand new node if edgea is not connected to
*              edgeb or update its cost if the new found cost is lower
*
***************************************************************************************/
void addnewedge (AdjacencyList ***find, int edgea, int edgeb, int cost, int wall[]){
    AdjacencyList *aux=NULL, *new=NULL;
    int flag=0;
    aux= (*find)[edgea];
    while(aux!=NULL){//We check if the edgea already is connected to edgeb
        if(aux->adj==edgeb){
            flag++;
            if(cost<aux->cost){//Update the cost if its lower and therefore the wall broken
                aux->cost=cost;
                aux->wall[0]=wall[0];
                aux->wall[1]=wall[1];
                break;
            }
        }
        aux=aux->next;//Go to the next edge
    }
    aux= (*find)[edgeb];
    while(aux!=NULL){
        if(aux->adj==edgea){//We check if the edgeb already is connected to edgea
            flag++;
            if(cost<aux->cost){//Update the cost if its lower and therefore the wall broken
                aux->cost=cost;
                aux->wall[0]=wall[0];
                aux->wall[1]=wall[1];
                break;
            }
        }
        aux=aux->next;//Go to the next edge
    }

    if(flag==0){//Edgea wasnt connected to b
    
        //Memory allocation for edge a
        if ((new = (AdjacencyList *) malloc(sizeof(AdjacencyList)))==NULL)
            exit(0);
            
        //Node initialization
        new->adj=edgeb;
        new->cost=cost;
        new->wall[0]=wall[0];
        new->wall[1]=wall[1];
        aux=(*find)[edgea];
        new->next=aux;
        (*find)[edgea]=new;

        //Memory allocation for edge b
        if ((new = (AdjacencyList *) malloc(sizeof(AdjacencyList)))==NULL)
            exit(0);
        
        //Node initialization
        new->adj=edgea;
        new->cost=cost;
        new->wall[0]=wall[0];
        new->wall[1]=wall[1];
        aux=(*find)[edgeb];
        new->next=aux;
        (*find)[edgeb]=new;  
    }

}

/***************************************************************************************
* Function name: createedges()
*
* Arguments: int **maze -  2D matrix with the maze
*            int tl - Total Lines
*            int tc - Total columns
*            int edges - Total number of edges
*
* Return: AjacencyList ** - Pointer to the adjacency List created
*
* Side-effects: none to report
*
* Description: This function is responsible for allocating memory for the adjacency
*              list and check how each node is connected
*
***************************************************************************************/
AdjacencyList **createedges(int **maze, int tl, int tc, int edges){
    int l,c, *inspect=NULL, a[2]={0,0},room[2]={0,0}, cost=0, ignore=0;
    AdjacencyList **adjVector = NULL;
    
    //Vector allocation
    if((adjVector= (AdjacencyList **) calloc(edges,sizeof(AdjacencyList *)))==NULL)
        exit(0);

    for(l=0;l<tl;l++)//Goes through the matrix
        for(c=0;c<tc;c++)
            if(maze[l][c]>0){//If we are in the presence of a gray cell
                a[0]=l;
                a[1]=c;
                cost=maze[l][c];
                inspect=a;
                if(whatway(maze,&inspect,tl-1,tc-1,INT_MIN,0)==1){//Check how the wall is connected with the rooms !=1 if its not connected
                    room[0]=-maze[inspect[0]][inspect[1]]-2;//Conversion of the room number
                    //For the next time we try to see where to move we ignore the room that we already know its connected
                    ignore=maze[inspect[0]][inspect[1]];
                    a[0]=l;
                    a[1]=c;
                    inspect=a;
                    whatway(maze,&inspect,tl-1,tc-1,ignore,0);//Gets the other room
                    room[1]=-maze[inspect[0]][inspect[1]]-2;
                    a[0]=l;
                    a[1]=c;
                    if(room[0]!=-2 && room[1]!=-2)//Add edge if its not an unconnected cell (a room that´s not marked)
                        addnewedge (&adjVector, room[0], room[1], cost, a);//adds the edge between the 2 rooms
                }
                a[0]=l;
                a[1]=c;
                cost=maze[l][c];
                inspect=a;
                if(whatway(maze,&inspect,tl-1,tc-1,INT_MIN,1)==1){//Check a second time if the wall is connected to any other 2 rooms
                    room[0]=-maze[inspect[0]][inspect[1]]-2;
                    //For the next time we try to see where to move we ignore the room that we already know its connected
                    ignore=maze[inspect[0]][inspect[1]];
                    a[0]=l;
                    a[1]=c;
                    inspect=a;
                    whatway(maze,&inspect,tl-1,tc-1,ignore,1);//Gets the other room
                    room[1]=-maze[inspect[0]][inspect[1]]-2;
                    a[0]=l;
                    a[1]=c;
                    if(room[0]!=-2 && room[1]!=-2)//Add edge if its not an unconnected cell (a room that´s not marked)
                        addnewedge (&adjVector, room[0], room[1], cost, a);//adds an edge between the 2 rooms
                }
            }
    
    return adjVector;
}

/*****************************************************************************************
* Function name: solve()
*
* Arguments: int **maze - 2D matrix containing Maze
*            int tl - Total Lines
*            int tc - Total columns
*            int des[] - destination coordinates
*            FILE *fp - fp to the output file
*
* Return: AjacencyList adjVector - AdjacencyList
*
* Side-effects: The 0´s in the matrix will be replaced by negative numbers representing the rooms
*
* Description: This function is responsible for going through the maze matrix.
*              If the cell visited is a white cell it will call a function to find
*              the room in which the cell is inserted
*
*****************************************************************************************/
void solve(int **maze, int tl, int tc, int des[], FILE *fp){
    int start[2]={1,1}, count=-2, a[2]={0,0}, destination=0, l=0, c=0;
    AdjacencyList **adjvec=NULL;
    //We start by analyzing if the starting cell and destination cell can move
    if(((type_analysis('2',maze,des,des,tl,tc)==0)&&(type_analysis('3',maze,des,des,tl,tc)==0)) ||
    ((type_analysis('2',maze,start,des,tl,tc)==0)&&(type_analysis('3',maze,start,des,tl,tc)==0))){
        fprintf(fp,"-1\n\n\n");//Not connect
        return;
    }
    startA6(maze,start,a,tl,tc,-2,0);//start by seeing the room in which the starting cell is inserted
    if(maze[des[0]-1][des[1]-1]==-2){//Checks if the destination cell is in the same as the starting cell
        fprintf(fp,"0\n\n\n");//The 2 cells are connected
        return;
    }
    count--;
    //For each position of the matrix we will see if we have a cell equal to 0 meaning its unvisited,
    //We will then mark the room in which it is inserted
    for(l=1;l<=tl;l++){
        for(c=1;c<=tc;c++){
            if(maze[l-1][c-1]==0){
                start[0]=l;
                start[1]=c;
                a[0]=tl+1;
                a[1]=tc+1;
                startA6(maze,start,a,tl,tc,count,0);//make a room using A6 for the line and collumn analyzed
                count--;
            }
        }
    }
    //The destination room is marked with a negative number that can start in -2
    //So we know the room we are suposed to reach
    destination=-maze[des[0]-1][des[1]-1]-2;
    adjvec = createedges(maze,tl,tc,-count-2);//Creates an adjacency list
    djikstra(adjvec,destination,-count-2,fp);//Solves the connectivity problem using djikstra
    clearmemory(maze,tl,tc,adjvec,-count-2);
    return;
}
