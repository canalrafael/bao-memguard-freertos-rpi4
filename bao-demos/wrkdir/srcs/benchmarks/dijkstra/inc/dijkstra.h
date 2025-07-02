#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES                          10//100
#define NONE                               99//9999

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;


extern int g_qCount;



void print_path (NODE *rgnNodes, int chNode);
void enqueue (int iNode, int iDist, int iPrev);
void dequeue (int *piNode, int *piDist, int *piPrev);
int qcount (void);
int dijkstra(int chStart, int chEnd);

#endif
