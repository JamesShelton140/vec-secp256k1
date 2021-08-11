/*
+-----------------------------------------------------------------------------+
| This code corresponds to the the paper "Efficient 4-way Vectorizations of   |
| the Montgomery Ladder" authored by   			       	       	      |
| Kaushik Nath,  Indian Statistical Institute, Kolkata, India, and            |
| Palash Sarkar, Indian Statistical Institute, Kolkata, India.	              |
+-----------------------------------------------------------------------------+
| Copyright (c) 2020, Kaushik Nath and Palash Sarkar.                         |
|                                                                             |
| Permission to use this code is granted.                          	      |
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

#include "basic_types.h"
#include "gf_p256k1_type.h"
#include "gf_p256k1_pack.h"
#include "gf_p256k1_arith.h"
#include "curve256k1.h"

void ladder_setup(gfe_p256k1_4L *, gfe_p256k1_4L *, gfe_p256k1_4L *, gfe_p256k1_4L *, gfe_p256k1_4L *, const gfe_p256k1_4L *);
void QR_ladder_step(gfe_p256k1_10L *, gfe_p256k1_10L *, gfe_p256k1_10L *, gfe_p256k1_10L *, gfe_p256k1_10L *);
//Compute finalization of the scalar multiplication algorithm
void final_step(gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *);

void calculate_M(gfe_p256k1_4L *M, const gfe_p256k1_10L *xRP_10L, const gfe_p256k1_10L *yQ_10L, const gfe_p256k1_10L *yR_10L, const gfe_p256k1_10L *G_10L);

int curve256k1_scalarmult(uchar8 *q, const uchar8 *n, const uchar8 *p) {
	gfe_p256k1_10L xQP_10L, xRP_10L, yQ_10L, yR_10L, G_10L, xP_10L, n_10L, temp;
	gfe_p256k1_4L xQP, xRP, yQ, yR, G, xP, xQ, M, n_4L;

	// Prepare n for non-zero initial state ladder
	gfp256k1pack10(&n_10L, n);
	gfp256k1pack104(&n_4L, &n_10L);
	curve256k1subc(&n_4L, &n_4L);
	//gfp256k1makeunique(&n_4L);

	gfp256k1pack10(&xP_10L, p);
	gfp256k1pack104(&xP, &xP_10L);

	// Ladder setup procedure
	ladder_setup(&xQP, &xRP, &yQ, &yR, &G, &xP);

	uint64 prevswap = 0;
	uint64 swap;
	int i, bit, limb;
	for(i=255; i >= 0; i--) {
		// swap bit calc
		// select bit at position i
        limb = i/64;
        bit = i%64;
		// swap = not(bit i)
        swap = (uint64)1 ^ ((uint64)1 & (n_4L.l[limb] >> bit));
		// prevswap = prevswap xor swap
		prevswap ^= swap;

		//perform conditional swap
		if(prevswap) {
			temp = xRP_10L;
			xRP_10L = xQP_10L;
			xQP_10L = temp;

			temp = yR_10L;
			yR_10L =yQ_10L;
			yQ_10L = temp;
		} else {
			// attempt to make constant time
			// will be implemented in constant time using vpermd instruction in asm
			temp = xQP_10L;
			xRP_10L = xRP_10L;
			xQP_10L = temp;

			temp = yQ_10L;
			yR_10L =yR_10L;
			yQ_10L = temp;
		}
		temp = ZERO_10L;

		QR_ladder_step(&xQP_10L, &xRP_10L, &yQ_10L, &yR_10L, &G_10L);

		if(i==1) {
			calculate_M(&M, &xRP_10L, &yQ_10L, &yR_10L, &G_10L);
		}

		// preevswap = swap
		prevswap = swap;
	}

	gfp256k1pack104(&xQP, &xQP_10L);
	gfp256k1pack104(&xRP, &xRP_10L);

	final_step(&xQ, &xQP, &xRP, &M, &xP);
	
	gfp256k1unpack(q, &xQ);

	return 0;
}

void ladder_setup(gfe_p256k1_4L *xQP, gfe_p256k1_4L *xRP, gfe_p256k1_4L *yQ, gfe_p256k1_4L *yR, gfe_p256k1_4L *G, const gfe_p256k1_4L *xP) {
	// Hamburg supplementary Sage code (includes power analysis attack mitigation)
	//
	// r = random_nonzero(xP.parent())
    // xP  *= r
    // r   *= 1
    
    // rr   = r^2
    // yP   = 4*((xP^2 + ga*rr)*xP + gb*r*rr)
    // xP3  = 3*xP
    // m    = xP*xP3 + ga*rr
    // xP3 *= yP
    // yP   = yP^2
    // xRP  = m^2 - xP3
    // yR   = 2*m*xRP + yP
	uint64 u64three = 3;
	uint64 u64four = 4;

	// yP   = 4*((xP^2)*xP + 7)
	gfe_p256k1_4L yP, xPxP, xPxPxP, yPt, yPt4;
	gfp256k1sqr(&xPxP, xP);
	gfp256k1mul(&xPxPxP, &xPxP, xP);
	gfp256k1add(&yPt, &xPxPxP, &(gfe_p256k1_4L){7,0,0,0});
	gfp256k1mulc(&yP, &yPt, &u64four);
	//gfp256k1mul(&yP, &yPt, &(gfe_p256k1_4L){4,0,0,0});

	// xP3  = 3*xP
	gfe_p256k1_4L xP3;
	gfp256k1mulc(&xP3, xP, &u64three);
	//gfp256k1mul(&xP3, xP, &(gfe_p256k1_4L){3,0,0,0});

	// m    = xP*xP3
	gfe_p256k1_4L m;
	gfp256k1mul(&m, &xP3, xP);

	// xP3 *= yP
	gfe_p256k1_4L xP3yP;
	gfp256k1mul(&xP3yP, &xP3, &yP);

	// yQ   = yP^2
	gfp256k1sqr(yQ, &yP);

	// xRP  = m^2 - xP3yP
	gfe_p256k1_4L mm;
	gfp256k1sqr(&mm, &m);
	gfp256k1sub(xRP, &mm, &xP3yP);

	// yR   = 2*m*xRP + yQ
	gfe_p256k1_4L mxRP, mxRP2;
	gfp256k1mul(&mxRP, &m, xRP);
	gfp256k1add(&mxRP2, &mxRP, &mxRP);
	gfp256k1add(yR, &mxRP2, yQ);

	// xQP  = 0
	xQP = &(gfe_p256k1_4L){0,0,0,0};

	// G    = xRP^2
	gfp256k1sqr(G, xRP);

	return;
}

void QR_ladder_step(gfe_p256k1_10L *xQP, gfe_p256k1_10L *xRP, gfe_p256k1_10L *yQ, gfe_p256k1_10L *yR, gfe_p256k1_10L *G) {
	gfe_p256k1_10L M, V, bin;
	vec n[NLIMBS_VEC_10] = {0};
	vec p[NLIMBS_VEC_10] = {0};
	vec q[NLIMBS_VEC_10] = {0};

	// <xQP, xRP, yQ, yR> 	= <xQP, xRP, yQ, yR> 		* <G, G, yR, yR>
	set_vector(n, xQP, xRP, yQ, yR);
	set_vector(p, G, G, yR, yR);
	vecp256k1mul(q, n, p);
	unpack_vector(xQP, xRP, yQ, yR, q);

	// <xQP, G, -, M> 		= <xQP, xRP, [xQP], xRP> 	- <yQ, yQ, [yR], yR>
	set_vector(n, xQP, xRP, xQP, xRP);
	set_vector(p, yQ, yQ, yR, yR);
	vecp256k1sub(q, n, p);
	unpack_vector(xQP, G, &bin, &M, q);

	// <xQP, xRP, yQ, V> 	= <yQ, xRP, yQ, xQP> 		* <yR, G, G, yR>
	set_vector(n, yQ, xRP, yQ, xQP);
	set_vector(p, yR, G, G, yR);
	vecp256k1mul(q, n, p);
	unpack_vector(xQP, xRP, yQ, &V, q);

	// <-, xRP, yQ, M> 		= <xQP, xRP, yQ, M> 		+ <[0], V, V, G>
	set_vector(n, xQP, xRP, yQ, &M);
	set_vector(p, &ZERO_10L, &V, &V, G);
	vecp256k1add(q, n, p);
	unpack_vector(&bin, xRP, yQ, &M, q);

	// <-, G, yQ, M> 		= <[xQP], xRP, yQ, M> 		* <[1], xRP, yR, xRP>
	set_vector(n, xQP, xRP, yQ, &M);
	set_vector(p, &ONE_10L, xRP, yR, xRP);
	vecp256k1mul(q, n, p);
	unpack_vector(&bin, G, yQ, &M, q);

	// <-, xRP, -, yR> 		= <[xQP], xRP, [yQ], M> 	+ <[0], xQP, [0], yQ>
	set_vector(n, xQP, xRP, yQ, &M);
	set_vector(p, &ZERO_10L, xQP, &ZERO_10L, yQ);
	vecp256k1add(q, n, p);
	unpack_vector(&bin, xRP, &bin, yR, q);
}

void calculate_M(gfe_p256k1_4L *M, const gfe_p256k1_10L *xRP_10L, const gfe_p256k1_10L *yQ_10L, const gfe_p256k1_10L *yR_10L, const gfe_p256k1_10L *G_10L) {
	gfe_p256k1_4L xRP, yQ, yR, G, H, L, J, Mt, xRPrime;

	gfp256k1pack104(&xRP, xRP_10L);
	gfp256k1pack104(&yQ, yQ_10L);
	gfp256k1pack104(&yR, yR_10L);
	gfp256k1pack104(&G, G_10L);

	// H 		= yR^2
	gfp256k1sqr(&H, &yR);

	// L 		= yQ*yR
	gfp256k1mul(&L, &yQ, &yR);

	// xRPrime  = xRP*G
	gfp256k1mul(&xRPrime, &xRP, &G);

	// J 		= xRPrime - L
	gfp256k1sub(&J, &xRPrime, &L);

	// M 		= J + xRPrime - H
	gfp256k1add(&Mt, &J, &xRPrime);
	gfp256k1sub(M, &Mt, &H);

}

void final_step(gfe_p256k1_4L *xQ, const gfe_p256k1_4L *xQP, const gfe_p256k1_4L *xRP, const gfe_p256k1_4L *M, const gfe_p256k1_4L *xP) {
	// xP*Z^2 = (M^2 - xQP - xRP)/3
	gfe_p256k1_4L MM, MMt1, MMt2, xPZZ;
	gfp256k1sqr(&MM, M);
	gfp256k1sub(&MMt1, &MM, xQP);
	gfp256k1sub(&MMt2, &MMt1, xRP);
	gfp256k1mul(&xPZZ, &MMt2, &threeinv);

	// 1/Z^2  = xP / (xP*Z^2)
	gfe_p256k1_4L ZZinv, xPZZinv;
	gfp256k1inv(&xPZZinv, &xPZZ);
	gfp256k1mul(&ZZinv, xP, &xPZZinv);

	// xQ     = (xP + xQP) / Z^2
	gfe_p256k1_4L xQt;
	gfp256k1add(&xQt, &xPZZ, xQP);//need to work out if 'xP' is correct here (might be xP*Z^2)
	gfp256k1mul(xQ, &xQt, &ZZinv);

}