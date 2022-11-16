/*=========================================================================*/
/*Program file: Movingalgorithm.c                                          */
/*Brief: Contains the code for the moving algorithm                        */
/*                                                                         */
/*Description: This file is responsible for receiving 2                    */
/*             coordinates and check if they are in the                    */
/*             room of a maze. For doing so, it will                       */
/*             use the DFS algortihm, using iteraction and                 */
/*             a First-In-First-Out stack                                  */
/*                                                                         */
/*Dependencies: Depending on the size of the problem,                      */
/*              it can occur stack overflow                                */
/*                                                                         */
/*Webgraphy: https://bit.ly/3aIU1dH                                        */
/*           https://www.geeksforgeeks.org/iterative-depth-first-traversal */
/*           https://www.techiedelight.com/depth-first-search/             */
/*                                                                         */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC                   */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC                   */
/*                                                                         */
/*                                                         Last mod:5/11/21*/
/*=========================================================================*/

#include "Movingalgorithm.h"

struct StackNode{
    int coord[2];
    struct StackNode* next;
};
 
 /******************************************************************************
* Function name: newNode() 
*
* Arguments: ins save[] - coordinates of the cell analyzed and to be saved         
*
* Return: stackNode 
*
* Side-effects: none to report 
*
* Description: Alocates a new node in the stack to contain the coordinate 
*              analyzed                           
*
*******************************************************************************/
StackNode* newNode(int save[]){
    StackNode* stackNode = NULL;
    if ((stackNode = (StackNode*) malloc(sizeof(StackNode)))==NULL)//Allocates memory for the new node
        exit(0);
    //Puts the value of the cell to be saved on the stackNode coords
    stackNode->coord[0] = save[0];
    stackNode->coord[1] = save[1];

    stackNode->next = NULL;
    return stackNode;
}

/******************************************************************************
* Function name: push()
*
* Arguments: int data[] - coordinates to be stored
*            StackNode** previous - Last element of the stack
*
* Return: void
*
* Side-effects: Previous will be updated with the new node created 
*
* Description: This function will call another to create a new stack node
*              and it will make the necessary links and update the 
*              "previous" variable with sayed new node           
*
*******************************************************************************/
void push(StackNode** previous, int data[]){
    StackNode* stackNode = newNode(data); //Allocates memory for the new node and saves the cell´s data
    stackNode->next = *previous; //Pointer for the next node is set to be pointing at the previous allocated node
    *previous = stackNode;//Update the previous node with the new created
}
 
/******************************************************************************
* Function name: pop()
*
* Arguments: StackNode** root - last element of the stack       
*
* Return: void
*
* Side-effects: Deletes from memory the last node from the stack
*
* Description: This function will pop a node from the stack (if it exists)
*              and will update the root variable to the "previous" element
*              in the stack.            
*
*******************************************************************************/
void pop(StackNode** root){
    if ((*root)==NULL) //If the node is already NULL(inexistent), return
        return;
    //Create a temporary pointer to be possible to free the intended node
    StackNode* temp = *root; 
    *root = (*root)->next;//Update root with the previous created node
    free(temp);

}

/******************************************************************************
* Function name: popall()
*
* Arguments: StackNode* Head - Last element of the stack        
*
* Return: void
*
* Side-effects: Deletes all stack
*
* Description: This function will delete all the stack allocated. Starting
*              with the last element created.             
*
*******************************************************************************/
void popall(StackNode* Head){
    while(Head!=NULL){//Goes through the stack and popps all its elements
        pop(&Head);
    }
}

/******************************************************************************
* Function name: direction()
*
* Arguments: int **maze - The matrix containing the maze
*            int **coord - coordinates to be analyzed
*            int tl - total number of lines
*            int tc - total number of columns
* Return:  int - 1 if he can move to a cell
*                0 if he in unable to move to another cell that is unvisited
*
* Side-effects: None to report
*
* Description: This function will analyze in wich direction the current cell 
*              can move, updating the vector containing said coordinates
*              with that direction              
*
*******************************************************************************/
int direction (int **maze, int **coord,int tl, int tc){
    int l=(*coord)[0],c=(*coord)[1];
    if((l+1)<tl)//Analyzes if its not the last line
        if(maze[l+1][c]==0){    //Analyzes if the bottom cell is white
            (*coord)[0]+=1;     //Updates the coordinates so the cell moves to the bottom
            return 1;
        }
    if((l-1)>=0)//Analyzes if its not the first line
        if(maze[l-1][c]==0){    //Analyzes if the top cell is white
            (*coord)[0]+=-1;    //Updates the coordinates so the cell moves to the top
            return 1;
        }
    if((c+1)<tc)//Analyzes if its not the last column
        if(maze[l][c+1]==0){    //Analyzes if the right cell is white
            (*coord)[1]+=1;     //Updates the coordinates so the cell moves to the right
            return 1;
        }
    if((c-1)>=0)//Analyzes if its not the first column
        if(maze[l][c-1]==0){    //Analyzes if the left cell is white
            (*coord)[1]+=-1;    //Updates the coordinates so the cell moves to the left
            return 1;
        }
return 0;//No possible way to move
}


/******************************************************************************
* Function name: startA6()
*
* Arguments: int **maze - The matrix containing the maze
*            int start[] - 1st cell coordinates
*            int dest[] - 2nd cell coordinates
*            int tl - total number of lines in the maze
*            int tc - total number of columns in the maze
*            int mark - Number with which we will "paint" the rooms
*            int flag - check which type of input file we are using and then 
*                       proceed with the proper type of execution
* Return: int - 0 no connection 1 there is a connection
*
* Side-effects: none to report
*
* Description: This function is responsible for all the logic in the DFS
*              algorithm, pushing coordinates into the stack and popping
*              them when they are needing, analyzing if it has reached its
*              goal (the 2nd set of coordinates)
*
*******************************************************************************/
int startA6(int **maze, int start[],int dest[], int tl, int tc, int mark, int flag){
    StackNode *analyze=NULL;

    //Updates the coordinates to make sense with the matrix used to store the values
    start[0]-=1;
    start[1]-=1;
    dest[0]-=1;
    dest[1]-=1;
    if(flag)//For -s mode
        if((maze[start[0]][start[1]]!=0)||(maze[dest[0]][dest[1]]!=0)) //If the starting or destination cells aren´t a white cell, the answer is negative
            return 0;                                                  
    maze[start[0]][start[1]]=mark; //Marks the starting cell as visited
    analyze=newNode(start); //Places the starting cell in the stack
    while(analyze!=NULL){ //While the stack isn't empty there is still paths to be explored
        start[0]=analyze->coord[0]; //Gets the coordinates from the last node in the stack
        start[1]=analyze->coord[1];
        if((start[0]==dest[0])&&(start[1]==dest[1])){ //We have reached the destination cell
            popall(analyze);//Pop all elements in the stack
            return 1;//Return an affirmative answer
        }
        if((direction(maze,&start,tl,tc))==0) //The current cell can't move in any direction
            pop(&analyze);//Pop this cell from the stack
        else{
            maze[start[0]][start[1]]=mark; //Mark as visited
            push(&analyze,start); //Push the visited coordinate in the stack 
        }        
    } 
    return 0;
}
