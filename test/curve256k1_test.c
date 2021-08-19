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
void print_elem(const gfe_p256k1_4L *);
void print_vector(const vec *);
void set_values(uchar8 *, gfe_p256k1_10L *, gfe_p256k1_4L *, const int);
void get_field_element(gfe_p256k1_4L *);
void get_vector(vec *);
void set_defaults();
void pack_10l_operands();

static FILE *PRINTFILE;
static gfe_p256k1_4L a1_4L, a2_4L, a3_4L, a4_4L, b1_4L, b2_4L, b3_4L, b4_4L, res1, res2, res3, res4;
static gfe_p256k1_10L a1_10L, a2_10L, a3_10L, a4_10L, b1_10L, b2_10L, b3_10L, b4_10L, res_10L;
static const gfe_p256k1_4L E_VALUE = {0xFFFFFFFEFFFFFC2D,-1,-1,-1};
static const gfe_p256k1_4L F_VALUE = {0xFFFFFFFEFFFFFC2E,-1,-1,-1};
static const gfe_p256k1_4L ZERO_4L = {0,0,0,0};
static const gfe_p256k1_4L ONE_4L = {1,0,0,0};
static const gfe_p256k1_4L TWO_4L = {2,0,0,0};
static vec n[NLIMBS_VEC_10] = {0};
static vec p[NLIMBS_VEC_10] = {0};
static vec q[NLIMBS_VEC_10] = {0};

