/*
+-----------------------------------------------------------------------------+
| Copyright (c) 2021, Timothy James Shelton.								  |
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
// q = n + p
// On call: %rdi = &q, %rsi = &n, %rdx = &p

.p2align 5
.globl gfp256k1add
gfp256k1add:

push    %rbp
push    %rbx
push    %r12
push    %r13
push    %r14
push    %r15

// Move p into registers
movq    0(%rdx), %r8
movq    8(%rdx), %r9
movq    16(%rdx), %r10
movq    24(%rdx), %r11

// n + p
addq    0(%rsi), %r8
adcx    8(%rsi), %r9
adcx    16(%rsi), %r10
adcx    24(%rsi), %r11

// set rbx = delta if carry
movq    zero, %rbx
cmovc   twoe32p977, %rbx

// q + delta if carry
addq    %rbx, %r8
adcx    zero, %r9
adcx    zero, %r10
adcx    zero, %r11

// set rbx = delta if carry
movq    zero, %rbx
cmovc   twoe32p977, %rbx

// q + delta if carry
addq    %rbx, %r8
adcx    zero, %r9
adcx    zero, %r10
adcx    zero, %r11

// q definitely reduced so move to return
movq    %r8,   0(%rdi)
movq    %r9,   8(%rdi)
movq    %r10, 16(%rdi)
movq    %r11, 24(%rdi)

pop     %r15
pop     %r14
pop     %r13
pop     %r12
pop     %rbx
pop     %rbp

ret
