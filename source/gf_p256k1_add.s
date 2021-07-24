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