int main() {

	#if !defined(DEFAULT_TEST_VALUES) && !defined(USER_TEST_VALUES)
		printf("Define test values to be used in 'config.h'\n");
		return 1;
	#endif

	#if defined(DEFAULT_TEST_VALUES)
		fprintf(PRINTFILE, "Default values:\n");
		fprintf(PRINTFILE, "e = ");print_elem(&E_VALUE);
		fprintf(PRINTFILE, "f = ");print_elem(&F_VALUE);
	#endif

	PRINTFILE = fopen("output.txt", "w+");

	// ------------------- Addition on GFp256-32-977 -------------------
	// -----------------------------------------------------------------
	#if defined(SEQ_ADD_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test sequential addition
	fprintf(PRINTFILE,"------------------- Sequential Addition -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	gfp256k1add(&res1, &a1_4L, &b1_4L);
	gfp256k1add(&res2, &a2_4L, &b2_4L);
	gfp256k1add(&res3, &a3_4L, &b3_4L);
	gfp256k1add(&res4, &a4_4L, &b4_4L);

	gfp256k1makeunique(&res1);
	gfp256k1makeunique(&res2);
	gfp256k1makeunique(&res3);
	gfp256k1makeunique(&res4);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"e + 0:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"e + 1:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"e + 2:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"e + f:\t\t");print_elem(&res4);
	#else
	fprintf(PRINTFILE,"a1 + b1:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"a2 + b2:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"a3 + b3:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"a4 + b4:\t\t");print_elem(&res4);
	#endif
	#endif

	#if defined(VEC_ADD_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test vector addition
	fprintf(PRINTFILE,"------------------- Vector Addition -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	pack_10l_operands();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	set_vector(n, &a1_10L, &a2_10L, &a3_10L, &a4_10L);
	set_vector(p, &b1_10L, &b2_10L, &b3_10L, &b4_10L);

	vecp256k1add(q, n, p);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"<e,e,e,e> + <0,1,2,f>:\n");
	#else
	fprintf(PRINTFILE,"<a1,a2,a3,a4> + <b1,b2,b3,b4>:\n");
	#endif

	print_vector(q);
	#endif

	// ------------------- Subtraction on GFp256-32-977 -------------------
	// --------------------------------------------------------------------
	#if defined(SEQ_SUB_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test sequential subtraction
	fprintf(PRINTFILE,"------------------- Sequential Subtraction -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	gfp256k1sub(&res1, &a1_4L, &b1_4L);
	gfp256k1sub(&res2, &a2_4L, &b2_4L);
	gfp256k1sub(&res3, &b3_4L, &a3_4L);
	gfp256k1sub(&res4, &a4_4L, &b4_4L);

	gfp256k1makeunique(&res1);
	gfp256k1makeunique(&res2);
	gfp256k1makeunique(&res3);
	gfp256k1makeunique(&res4);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"e - 0:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"e - 1:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"e - 2:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"e - f:\t\t");print_elem(&res4);
	#else
	fprintf(PRINTFILE,"a1 - b1:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"a2 - b2:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"a3 - b3:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"a4 - b4:\t\t");print_elem(&res4);
	#endif
	#endif

	#if defined(VEC_SUB_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test vector subtraction
	fprintf(PRINTFILE,"------------------- Vector Subtraction -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	pack_10l_operands();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	set_vector(n, &a1_10L, &a2_10L, &a3_10L, &a4_10L);
	set_vector(p, &b1_10L, &b2_10L, &b3_10L, &b4_10L);

	vecp256k1sub(q, n, p);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"<e,e,e,e> - <0,1,2,f>:\n");
	#else
	fprintf(PRINTFILE,"<a1,a2,a3,a4> - <b1,b2,b3,b4>:\n");
	#endif

	print_vector(q);
	#endif

	// ------------------- Negation on GFp256-32-977 -------------------
	// --------------------------------------------------------------------
	#if defined(SEQ_NEG_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test sequential negation
	fprintf(PRINTFILE,"------------------- Sequential Negation -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	gfp256k1neg(&res1, &b1_4L);
	gfp256k1neg(&res2, &b2_4L);
	gfp256k1neg(&res3, &b3_4L);
	gfp256k1neg(&res4, &b4_4L);

	gfp256k1makeunique(&res1);
	gfp256k1makeunique(&res2);
	gfp256k1makeunique(&res3);
	gfp256k1makeunique(&res4);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"-0:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"-1:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"-2:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"-f:\t\t");print_elem(&res4);
	#else
	fprintf(PRINTFILE,"-b1:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"-b2:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"-b3:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"-b4:\t\t");print_elem(&res4);
	#endif
	#endif

	// ------------------- Multiplication on GFp256-32-977 -------------------
	// -----------------------------------------------------------------------
	#if defined(SEQ_MUL_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test sequential Multiplication
	fprintf(PRINTFILE,"------------------- Sequential Multiplication -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	gfp256k1mul(&res1, &a1_4L, &b1_4L);
	gfp256k1mul(&res2, &a2_4L, &b2_4L);
	gfp256k1mul(&res3, &a3_4L, &b3_4L);
	gfp256k1mul(&res4, &a4_4L, &b4_4L);

	gfp256k1makeunique(&res1);
	gfp256k1makeunique(&res2);
	gfp256k1makeunique(&res3);
	gfp256k1makeunique(&res4);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"e * 0:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"e * 1:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"e * 2:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"e * f:\t\t");print_elem(&res4);
	#else
	fprintf(PRINTFILE,"a1 * b1:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"a2 * b2:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"a3 * b3:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"a4 * b4:\t\t");print_elem(&res4);
	#endif
	#endif

	#if defined(VEC_MUL_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test vector Multiplication
	fprintf(PRINTFILE,"------------------- Vector Multiplication -------------------\n\n");

	#if defined(DEFAULT_TEST_VALUES)
	set_defaults();
	pack_10l_operands();
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	set_vector(n, &a1_10L, &a2_10L, &a3_10L, &a4_10L);
	set_vector(p, &b1_10L, &b2_10L, &b3_10L, &b4_10L);

	vecp256k1mul(q, n, p);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"<e,e,e,e> * <0,1,2,f>:\n");
	#else
	fprintf(PRINTFILE,"<a1,a2,a3,a4> * <b1,b2,b3,b4>:\n");
	#endif

	print_vector(q);
	#endif


	#if defined(SEQ_MULC_TEST) || defined(ALL_FUNCTION_TESTS)
	// Test sequential Multiplication
	fprintf(PRINTFILE,"------------------- Sequential Multiplication by Small Constant -------------------\n\n");
	uint64 b1_64, b2_64, b3_64, b4_64;

	#if defined(DEFAULT_TEST_VALUES)
	b1_64 = (uint64)0;
	b2_64 = (uint64)1;
	b3_64 = (uint64)2;
	b4_64 = 0xFFFFFFFFFFFFFFFF;
	#endif
	#if defined(USER_TEST_VALUES)
	#endif

	gfp256k1mulc(&res1, &a1_4L, &b1_64);
	gfp256k1mulc(&res2, &a2_4L, &b2_64);
	gfp256k1mulc(&res3, &a3_4L, &b3_64);
	gfp256k1mulc(&res4, &a4_4L, &b4_64);

	gfp256k1makeunique(&res1);
	gfp256k1makeunique(&res2);
	gfp256k1makeunique(&res3);
	gfp256k1makeunique(&res4);

	#if defined(DEFAULT_TEST_VALUES)
	fprintf(PRINTFILE,"e * 0:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"e * 1:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"e * 2:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"e * 2^64-1:\t\t");print_elem(&res4);
	#else
	fprintf(PRINTFILE,"a1 * b1:\t\t");print_elem(&res1);
	fprintf(PRINTFILE,"a2 * b2:\t\t");print_elem(&res2);
	fprintf(PRINTFILE,"a3 * b3:\t\t");print_elem(&res3);
	fprintf(PRINTFILE,"a4 * b4:\t\t");print_elem(&res4);
	#endif
	#endif

	// ------------------- Measure CPU-cycles -------------------
	// ----------------------------------------------------------

	#if defined(SPEED_TESTS)
	fprintf(PRINTFILE,"------------------- Measuring CPU Cycles -------------------\n\n");

	set_defaults();
	pack_10l_operands();
	set_vector(n, &a1_10L, &a2_10L, &a3_10L, &a4_10L);
	set_vector(p, &b1_10L, &b2_10L, &b3_10L, &b4_10L);

	// Addition
	MEASURE_TIME({gfp256k1add(&res1, &a1_4L, &b1_4L);gfp256k1add(&res2, &a2_4L, &b2_4L);gfp256k1add(&res3, &a3_4L, &b3_4L);gfp256k1add(&res4, &a4_4L, &b4_4L);});
	fprintf(PRINTFILE,"CPU-cycles for 4 sequential field additions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	MEASURE_TIME({vecp256k1add(q,n,p);});
	fprintf(PRINTFILE,"CPU-cycles for a 4-way vector field addition: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Subtraction
	MEASURE_TIME({gfp256k1sub(&res1, &a1_4L, &b1_4L);gfp256k1sub(&res2, &a2_4L, &b2_4L);gfp256k1sub(&res3, &a3_4L, &b3_4L);gfp256k1sub(&res4, &a4_4L, &b4_4L);});
	fprintf(PRINTFILE,"CPU-cycles for 4 sequential field subtractions: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	MEASURE_TIME({vecp256k1sub(q,n,p);});
	fprintf(PRINTFILE,"CPU-cycles for a 4-way vector field subtraction: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Multiplication
	MEASURE_TIME({gfp256k1mul(&res1, &a1_4L, &b1_4L);gfp256k1mul(&res2, &a2_4L, &b2_4L);gfp256k1mul(&res3, &a3_4L, &b3_4L);gfp256k1mul(&res4, &a4_4L, &b4_4L);});
	fprintf(PRINTFILE,"CPU-cycles for 4 sequential field multiplications: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	MEASURE_TIME({vecp256k1mul(q,n,p);});
	fprintf(PRINTFILE,"CPU-cycles for a 4-way vector field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));

	// Single-way multiplication
	uint64 test64 = 0x7FFFFFFFFFFFFFFF;
	
	MEASURE_TIME({gfp256k1mul(&res1, &a1_4L, &(gfe_p256k1_4L){0x7FFFFFFFFFFFFFFF,0,0,0});});
	fprintf(PRINTFILE,"CPU-cycles for a full field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));
	
	MEASURE_TIME({gfp256k1mulc(&res1, &a1_4L, &test64);});
	fprintf(PRINTFILE,"CPU-cycles for a small-constant field multiplication: %5.0lf\n\n", ceil(((get_median())/(double)(N))));
	#endif

	fclose(PRINTFILE);

	return 0;
}

void print_elem(const gfe_p256k1_4L *e){

	uchar8  i;

	for (i=NLIMBS-1; i>0; --i) 
		fprintf(PRINTFILE,"%16llX ",e->l[i]);
	fprintf(PRINTFILE,"%16llX \n\n",e->l[0]);
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

		fprintf(PRINTFILE,"Elem %d:\t\t",j);print_elem(&res4L);
	}
}

void get_field_element(gfe_p256k1_4L *elem) {
	uint64 limb0, limb1, limb2, limb3;
	printf("Enter 64-bit limbs in little-endian order. Use hex format.\n");

	printf("Limb 0: ");
	scanf("%llX", &limb0);
	printf("Limb 1: ");
	scanf("%llX", &limb1);
	printf("Limb 2: ");
	scanf("%llX", &limb2);
	printf("Limb 3: ");
	scanf("%llX", &limb3);

	*elem = (gfe_p256k1_4L){limb0, limb1, limb2, limb3};
}

void get_vector(vec *V) {
	gfe_p256k1_4L elem0, elem1, elem2, elem3;
	gfe_p256k1_10L elem0_10, elem1_10, elem2_10, elem3_10;

	printf("Element 0: \n");
	get_field_element(&elem0);
	printf("Element 1: \n");
	get_field_element(&elem1);
	printf("Element 2: \n");
	get_field_element(&elem2);
	printf("Element 3: \n");
	get_field_element(&elem3);

	gfp256k1pack410(&elem0_10, &elem0);
	gfp256k1pack410(&elem1_10, &elem1);
	gfp256k1pack410(&elem2_10, &elem2);
	gfp256k1pack410(&elem3_10, &elem3);

	set_vector(V, &elem0_10, &elem1_10, &elem2_10, &elem3_10);
}

void set_defaults() {
	a1_4L = a2_4L = a3_4L = a4_4L = E_VALUE;
	b1_4L = ZERO_4L;
	b2_4L = ONE_4L;
	b3_4L = TWO_4L;
	b4_4L = F_VALUE;
}

void pack_10l_operands() {
	gfp256k1pack410(&a1_10L, &a1_4L);
	gfp256k1pack410(&a2_10L, &a2_4L);
	gfp256k1pack410(&a3_10L, &a3_4L);
	gfp256k1pack410(&a4_10L, &a4_4L);

	gfp256k1pack410(&b1_10L, &b1_4L);
	gfp256k1pack410(&b2_10L, &b2_4L);
	gfp256k1pack410(&b3_10L, &b3_4L);
	gfp256k1pack410(&b4_10L, &b4_4L);
}