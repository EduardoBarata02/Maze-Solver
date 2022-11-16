/*===========================================================*/
/*Program: RoaP Game                                         */
/*                                                           */
/*Description: A program that analyzes mazes and answers     */
/*             to questions asked by the user about a        */
/*             certain set of coordinates.                   */
/*                                                           */
/*Files:                                                     */
/*  (1)  Commandline.c                                       */
/*  (2)  Data collection.c                                   */
/*  (3)  Type.c                                              */
/*  (4)  Movingalgorithm.c                                   */
/*  (5)  Header.h                                            */
/*  (6)  Type.h                                              */
/*  (7)  Commandline.h                                       */
/*  (8)  DataCollection.h                                    */
/*  (9)  Movingalgorithm.h                                   */
/*  (10) Mazesolver.c                                        */
/*  (11) Mazesolver.h                                        */
/*  (12) Ddjikstra.c                                         */
/*  (13) Ddjikstra.h                                         */
/*  (14) Makefile                                            */
/*                                                           */
/* UC: AED - IST-LEEC - 2nd year - 1st period                */
/*                                                           */
/*Autorship: Eduardo Filipe Braz Barata 99930, IST-LEEC      */
/*           Joao Pedro Sousa Batista   99994, IST-LEEC      */
/*                                                           */
/*                                          Last mod:11/11/21*/
/*===========================================================*/
#ifndef RoaP_h_included
#define RoaP_h_included

#include <stdio.h>

typedef struct AdjacencyListstruct{
    int adj;
    int cost;
    short wall[2];
    struct AdjacencyListstruct *next;
}AdjacencyList;

//Functions declaration
void fileopener(char *inputname, int flag);
int type_analysis(char type, int **maze, int destination[], int destination2nd[], int totallines, int totalcolumns);
int startA6(int **maze, int start[],int dest[], int tl, int tc, int mark, int flag);
int celloutofmaze(int a, int b,int maxlines, int maxcollumns);
void solve(int **maze, int tl, int tc, int des[], FILE *fp);
int iscellbreakable(int analyzel, int analyzec, int **maze, int maxline, int maxcolumn);
void djikstra(AdjacencyList** graph, int des, int edges, FILE *fp);

#endif // RoaP_included
