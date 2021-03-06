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

#ifndef __P256k1_ARITH_H__
#define __P256k1_ARITH_H__

#include "basic_types.h"
#include "gf_p256k1_type.h"

#define gfp256k1sqr(x,y) gfp256k1nsqr(x,y,1)

extern void gfp256k1sub(gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *);
extern void gfp256k1add(gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *);
extern void gfp256k1mul(gfe_p256k1_4L *, const gfe_p256k1_4L *, const gfe_p256k1_4L *);
extern void gfp256k1mulc(gfe_p256k1_4L *, const gfe_p256k1_4L *, const uint64 *);
extern void gfp256k1neg(gfe_p256k1_4L *, const gfe_p256k1_4L *);
extern void gfp256k1nsqr(gfe_p256k1_4L *, const gfe_p256k1_4L *, const uchar8);
extern void gfp256k1makeunique(gfe_p256k1_4L *);

void gfp256k1inv(gfe_p256k1_4L *, const gfe_p256k1_4L *);
void gfp256k1fltinv(gfe_p256k1_4L *, const gfe_p256k1_4L *);

extern void vecp256k1add(vec *, const vec *, const vec *);
extern void vecp256k1sub(vec *, const vec *, const vec *);
extern void vecp256k1mul(vec *, const vec *, const vec *);

#endif
