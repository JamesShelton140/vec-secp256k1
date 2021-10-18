/*
+-----------------------------------------------------------------------------+
| Copyright (c) 2020, Kaushik Nath and Palash Sarkar.                         |
| 				2021, Timothy James Shelton.								  |
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

#include "gf_p256k1_pack.h"

void gfp256k1pack(gfe_p256k1_9L *v,const uchar8 *u) {

	uchar8 i,j,k,l;
	gfe_p256k1_4L t;

	for (i=0;i<NLIMBS;++i) {
	
		j = i*8;
		t.l[i] = (uint64)u[j]; l = 1;
		for (k=1;k<9;++k)
			t.l[i] |= ((uint64)u[j+l++]  << k*8);
	}

	v->l[0] = ((t.l[0] & 0x000000001FFFFFFF));
	v->l[1] = ((t.l[0] & 0x03FFFFFFE0000000) >> 29);
	v->l[2] = ((t.l[0] & 0xFC00000000000000) >> 58) | ((t.l[1] & 0x00000000007FFFFF) <<  6);
	v->l[3] = ((t.l[1] & 0x000FFFFFFF800000) >> 23);
	v->l[4] = ((t.l[1] & 0xFFF0000000000000) >> 52) | ((t.l[2] & 0x000000000001FFFF) << 12);
	v->l[5] = ((t.l[2] & 0x00003FFFFFFE0000) >> 17);
	v->l[6] = ((t.l[2] & 0xFFFFC00000000000) >> 46) | ((t.l[3] & 0x00000000000007FF) << 18);
	v->l[7] = ((t.l[3] & 0x000000FFFFFFF800) >> 11);
	v->l[8] = ((t.l[3] & 0xFFFFFF0000000000) >> 40);
}

void gfp256k1pack94(gfe_p256k1_4L *v,const gfe_p256k1_9L *u) {

        v->l[0] = ((u->l[0] & 0x000000001FFFFFFF))       | ((u->l[1] & 0x000000001FFFFFFF) << 29) | ((u->l[2] & 0x000000000000003F) << 58);
        v->l[1] = ((u->l[2] & 0x000000001FFFFFC0) >>  6) | ((u->l[3] & 0x000000001FFFFFFF) << 23) | ((u->l[4] & 0x0000000000000FFF) << 52);
        v->l[2] = ((u->l[4] & 0x000000001FFFF000) >> 12) | ((u->l[5] & 0x000000001FFFFFFF) << 17) | ((u->l[6] & 0x000000000003FFFF) << 46);
        v->l[3] = ((u->l[6] & 0x000000001FFC0000) >> 18) | ((u->l[7] & 0x000000001FFFFFFF) << 11) | ((u->l[8] & 0x0000000000FFFFFF) << 40);   
}

void gfp256k1unpack(uchar8 *v,const gfe_p256k1_4L *u) {

	uchar8 i,j;

	for (i=0;i<NLIMBS;++i) {

		j = i*8;
		v[j+0] = (uchar8)((u->l[i] & 0x00000000000000FF));
		v[j+1] = (uchar8)((u->l[i] & 0x000000000000FF00) >>  8);
		v[j+2] = (uchar8)((u->l[i] & 0x0000000000FF0000) >> 16);
		v[j+3] = (uchar8)((u->l[i] & 0x00000000FF000000) >> 24);
		v[j+4] = (uchar8)((u->l[i] & 0x000000FF00000000) >> 32);
		v[j+5] = (uchar8)((u->l[i] & 0x0000FF0000000000) >> 40);
		v[j+6] = (uchar8)((u->l[i] & 0x00FF000000000000) >> 48);
		v[j+7] = (uchar8)((u->l[i] & 0xFF00000000000000) >> 56);
	}
}

void gfp256k1pack10(gfe_p256k1_10L *v, const uchar8 *u) {
   	uchar8 i,j,k,l;
	gfe_p256k1_4L t;

	for (i=0;i<NLIMBS;++i) {

		j = i*8;
		t.l[i] = (uint64)u[j]; l = 1;
		for (k=1;k<9;++k)
			t.l[i] |= ((uint64)u[j+l++]  << k*8);
	}

	v->l[0] = ((t.l[0] & 0x0000000003FFFFFF));
	v->l[1] = ((t.l[0] & 0x000FFFFFFC000000) >> 26);
	v->l[2] = ((t.l[0] & 0xFFF0000000000000) >> 52) | ((t.l[1] & 0x0000000000003FFF) << 12);
	v->l[3] = ((t.l[1] & 0x000000FFFFFFC000) >> 14);
	v->l[4] = ((t.l[1] & 0xFFFFFF0000000000) >> 40) | ((t.l[2] & 0x0000000000000003) << 24);
	v->l[5] = ((t.l[2] & 0x000000000FFFFFFC) >>  2);
	v->l[6] = ((t.l[2] & 0x003FFFFFF0000000) >> 28);
	v->l[7] = ((t.l[2] & 0xFFC0000000000000) >> 54) | ((t.l[3] & 0x000000000000FFFF) << 10);
	v->l[8] = ((t.l[3] & 0x000003FFFFFF0000) >> 16);
	v->l[9] = ((t.l[3] & 0xFFFFFC0000000000) >> 42);
}

void gfp256k1pack104(gfe_p256k1_4L *v, const gfe_p256k1_10L *u) {

	v->l[0] = ((u->l[0] & 0x0000000003FFFFFF))       | ((u->l[1] & 0x0000000003FFFFFF) << 26) | ((u->l[2] & 0x0000000000000FFF) << 52);
    v->l[1] = ((u->l[2] & 0x0000000003FFF000) >> 12) | ((u->l[3] & 0x0000000003FFFFFF) << 14) | ((u->l[4] & 0x0000000000FFFFFF) << 40);
    v->l[2] = ((u->l[4] & 0x0000000003000000) >> 24) | ((u->l[5] & 0x0000000003FFFFFF) <<  2) | ((u->l[6] & 0x0000000003FFFFFF) << 28) | ((u->l[7] & 0x00000000000003FF) << 54);
    v->l[3] = ((u->l[7] & 0x0000000003FFFC00) >> 10) | ((u->l[8] & 0x0000000003FFFFFF) << 16) | ((u->l[9] & 0x00000000003FFFFF) << 42);
}

void gfp256k1pack410(gfe_p256k1_10L *v, const gfe_p256k1_4L *u) {

	v->l[0] = ((u->l[0] & 0x0000000003FFFFFF));
	v->l[1] = ((u->l[0] & 0x000FFFFFFC000000) >> 26);
	v->l[2] = ((u->l[0] & 0xFFF0000000000000) >> 52) | ((u->l[1] & 0x0000000000003FFF) << 12);
	v->l[3] = ((u->l[1] & 0x000000FFFFFFC000) >> 14);
	v->l[4] = ((u->l[1] & 0xFFFFFF0000000000) >> 40) | ((u->l[2] & 0x0000000000000003) << 24);
	v->l[5] = ((u->l[2] & 0x000000000FFFFFFC) >>  2);
	v->l[6] = ((u->l[2] & 0x003FFFFFF0000000) >> 28);
	v->l[7] = ((u->l[2] & 0xFFC0000000000000) >> 54) | ((u->l[3] & 0x000000000000FFFF) << 10);
	v->l[8] = ((u->l[3] & 0x000003FFFFFF0000) >> 16);
	v->l[9] = ((u->l[3] & 0xFFFFFC0000000000) >> 42);
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

void unpack_vector(gfe_p256k1_10L *a0, gfe_p256k1_10L *a1, gfe_p256k1_10L *a2, gfe_p256k1_10L *a3, const vec *V) {

	uchar8 i;
	
	for (i=0; i<NLIMBS_VEC_10;++i) {
		a0->l[i] = V[i][0];
		a1->l[i] = V[i][1];
		a2->l[i] = V[i][2];
		a3->l[i] = V[i][3];
	}
}