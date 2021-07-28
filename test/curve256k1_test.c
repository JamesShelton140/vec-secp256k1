/*
+-----------------------------------------------------------------------------+
| This code corresponds to the the paper "Efficient 4-way Vectorizations of   |
| the Montgomery Ladder" authored by   			       	       	      		  |
| Kaushik Nath,  Indian Statistical Institute, Kolkata, India, and            |
| Palash Sarkar, Indian Statistical Institute, Kolkata, India.	              |
+-----------------------------------------------------------------------------+
| Copyright (c) 2020, Kaushik Nath and Palash Sarkar.                         |
|                                                                             |
| Permission to use this code is granted.                          	      	  |
|                                                                             |
| Redistribution and use in source and binary forms, with or without          |
| modification, are permitted provided that the following conditions are      |
| met:                                                                        |
|                                                                             |
| * Redistributions of source code must retain the above copyright notice,    |
|   this list of conditions and the following disclaimer.                     |
|                                                                             |
| * Redistributions in binary form must reproduce the above copyright         |
|   notice, this list of conditions and the following disclaimer in the       |
|   documentation and/or other materials provided with the distribution.      |
|                                                                             |
| * The names of the contributors may not be used to endorse or promote       |
|   products derived from this software without specific prior written        |
|   permission.                                                               |
+-----------------------------------------------------------------------------+
| THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY EXPRESS OR       |
| IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES   |
| OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.     |
| IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,      |
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT    |
| NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   |
| DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       |
| THEORY LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING |
| NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,| 
| EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                          |
+-----------------------------------------------------------------------------+
*/

#include <stdio.h>
#include <math.h>
#include "curve256k1.h"
#include "basic_types.h"
#include "gf_p256k1_type.h"
#include "gf_p256k1_pack.h"
#include "gf_p256k1_arith.h"
#include "measure.h"

#define change_input(x,y,z)  {x[0] = y[0]^z[0];}
#define FILE stdout
void print_elem(const gfe_p256k1_4L *);
void print_vector(const vec *);
void set_values(uchar8 *, gfe_p256k1_10L *, gfe_p256k1_4L *, const int);
void set_vector(vec *, const gfe_p256k1_10L *, const gfe_p256k1_10L *, const gfe_p256k1_10L *, const gfe_p256k1_10L *);

