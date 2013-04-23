#include <math.h>
#include <vector>

#define polygon std::vector< decart* >

struct decart
{
   double x;
   double y;
   double z;
};

double square_distance (struct decart *place1, struct decart *place2);
double distance (struct decart *place1, struct decart *place2);
decart *do_vector_from_null(decart *beg_vec,decart *end_vec);
bool isInside(struct decart *point, polygon pol);
double proect(decart *vect, decart *axis);
