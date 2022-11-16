/*==============================================================================================================*/
/*Program file: djikstra.c                                                                                      */
/*Brief: Executes the djikstra algorithm responsible for solving the maze                                       */
/*                                                                                                              */
/*Description: This file is responsible for running the djikstra algorithm applied to an adjacency list of the  */
/*             connected rooms, this will be done using a binary heap                                           */
/*                                                                                                              */
/*Dependencies: Adjacency list should be created                                                                */
/*                                                                                                              */
/*Authorship: Eduardo Filipe Braz Barata 99930, IST-LEEC                                                        */
/*            Joao Pedro Sousa Batista   99994, IST-LEEC                                                        */
/*                                                                                                              */
/* Webgraphy: https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8 */
/*                                                                                                              */          
/*                                                                                             Last mod:11/11/21*/
/*==============================================================================================================*/

#include "djikstra.h"

#define swapMinHeapNode(a,b) { MinHeapNode* t = *a;  *a = *b;  *b = t; }

// Structure to represent a min heap node
struct MinHeapNode{
    int  room;
    unsigned long int cost;
};
 
// Structure to represent a min heap
struct MinHeap{
    int size;    
    int totalsize; 
    int *currentpos;   
    struct MinHeapNode **arr;
};
 
/*********************************************************************************
* Function name: newMinHeapNode()
*
* Arguments: int room - Number of the room to be placed
*            int cost - Cost to get to the room
*
* Return: MinHeapNode* - pointer to the new node created
*
* Side-effects: none to report
*
* Description: This function will allocate memory for a new node for the minHeap,
*              assigning the values to their correct positions
*
**********************************************************************************/
MinHeapNode* newMinHeapNode(int room, int cost){
    MinHeapNode* new = NULL;
    if((new = (MinHeapNode*) malloc(sizeof(MinHeapNode)))==NULL)//Allocates a new Node
        exit(0);
    //Assigns values
    new->room = room;
    new->cost = cost;
    return new;
}
 
/*********************************************************************************
* Function name: createMinHeap()
*
* Arguments: int edges - total number of edges (Heap will have this size)
*
* Return: MinHeap* - pointer to the new MinHeap created
*
* Side-effects: none to report
*
* Description: This function will allocate memory for a new MinHeap, calling 
*              a function to create each node and assigning the values to their 
*              correct positions
*
**********************************************************************************/
MinHeap* createMinHeap(int edges){
    MinHeap* newHeap = NULL;
    if((newHeap = (MinHeap*) malloc(sizeof(MinHeap)))==NULL) //Allocates a MinHeap
        exit(0);
    newHeap->currentpos=NULL;
    if((newHeap->currentpos = (int *) malloc(edges*sizeof(int)))==NULL)//Allocates a vector of MinHeapnodes
        exit(0);
    //Inicializes values
    newHeap->size=0;
    newHeap->totalsize=edges;
    newHeap->arr=NULL;
    if((newHeap->arr = (MinHeapNode**) malloc(edges*sizeof(MinHeapNode*)))==NULL)//Allocates a vector to keep track of positions
        exit(0);
    return newHeap;
}

/*********************************************************************************
* Function name: minHeapify()
*
* Arguments: MinHeap* minHeap - Pointer to the minHeap
*            int index        - Index to be heapified
*
* Return: void
*
* Side-effects: none to report
*
* Description: This function will heapify a certain index, this meaning that
*              it will start in the top position and where the value fits 
*              according to the condition that the "father" cost of this position 
*              should be lower than the cost of the index. (this will be done in O(Log(n))
*
**********************************************************************************/
void minHeapify(MinHeap* minHeap,int index){
    int smallest=0, child1=0, child2=0;
    MinHeapNode *smallestnode=NULL, *aux=NULL;
    smallest=index;

    for( ; ; ){//Infinite loop, it will end when the index is its correct position
        child1=2*index+1;//get the left child
        child2=2*index+2;//get the right child
        //Checks if the left/right child is still inside the minHeap and if it is if the cost
        //of getting to that room is lower then the cost of going to the current smallest room 
        if (child1< minHeap->size && (minHeap->arr[child1]->cost < minHeap->arr[smallest]->cost))
            smallest= child1;
        if (child2< minHeap->size && (minHeap->arr[child2]->cost < minHeap->arr[smallest]->cost))
            smallest= child2;
        //Breaking condition: The parent node is lower than its child's
        if (smallest==index)
            break;
        //The parent and child node need to be swapped so we will
        //get a pointer to their positions
        smallestnode = minHeap->arr[smallest];
        aux = minHeap->arr[index];
        //Swap the current position vector to mantain a O(1) acess to the queue
        minHeap->currentpos[smallestnode->room] = index;
        minHeap->currentpos[aux->room] = smallest;
        //Swap the nodes using macro
        swapMinHeapNode(&minHeap->arr[smallest],&minHeap->arr[index]);
        index=smallest; //Do the same search for the child node
    }
}

/*********************************************************************************
* Function name: getminimumvalue()
*
* Arguments: MinHeap* minHeap - Pointer to the minHeap
*
* Return: MinHeapNode* - Pointer to the first (and smallest) element of the Heap
*
* Side-effects: The Smallest element will now be in the last position and the Heap
*               will have its size decreased
*
* Description: This function will extract the minimum node of the heap
*              returning a pointer to it. To do this, it will swap the first
*              node with the last one and it will then "cut" this last node from
*              the queue
*              
**********************************************************************************/
MinHeapNode* getminimumvalue(MinHeap* minHeap){
    MinHeapNode* first=NULL, *lastnode=NULL;
    if (minHeap->size==0)
        return NULL;
    //Decreases the size of the Heap because we will "cut" the last element 
    minHeap->size--;
    first=minHeap->arr[0]; // Store the first node
    lastnode=minHeap->arr[minHeap->size]; // Replace first node with last node
    minHeap->arr[0]=lastnode;
 
    //Update the current position of last node
    minHeap->currentpos[first->room]=minHeap->size;
    minHeap->currentpos[lastnode->room]=0;
    minHeapify(minHeap, 0); //heapify starting on the first node
    return first;
}
 
