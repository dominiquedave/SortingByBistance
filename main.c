//Written by Dave Dominque
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include "leak_detector_c.h"

typedef struct coordinate {
  int x;
  int y;
  int distance;
  int rank;
}coordinate;

int myLocationX;
int myLocationY;

int distance(int myLocationX, int myLocationY, coordinate* point)
{
  int distnce = sqrt( (point->x - myLocationX )*(point->x - myLocationX) +
                        (point->y - myLocationY )*(point->y - myLocationY)
                       );

  return round(distnce);
}

coordinate* readData(FILE* infile, int myLocationX, int myLocationY, int num_points)
{
  //unsorted array of coordinates
  coordinate* coordArr = malloc(sizeof(coordinate) * num_points);

  //filling the coordinates array with points 
  for(int pi = 0; pi < num_points; pi++)
  {
    fscanf(infile, "%d %d", &coordArr[pi].x, &coordArr[pi].y);
    coordArr[pi].distance = distance(myLocationX,myLocationY, &coordArr[pi]);
  }

  return coordArr;
}

int compareTo(coordinate* ptrPt1, coordinate* ptrPt2)
{
   //same distances same x's same y's
  if( (ptrPt1->distance == ptrPt2->distance) && (ptrPt1->x == ptrPt2->x) && ptrPt1->y == ptrPt2->y)
  {
        return 0;
  }

  //same distances same x's diff y's
  if(ptrPt1->distance == ptrPt2->distance && ptrPt1->x == ptrPt2->x )
  {
      if(ptrPt1->y < ptrPt2->y)
        return -1;
     
      else
        return 1;
  }

  //same distances but different x's
  if(ptrPt1->distance == ptrPt2->distance)
  {
     if(ptrPt1->x < ptrPt2->x )
      return -1;
  
     else
      return 1;
  }


  //different distances
  if(ptrPt1->distance < ptrPt2->distance)
    return -1;
  
  else 
    return 1;
}

void insertionSort(coordinate* list, int first, int last)
{ 
  int i, hole; coordinate pair;

  for(i = first +1 ; i <= last; i++)
  {
    pair = list[i];

    for (hole = i -1; hole>= first; hole--)
    {
      if (compareTo(&list[hole], &pair) == 1 )
         { 
          list[hole+1] = list[hole]; 
         }
      else 

        break;
    }

    list[hole+1] = pair;
  }
}

void merge(coordinate* list, int first, int mid, int last)
{

  int leftIndex, rightIndex, k;

  int leftLen = mid - first + 1;
  int rightLen = last - mid;

  coordinate* leftArr = malloc(sizeof(coordinate) * leftIndex );
  coordinate* rightArr = malloc(sizeof(coordinate) * rightLen );

  for (leftIndex = 0; leftIndex < leftLen; leftIndex++)
  {
    leftArr[leftIndex] = list[first + leftIndex];
  }

  for (rightIndex = 0; rightIndex < rightLen; rightIndex++)
  {
    rightArr[rightIndex] = list[mid + 1+ rightIndex];
  } 

  /* Merge the temp arrays back into arr[l..r]*/
  leftIndex = 0; // Initial index of first subarray
  rightIndex = 0; // Initial index of second subarray
  k = first; // Initial index of merged subarray
 
  while (leftIndex < leftLen && rightIndex < rightLen)
  {
    if(compareTo(&leftArr[leftIndex], &rightArr[rightIndex]) == 1)
    {
      list[k] = rightArr[rightIndex];
      rightIndex++;
    }

    else
    {
      list[k] = leftArr[leftIndex];
      leftIndex++;
    }

    k++;
  }

  while(leftIndex < leftLen)
  {
    list[k] = leftArr[leftIndex];
    leftIndex++;
    k++;
  }
  
  while(rightIndex < rightLen)
  {
    list[k] = rightArr[rightIndex];
    rightIndex++;
    k++;
  }
    free(leftArr);
    free(rightArr);
}

void mergeSort(coordinate list[], int first, int last, int t)
{
    if ((last-first) <= t)
    {
      insertionSort(list, first, last);
    }
    
    else if(first<last)
    {
      int mid = (first + last) / 2;
      mergeSort(list, first, mid, t);
      mergeSort(list, mid+1, last, t);
      
      merge(list, first, mid, last);
    }
}  

