#ifndef QSORT_H
#define QSORT_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define UNLIMIT
#define QSORT_MAXARRAY                                                         \
  1000 /* this number, if too large, will cause a seg. fault!! */

struct my3DVertexStruct {
  int x, y, z;
  uint32_t distance;
};

int qsort_compare(const void *elem1, const void *elem2);

#endif
