#ifndef __P256k1_UTIL_H__
#define __P256k1_UTIL_H__

#include <stdio.h>
#include <math.h>
#include "basic_types.h"
#include "gf_p256k1_type.h"

#define STDOUT stdout

void print_elem(const gfe_p256k1_4L *);
void print_vector(const vec *);
void set_values(uchar8 *, gfe_p256k1_10L *, gfe_p256k1_4L *, const int);

#endif
