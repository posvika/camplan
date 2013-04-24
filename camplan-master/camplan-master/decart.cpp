#include <math.h>
#include <vector>
#include <stdio.h>  //for perror

#include "decart.hpp"

double square_distance (struct decart *place1, struct decart *place2)
{
    decart *dist = new (decart);
    dist->x=pow(place1->x - place2->x,2);
    dist->y=pow(place1->y - place2->y,2);
    dist->z=pow(place1->z - place2->z,2);
    return (dist->x + dist->y + dist->z);
}

double distance (struct decart *place1, struct decart *place2)
{
    return pow(square_distance(place1,place2),2);
}

bool isInside(struct decart *point, polygon pol)
{
   int i, size=pol.size(), counter=0;
   if (size < 3) perror ("memory broblem, polygom failed\n");
   bool prev, curr;
   for (i=1;i<size;i++)
   {
      if (point->x > pol[i]->x)
       continue;
      prev = (pol[i-1]->y > 0)?true:false;
      curr = (pol[i]->y > 0)? true:false;
      if (prev!=curr) counter++;
   }
   if (counter%2==0)
    return false;
   else
    return true;
}

decart *do_vector_from_null(decart *beg_vec,decart *end_vec)
{
    decart *temp=new(decart);
    temp->x = end_vec->x - beg_vec->x;
    temp->y = end_vec->y - beg_vec->y;
    temp->z = end_vec->z - beg_vec->z;
    return temp;
}

double proect(decart *vect, decart *axis)
{
    decart null_point;
    null_point.x=0; null_point.y=0; null_point.z=0;
    double angle=0;
    double a,b,c;
    a=square_distance(axis,vect);
    b=square_distance(&null_point,axis);
    c=square_distance(&null_point,vect);
    angle=(pow(a,2)+pow(b,2)+pow(c,2))/(2*b*c);
    return sqrt(c)*cos(angle);
}
