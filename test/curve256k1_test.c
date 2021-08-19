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
#include "basic_types.h"
#include "gf_p256k1_type.h"
#include "gf_p256k1_pack.h"
#include "gf_p256k1_arith.h"
#include "curve256k1.h"
#include "measure.h"
#include "config.h"

#define change_input(x,y,z)  {x.l[0] = y.l[0]^z.l[0];}
#define FILE stdout
void print_elem(const gfe_p256k1_4L *);
void print_vector(const vec *);
void set_values(uchar8 *, gfe_p256k1_10L *, gfe_p256k1_4L *, const int);
void get_field_element(gfe_p256k1_4L *);
void get_vector(vec *);

int main() {

	#if !defined(DEFAULT_TEST_VALUES) && !defined(USER_TEST_VALUES)
		fprintf(FILE, "Define test values to be used in 'config.h'");
		return 1;
	#endif

	const gfe_p256k1_4L E_VALUE = {0xFFFFFFFEFFFFFC2E,-1,-1,-1};
	// const gfe_p256k1_4L E_VALUE = {3,0,0,0};
	// const gfe_p256k1_4L E_VALUE = {3,0xFFFFF,3,3};

	const gfe_p256k1_4L F_VALUE = {0xFFFFFFFEFFFFFC2D,-1,-1,-1};

	const gfe_p256k1_4L ZERO_4L = {0,0,0,0};

	gfe_p256k1_4L e4L = E_VALUE;
	gfe_p256k1_4L f4L = F_VALUE;
	gfe_p256k1_4L zero4L = ZERO_4L;
	gfe_p256k1_4L one4L = {1,0,0,0};
	gfe_p256k1_4L two4L = {2,0,0,0};
	gfe_p256k1_4L res;

	uchar8 echar8[CRYPTO_BYTES];
	gfe_p256k1_10L e10L;

	// ------------------- Addition on GFp256-32-977 -------------------
	// -----------------------------------------------------------------
	#if defined(SEQ_ADD_TEST)
	// Test sequential addition

	set_values(echar8, &e10L, &e4L, 2);

	gfp256k1add(&res, &e4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te + e in 4-limb form:\n");
	fprintf(FILE,"e hex:\t\t");print_elem(&res);
	#endif

	#if defined(VEC_ADD_TEST)
	// Test vector addition

	vec n[NLIMBS_VEC_10] = {0};
	vec p[NLIMBS_VEC_10] = {0};
	vec q[NLIMBS_VEC_10] = {0};

	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &e10L, &e10L, &e10L, &e10L);

	vecp256k1add(q, n, p);

	fprintf(FILE,"\t<e,e,e,e> + <e,e,e,e> in 10-limb form:\n");
	print_vector(q);
	#endif

	// ------------------- Subtraction on GFp256-32-977 -------------------
	// --------------------------------------------------------------------
	#if defined(SEQ_SUB_TEST)
	// Test sequential subtraction

	e4L = E_VALUE;
	f4L = F_VALUE;
	zero4L = ZERO_4L;

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
	#endif

	#if defined(VEC_SUB_TEST)
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
	#endif

	// ------------------- Multiplication on GFp256-32-977 -------------------
	// -----------------------------------------------------------------------
	#if defined(SEQ_MULC_TEST)
	// Test sequential multiplication
	e4L = E_VALUE;
	f4L = F_VALUE;
	zero4L = ZERO_4L;
	uint64 one64 = 1;
	uint64 two64 = 2;
	uint64 F64 = 0xFFFFFFFFFFFFFFFF;

	// Test multiplication by a small constant

	gfp256k1mulc(&res, &e4L, &one64);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * (uint64) 1 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mulc(&res, &e4L, &two64);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * (uint64) 2 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mulc(&res, &e4L, &F64);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * (uint64) 2^64 - 1 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mul(&res, &e4L, &(gfe_p256k1_4L){0xFFFFFFFFFFFFFFFF,0,0,0});

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * 2^64 - 1 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);
	#endif

	#if defined(SEQ_MUL_TEST)
	// Standard sequential multiplication

	gfp256k1mul(&res, &e4L, &one4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * 1 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mul(&res, &e4L, &two4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * 2 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mul(&res, &e4L, &e4L);

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * e in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);

	gfp256k1mul(&res, &e4L, &(gfe_p256k1_4L){0,0,0,0x7FFFFFFFFFFFFFFF});

	gfp256k1makeunique(&res);

	fprintf(FILE,"\te * 0 in 4-limb form:\n");
	fprintf(FILE,"res hex:\t\t");print_elem(&res);
	#endif

	#if defined(VEC_MUL_TEST)
	// Test vector multiplication

	gfe_p256k1_10L one10L, two10L;

	e4L = E_VALUE;
	f4L =  F_VALUE;
	zero4L = ZERO_4L;

	set_values(echar8, &e10L, &e4L, 2);
	set_values(echar8, &one10L, &one4L, 2);
	set_values(echar8, &two10L, &two4L, 2);
	set_values(echar8, &zero10L, &(gfe_p256k1_4L){0,0,0,0x7FFFFFFFFFFFFFFF}, 2);
	
	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &one10L, &two10L, &e10L, &zero10L);

	vecp256k1mul(q, n, p);

	// fprintf(FILE,"\t<e,e,e,e> * <1,2,e,0> in 10-limb form:\n");
	// print_vector(q);
	#endif

	// ------------------- Measure CPU-cycles -------------------
	// ----------------------------------------------------------

	#if defined(CPU_TESTS)
	e4L = E_VALUE;
	f4L = F_VALUE;
	
	set_values(echar8, &e10L, &e4L, 2);
	set_values(echar8, &f10L, &f4L, 2);
	set_values(echar8, &one10L, &one4L, 2);
	set_values(echar8, &two10L, &two4L, 2);
	
	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &e10L, &f10L, &one10L, &two10L);

	// Addition
	MEASURE_TIME({gfp256k1add(&res, &e4L, &e4L);gfp256k1add(&res, &e4L, &f4L);gfp256k1add(&res, &e4L, &one4L);gfp256k1add(&res, &e4L, &two4L);});
	fprintf(FILE,"CPU-cycles for 4 sequential field additions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

    set_values(echar8, &e10L, &e4L, 2);
	set_values(echar8, &f10L, &f4L, 2);
	set_values(echar8, &one10L, &one4L, 2);
	set_values(echar8, &two10L, &two4L, 2);
	
	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &e10L, &f10L, &one10L, &two10L);

	MEASURE_TIME({vecp256k1add(q,n,p);});
	fprintf(FILE,"CPU-cycles for a 4-way vector field addition: %5.0lf\n\n", ceil(((get_median())/(double)(N))));
	
	//MEASURE_TIME({change_input(e4L,res,e4L);set_values(echar8, &e10L, &e4L, 2);set_vector(n, &e10L, &e10L, &e10L, &e10L);});
	//fprintf(FILE,"CPU-cycles for change input: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Subtraction
	MEASURE_TIME({gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);gfp256k1sub(&res, &e4L, &f4L);});
	fprintf(FILE,"CPU-cycles for 4 sequential field subtractions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	set_vector(n, &e10L, &e10L, &f10L, &zero10L);
	set_vector(p, &e10L, &f10L, &e10L, &e10L);

	MEASURE_TIME({vecp256k1sub(q,n,p);});
	fprintf(FILE,"CPU-cycles for a 4-way vector field subtraction: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Multiplication
	MEASURE_TIME({gfp256k1mul(&res, &e4L, &e4L);gfp256k1mul(&res, &e4L, &f4L);gfp256k1mul(&res, &e4L, &f4L);gfp256k1mul(&res, &e4L, &(gfe_p256k1_4L){0,0,0,0x7FFFFFFFFFFFFFFF});});
	fprintf(FILE,"CPU-cycles for 4 sequential field multiplications: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

    set_values(echar8, &e10L, &e4L, 2);
	set_values(echar8, &f10L, &f4L, 2);

	set_vector(n, &e10L, &e10L, &e10L, &e10L);
	set_vector(p, &e10L, &f10L, &f10L, &zero10L);

	MEASURE_TIME({vecp256k1mul(q,n,p);});
	fprintf(FILE,"CPU-cycles for a 4-way vector field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Single-way multiplication
		
	uint64 test64 = 0x7FFFFFFFFFFFFFFF;
	
	MEASURE_TIME({gfp256k1mul(&res, &e4L, &(gfe_p256k1_4L){0x7FFFFFFFFFFFFFFF,0,0,0});});
	fprintf(FILE,"CPU-cycles for a full field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));
	
	MEASURE_TIME({gfp256k1mulc(&res, &e4L, &test64);});
	fprintf(FILE,"CPU-cycles for a small constant field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));
	#endif

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

void get_field_element(gfe_p256k1_4L *elem) {
	uint64 limb0, limb1, limb2, limb3;
	printf("Enter 64-bit limbs in little-endian order. Use hex format.\n");

	printf("Limb 0: ");
	scanf("%llX", %limb0);
	printf("Limb 1: ");
	scanf("%llX", %limb1);
	printf("Limb 2: ");
	scanf("%llX", %limb2);
	printf("Limb 3: ");
	scanf("%llX", %limb3);

	elem = (gfe_p256k1_4L){limb0, limb1, limb2, limb3};
}

void get_vector(vec *V) {
	gfe_p256k1_4L elem0, elem1, elem2, elem3;

	printf("Element 0: \n");
	get_field_element(&elem0);
	printf("Element 1: \n");
	get_field_element(&elem1);
	printf("Element 2: \n");
	get_field_element(&elem2);
	printf("Element 3: \n");
	get_field_element(&elem3);

	set_vector(V, &elem0, &elem1, &elem2, &elem3);
}