/****************************************************************************
* Function name: decreaseKey()
*
* Arguments: MinHeap* minHeap - Pointer to the minHeap
*            int room - The room to be updated
*            int cost - The new cost for acessing the room
*
* Return: void
*
* Side-effects: none to report
*
* Description: Function that will update the cost (to a lower one) to get to 
*              a room and maintain the order of the table content
*              
*****************************************************************************/
void decreaseKey(MinHeap* minHeap,int room, int cost){
    int i=0;
    i=minHeap->currentpos[room]; // Get the index of the room in  heap array
    minHeap->arr[i]->cost=cost; //Update the cost of the room
    //Do a fix up while the cost if lower then its parent
    while(i && (minHeap->arr[i]->cost < minHeap->arr[(i-1)/2]->cost)){
        //Swap this node with its parent
        minHeap->currentpos[minHeap->arr[i]->room]=(i-1)/2;
        minHeap->currentpos[minHeap->arr[(i-1)/2]->room]=i;
        swapMinHeapNode(&minHeap->arr[i], &minHeap->arr[(i-1)/2]);
        //Move to parent index
        i=(i-1)/2;
    }
}
 
/*********************************************************************************
* Function name: djikstra()
*
* Arguments: AdjacencyList** adjvec - Adjacency list vector
*            int des - Destination room
*            int edges - Total number of rooms
*            FILE *fp - Pointer of the output file
*
* Return: void
*
* Side-effects: none to report
*
* Description: Function that will find the lowest cost path between the destination
*              room and the first room (the one where the cell 1 1 is locatated).
*              This will be done using djikstra algorithm             
*              
**********************************************************************************/
void djikstra(AdjacencyList** adjvec, int des, int edges, FILE *fp){ 
    
    unsigned long int *cost=NULL;
    int *previous=NULL, v=0, u=0, numberofroom=0, i=0;
    AdjacencyList *aux=NULL;
    MinHeap* minHeap = NULL;
    MinHeapNode* minHeapNode=NULL;
    
    minHeap=createMinHeap(edges); //Creates a minHeap with the number of edges
    if((cost=malloc(edges*sizeof(unsigned long int)))==NULL)//Create a cost vector
        exit(0);
    if((previous=malloc(edges*sizeof(int)))==NULL)//Create a vector to know which cells where visited
        exit(0);

    minHeap->size=edges; //minHeap has a size of the total edges
    for (i=0;i<edges;i++){
        cost[i] = ULONG_MAX; //All cells are initially at an infinite distance from the source
        minHeap->arr[i]=newMinHeapNode(i,cost[i]); //Create a Node for each edge
        minHeap->currentpos[i]=i; //Each edge is currently in their "correct" position
        previous[i]=-1; //Currently there is no previous room visited (no room was visted)
    }

    minHeap->currentpos[des]=des;
    cost[des]=0;//The cost to get to get to the 1st room is 0
    decreaseKey(minHeap,des,cost[des]);//Update the table so the 1st room is on top
    while (minHeap->size!=0){//While there are still nodes to be analyzed
        minHeapNode=getminimumvalue(minHeap); //gets the room with lost cost to reach yet still in the Heap
        u=minHeapNode->room;
        free(minHeapNode);//Free the element that we got
        if(u==0)//If we are in the 1st room we found the lowest cost path 
            break;
        //Go through all the adjacent vertices of u and update their cost values
        aux=adjvec[u];
        while(aux!=NULL){
            v = aux->adj;
            // If shortest distance to v is not finalized yet, and distance to v through u is lower 
            //Than its previously calculated cost
            if ((minHeap->currentpos[v] < minHeap->size) && cost[u] != ULONG_MAX && aux->cost + cost[u] < cost[v]){
                cost[v] = cost[u] + aux->cost;
                previous[v]=u;
                //Update the distance value in the min heap
                decreaseKey(minHeap, v, cost[v]);
            }
            aux = aux->next;//Get the next connect room
        } 
    }
    //Correct memory free
    for(v=0;v<minHeap->size;++v){
        free(minHeap->arr[v]);
    }
    free(minHeap->currentpos);
    free(minHeap->arr);
    free(minHeap);
 
    v=0;
    if(cost[0]!=ULONG_MAX){//If the cost to the 1st room is not infinite
        for(;;){//Get the total number of rooms explored for the lowest solution
            numberofroom++;
            if(previous[v]==des)//We have reached the destination
                break;
            v=previous[v];
        }    
        v=0;
        fprintf(fp,"%lu\n%d\n",cost[0],numberofroom);
        for(;;){
            aux = adjvec[v];//Get the pointer to the room where v is inserted
            while(aux->adj!=previous[v])//Get the wall that connects the rooms
                aux=aux->next;
            fprintf(fp, "%d %d %d\n", aux->wall[0]+1, aux->wall[1]+1, aux->cost);
            if(previous[v]==des)//Destination was reached
                break;
            v=previous[v];//Get the next room to be printed
        }
    }
    else
        fprintf(fp,"-1");
    fprintf(fp,"\n\n");
    free(previous);
    free(cost);

}



