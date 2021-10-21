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

.data

.globl neg_p0
.globl neg_p1
.globl neg_p23
.globl neg_p4
.globl neg_xor
.globl vec977
.globl vec15632
.globl vecmask22
.globl vecmask26
.globl vecmask26d
.globl vecmask2226d
.globl vecmask32

.globl SECP256K1_N_0
.globl SECP256K1_N_1
.globl SECP256K1_N_2
.globl SECP256K1_N_3

.globl SECP256K1_N_C_0
.globl SECP256K1_N_C_1
.globl SECP256K1_N_C_2

.p2align 5

neg_p0          : .long 0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF
neg_p1          : .long 0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF
neg_p23         : .long 0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF
neg_p4          : .long 0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF
neg_xor    	    : .long -1,-1,-1,-1,-1,-1,-1,-1
vec977          : .quad 977,977,977,977
vec15632		: .quad 15632,15632,15632,15632
vecmask22       : .quad 0x3FFFFF,0x3FFFFF,0x3FFFFF,0x3FFFFF
vecmask26       : .quad 0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF
vecmask26d      : .long 0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF
vecmask2226d    : .long 0x3FFFFFF,0x3FFFFF,0x3FFFFFF,0x3FFFFF,0x3FFFFFF,0x3FFFFF,0x3FFFFFF,0x3FFFFF
vecmask32       : .quad 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF

SECP256K1_N_0   : .quad	0xBFD25E8CD0364141 
SECP256K1_N_1   : .quad	0xBAAEDCE6AF48A03B 
SECP256K1_N_2   : .quad	0xFFFFFFFFFFFFFFFE 
SECP256K1_N_3   : .quad	0xFFFFFFFFFFFFFFFF 

SECP256K1_N_C_0 : .quad	0x402DA1732FC9BEBF 
SECP256K1_N_C_1	: .quad 0x4551231950B75FC4 
SECP256K1_N_C_2	: .quad                  1 