int main() {
	const gfe_p256k1_4L E_VALUE = {0xFFFFFFFEFFFFFC2E,-1,-1,-1};
	// const gfe_p256k1_4L E_VALUE = {3,0,0,0};
	// const gfe_p256k1_4L E_VALUE = {3,3,3,3};

	const gfe_p256k1_4L F_VALUE = {0xFFFFFFFEFFFFFC2D,-1,-1,-1};

	const gfe_p256k1_4L ZERO_4L = {0,0,0,0};

	gfe_p256k1_4L e4L = E_VALUE;
	gfe_p256k1_4L res;

	uchar8 echar8[CRYPTO_BYTES];
	gfe_p256k1_10L e10L;

	// ------------------- Addition on GFp256-32-977 -------------------
	// -----------------------------------------------------------------

	// Test sequential addition

	set_values(echar8, &e10L, &e4L, 2);

	gfp256k1add(&res, &e4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te + e in 4-limb form:\n");
	fprintf(FILE,"e hex:\t\t");print_elem(&res);

	// Test vector addition

	vec n[NLIMBS_VEC_10] = {0};
	vec p[NLIMBS_VEC_10] = {0};
	vec q[NLIMBS_VEC_10] = {0};

	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &e10L, &e10L, &e10L, &e10L);

	vecp256k1add(q, n, p);

	fprintf(FILE,"\t<e,e,e,e> + <e,e,e,e> in 10-limb form:\n");
	print_vector(q);

	// ------------------- Subtraction on GFp256-32-977 -------------------
	// --------------------------------------------------------------------

	// Test sequential subtraction

	e4L = E_VALUE;

	gfe_p256k1_4L f4L =  F_VALUE;
	gfe_p256k1_4L zero4L = ZERO_4L;

	gfp256k1sub(&res, &e4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te - e in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1sub(&res, &e4L, &f4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te - f in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1sub(&res, &f4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\tf - e in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1sub(&res, &zero4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\t0 - e in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	// Test vector subtraction

	gfe_p256k1_10L f10L, zero10L;

	e4L = E_VALUE;
	f4L =  F_VALUE;
	zero4L = ZERO_4L;

	set_values(echar8, &e10L, &e4L, 2);
	set_values(echar8, &f10L, &f4L, 2);
	set_values(echar8, &zero10L, &zero4L, 2);
	
	set_vector(n, &e10L, &e10L, &f10L, &zero10L);
	set_vector(p, &e10L, &f10L, &e10L, &e10L);

	vecp256k1sub(q, n, p);

	fprintf(FILE,"\t<e,e,f,0> - <e,f,e,e> in 10-limb form:\n");
	print_vector(q);

	// ------------------- Measure CPU-cycles -------------------
	// ----------------------------------------------------------

	e4L = E_VALUE;
	f4L = F_VALUE;

	// Addition
	MEASURE_TIME({gfp256k1add(&res, &e4L, &e4L);gfp256k1add(&res, &e4L, &e4L);gfp256k1add(&res, &e4L, &e4L);gfp256k1add(&res, &e4L, &e4L);});
	fprintf(FILE,"CPU-cycles for 4 sequential field additions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	MEASURE_TIME({vecp256k1add(q,n,p);});
	fprintf(FILE,"CPU-cycles for a 4-way vector field addition: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Subtraction
	MEASURE_TIME({gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);});
	fprintf(FILE,"CPU-cycles for 4 sequential field subtractions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	set_vector(n, &e10L, &e10L, &f10L, &zero10L);
	set_vector(p, &e10L, &f10L, &e10L, &e10L);

	MEASURE_TIME({vecp256k1sub(q,n,p);});
	fprintf(FILE,"CPU-cycles for a 4-way vector field subtraction: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// uchar8 n[CRYPTO_BYTES] = {102, 66, 236, 240, 6, 149, 92, 7, 43, 107, 163, 255, 64, 145, 5, 203, 230, 54, 147, 234, 197, 5, 215, 214, 124, 189, 226, 219, 235, 71, 20, 254};
	// uchar8 p[CRYPTO_BYTES] = {9};
	// uchar8 q[CRYPTO_BYTES];

	// uchar8 i;

	// curve256k1_scalarmult(q,n,p);

	// fprintf(FILE,"\n");
	// for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",p[i]); fprintf(FILE,"\n\n");
	// for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",q[i]); fprintf(FILE,"\n\n");

	// curve256k1_scalarmult_base(q,n,p);

	// for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",p[i]); fprintf(FILE,"\n\n");
	// for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",q[i]); fprintf(FILE,"\n\n");

	// fprintf(FILE,"Computing CPU-cycles. It will take some time. Please wait!\n\n");

	// MEASURE_TIME({curve256k1_scalarmult_base(q,n,p);change_input(p,q,p);});
	// fprintf(FILE,"CPU-cycles for key generation of Curve256k1: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// MEASURE_TIME({curve256k1_scalarmult(q,n,p);change_input(p,q,p);});
	// fprintf(FILE,"CPU-cycles for shared secret computation of Curve256k1: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	return 0;
}

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

void set_vector(vec *V, const gfe_p256k1_10L *a0, const gfe_p256k1_10L *a1, const gfe_p256k1_10L *a2, const gfe_p256k1_10L *a3) {
	uchar8 i;
	for (i=0; i<NLIMBS_VEC_10;++i) {
		V[i][0] = a0->l[i];
		V[i][1] = a1->l[i];
		V[i][2] = a2->l[i];
		V[i][3] = a3->l[i];
	}
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

		fprintf(FILE,"%d hex:\t\t",j);print_elem(&res4L);
	}
}