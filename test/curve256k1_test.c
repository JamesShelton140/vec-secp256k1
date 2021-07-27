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

int main() {

	// gfe_p256k1_4L e4L = {0xFFFFFFFEFFFFFC2E,-1,-1,-1};
	gfe_p256k1_4L e4L = {3,0,0,0};
	gfe_p256k1_4L res;

	uchar8 echar8[CRYPTO_BYTES];
	gfp256k1unpack(echar8, &e4L);

	gfe_p256k1_10L e10L;
	gfp256k1pack10(&e10L, echar8);

	vec n[NLIMBS_VEC_10] = {0};
	vec p[NLIMBS_VEC_10] = {0};
	vec q[NLIMBS_VEC_10] = {0};

	uchar8 i;
	for (i=0; i<NLIMBS_VEC_10;++i) {
		n[i][0] = n[i][1] = n[i][2] = n[i][3] = e10L.l[i];
		p[i][0] = p[i][1] = p[i][2] = p[i][3] = e10L.l[i];
	}

	gfp256k1add(&res, &e4L, &e4L);

	gfp256k1unpack(echar8, &res);

	fprintf(FILE,"e + e in 4-limb form:\n");
	for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",echar8[i]); fprintf(FILE,"\n\n");

	uchar8 resChar0[CRYPTO_BYTES];
	uchar8 resChar1[CRYPTO_BYTES];
	uchar8 resChar2[CRYPTO_BYTES];
	uchar8 resChar3[CRYPTO_BYTES];

	gfe_p256k1_10L res0, res1, res2, res3;

	gfe_p256k1_4L res0S, res1S, res2S, res3S;

	vecp256k1add(q,n,p);

	for (i=0; i<NLIMBS_VEC_10;++i) {
		res0.l[i] = q[i][0];
		res1.l[i] = q[i][1];
		res2.l[i] = q[i][2];
		res3.l[i] = q[i][3];
	}

	gfp256k1pack104(&res0S,&res0);
	gfp256k1pack104(&res1S,&res1);
	gfp256k1pack104(&res2S,&res2);
	gfp256k1pack104(&res3S,&res3);

	gfp256k1unpack(resChar0,&res0S);
	gfp256k1unpack(resChar1,&res1S);
	gfp256k1unpack(resChar2,&res2S);
	gfp256k1unpack(resChar3,&res3S);

	fprintf(FILE,"<e,e,e,e> + <e,e,e,e> in 10-limb form:\n");
	fprintf(FILE,"0:\t\t");for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",resChar0[i]); fprintf(FILE,"\n\n");
	fprintf(FILE,"1:\t\t");for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",resChar1[i]); fprintf(FILE,"\n\n");
	fprintf(FILE,"2:\t\t");for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",resChar2[i]); fprintf(FILE,"\n\n");
	fprintf(FILE,"3:\t\t");for(i=0;i<CRYPTO_BYTES;++i) fprintf(FILE,"%4d",resChar3[i]); fprintf(FILE,"\n\n");

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
