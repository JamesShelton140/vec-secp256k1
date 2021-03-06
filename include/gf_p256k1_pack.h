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

#ifndef __P256k1_PACK_H__
#define __P256k1_PACK_H__

#include "basic_types.h"
#include "gf_p256k1_type.h"

void gfp256k1pack(gfe_p256k1_9L *, const uchar8 *);
void gfp256k1pack94(gfe_p256k1_4L *, const gfe_p256k1_9L *);
void gfp256k1unpack(uchar8 *, const gfe_p256k1_4L *);

void gfp256k1pack10(gfe_p256k1_10L *, const uchar8 *);
void gfp256k1pack104(gfe_p256k1_4L *, const gfe_p256k1_10L *);
void gfp256k1pack410(gfe_p256k1_10L *, const gfe_p256k1_4L *);

void set_vector(vec *, const gfe_p256k1_10L *, const gfe_p256k1_10L *, const gfe_p256k1_10L *, const gfe_p256k1_10L *);
void unpack_vector(gfe_p256k1_10L *, gfe_p256k1_10L *, gfe_p256k1_10L *, gfe_p256k1_10L *, const vec *);

#endif
