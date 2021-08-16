#include "util.h"

void print_elem(const gfe_p256k1_4L *e){

	uchar8  i;

	for (i=NLIMBS-1; i>0; --i) 
		fprintf(FILE,"%16llX ",e->l[i]);
	fprintf(FILE,"%16llX \n\n",e->l[0]);
}

void set_values(uchar8 *echar8, gfe_p256k1_10L *e10L, gfe_p256k1_4L *e4L, const int src){
	if (src == 0) {
		gfp256k1pack10(e10L, echar8);
		gfp256k1pack104(e4L, e10L);
	}

	if (src == 1) {
		gfp256k1pack104(e4L, e10L);
	}

	gfp256k1unpack(echar8, e4L);
	gfp256k1pack10(e10L, echar8);
	gfp256k1pack104(e4L, e10L);
}

void print_vector(const vec *V) {
	gfe_p256k1_4L res4L;
	gfe_p256k1_10L res10L;

	int i, j;
	for(j=0; j<4; j++) {
		for (i=0; i<NLIMBS_VEC_10; i++) {
			res10L.l[i] = V[i][j];
		}

		gfp256k1pack104(&res4L,&res10L);

		gfp256k1makeunique(&res4L);

		fprintf(FILE,"%d hex:\t\t",j);print_elem(&res4L);
	}
}