/*
+-----------------------------------------------------------------------------+
| This code corresponds to the Galois field F(2^256-2^32-977) from the paper  |
| "Efficient Arithmetic In (Pseudo-)Mersenne Prime Order Fields" authored by  |
| Kaushik Nath,  Indian Statistical Institute, Kolkata, India, and            |
| Palash Sarkar, Indian Statistical Institute, Kolkata, India.	              |
+-----------------------------------------------------------------------------+
| Copyright (c) 2018, Kaushik Nath and Palash Sarkar.                         |
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
q = n * c
On call: %rdi = &q, %rsi = &n, %rdx = &c
*/

.p2align 5
.globl gfp256k1mulc
gfp256k1mulc:

push    %rbp
push    %rbx
push    %r12
push    %r13
push    %r14
push    %r15
    
movq    %rdx, %rbx

// multiply first limb
movq    0(%rbx), %rdx    
xorq    %r13, %r13

mulx    0(%rsi), %r8, %r9
mulx    8(%rsi), %rcx, %r10
adcx    %rcx, %r9     

mulx    16(%rsi), %rcx, %r11
adcx    %rcx, %r10    

mulx    24(%rsi), %rcx, %r12
adcx    %rcx, %r11
adcx    %r13, %r12

// reduce result
xorq     %rbp, %rbp
movq     twoe32p977, %rdx

mulx    %r12, %rax, %r12
adcx    %rax, %r8
adox    %r12, %r9

adcx    zero, %r9
adox    zero, %r10

adcx    zero, %r10
adox    zero, %r11

adcx    zero, %r11

adox    zero, %rbp
adcx    zero, %rbp

mulx    %rbp, %rax, %r12
addq    %rax, %r8
adcq    %r12, %r9
adcq    $0, %r10
adcq    $0, %r11
xorq    %r12, %r12
adcq    $0, %r12

mulx    %r12, %rax, %r12
addq    %rax, %r8
adcq    %r12, %r9

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
