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

.data

.globl neg_p0
.globl neg_p1
.globl neg_p23
.globl neg_p4
.globl neg_xor
.globl vec977
.globl vecmask22
.globl vecmask26
.globl vecmask26d
.globl vecmask2226d
.globl vecmask32

.p2align 5

neg_p0          : .long 0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF,0x7FFF85F,0x7FFFFFF
neg_p1          : .long 0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF,0x7FFFF7F,0x7FFFFFF
neg_p23         : .long 0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF,0x7FFFFFF
neg_p4          : .long 0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF,0x7FFFFFF,0x7FFFFF
neg_xor    	    : .long -1,-1,-1,-1,-1,-1,-1,-1
vec977          : .quad 977,977,977,977
vecmask22       : .quad 0x3FFFF,0x3FFFF,0x3FFFF,0x3FFFF
vecmask26       : .quad 0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF
vecmask26d      : .long 0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF,0x3FFFFFF
vecmask2226d    : .long 0x3FFFFFF,0x3FFFF,0x3FFFFFF,0x3FFFF,0x3FFFFFF,0x3FFFF,0x3FFFFFF,0x3FFFF
vecmask32       : .quad 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