coordinate* sortAlg(FILE* outfile, coordinate list[], int len, int t)
{
  int first = 0;
  int last = len - 1;

  mergeSort(list, first, last, t);

  for(int i = 0; i < len; i++)
  {
    list[i].rank = i + 1;
    printf("%d %d\n", list[i].x, list[i].y);
    fprintf(outfile,"%d %d\n", list[i].x, list[i].y);
  }

 return list;
}



coordinate* Queries(FILE* infile, int searchX)
{
   //array of coordinates used for queries
   coordinate* queries = malloc(sizeof(coordinate) * searchX );
   
   for(int si = 0; si < searchX; si++)
  {
    fscanf(infile, "%d %d", &queries[si].x, &queries[si].y);
    queries[si].distance = distance(myLocationX,myLocationY, &queries[si]);
  }
  return queries;
}

void querySearch(FILE* outfile, coordinate* list, coordinate targePoint, int len)
{
  int l = 0, h = len -1 ;
  int mid;
  while(l<=h)
  {
    mid = (l+h)/2;
    int hiearchy = compareTo(&targePoint,&list[mid]);

    if(hiearchy == 0)
    {
      printf("%d %d found at rank %d\n", list[mid].x,list[mid].y, list[mid].rank);
      fprintf(outfile,"%d %d found at rank %d\n", list[mid].x, list[mid].y, list[mid].rank);
      return;
    }
    
    //might swap
    if(hiearchy == -1)
    {
     h = mid -1;
    }
    else
      l = mid + 1;
  }
  //what should i return
  printf("%d %d not found\n", targePoint.x, targePoint.y);
  fprintf(outfile,"%d %d not found\n", targePoint.x, targePoint.y);
  return;
}

void querySearchRE(FILE* outfile, coordinate* list, int first, int last, coordinate targePoint)
{
  if( first > last)
  {
   printf("%d %d not found\n", targePoint.x, targePoint.y);
   fprintf(outfile,"%d %d not found\n", targePoint.x, targePoint.y);
    return;
  }
  
  int mid = (last + first) / 2;
  int hiearchy = compareTo(&targePoint,&list[mid]);

  if(hiearchy == 0)
  {
   printf("%d %d found at rank %d\n", list[mid].x, list[mid].y, list[mid].rank);
    fprintf(outfile,"%d %d found at rank %d\n", list[mid].x, list[mid].y, list[mid].rank);
   
    return;
  }

  if(hiearchy == -1)
  {
    querySearchRE(outfile, list, first, mid -1, targePoint);
  }

  else if (hiearchy == 1)
  {
    querySearchRE(outfile,list, mid + 1, last, targePoint);
  }
}



int main(void) {

 atexit(report_mem_leak);
 FILE* infile = fopen("in.txt", "r");
 FILE* outfile = fopen("out.txt", "w");

 int myLo_x, myLo_y;
 int num_points;
 int t;
 int searchX;

 if (infile != NULL)
 {
  //scans in the first five ints in the file
  fscanf(infile, "%d %d %d %d %d", &myLo_x, &myLo_y, &num_points, &searchX, &t );
    
  //establshing myLocation
  myLocationX = myLo_x;
  myLocationY = myLo_y;
  
  //generating my list of coordinates
  coordinate* list = readData(infile,  myLocationX, myLocationY, num_points);

  //sorting my unsorted list 
  coordinate* sortedList = sortAlg(outfile, list, num_points, t);
  // for(int i = 0; i < num_points; i++)
  // {
  //   printf("%d %d\n", sortedList[i].x, sortedList[i].y);
  //   fprintf(outfile,"%d %d\n", sortedList[i].x, sortedList[i].y);
  // }

  //generating my list of queries
  coordinate* queries = Queries(infile, searchX);

//   printf("\n");
//   //searching queries querySearchRE
//   int first = 0;
//   int last = num_points-1;
//   for(int i = 0; i < searchX; i++)
//   {
//     querySearchRE(outfile, sortedList, first, last, queries[i]);
//   }

//   printf("\n");
  //searching queries querySearch
  for(int i = 0; i < searchX; i++)
  {
    querySearch(outfile, sortedList, queries[i], num_points);
  }

  free(list);
  free(queries);

  fclose(infile);
 }
 
  else
  {
      printf("File could not be read\n");
      fprintf(outfile,"File could not be read\n");
      exit(-1);
  }
  
  fclose(outfile);
  return 0;
}
