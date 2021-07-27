// q = n - p
// On call: %rdi = &q, %rsi = &n, %rdx = &p

.p2align 5
.globl gfp256k1sub
gfp256k1sub:

push    %rbp
push    %rbx
push    %r12
push    %r13
push    %r14
push    %r15

// Move n into registers
movq    0(%rsi), %r8
movq    8(%rsi), %r9
movq    16(%rsi), %r10
movq    24(%rsi), %r11

// n - p
subq    0(%rdx), %r8
sbbq    8(%rdx), %r9
sbbq    16(%rdx), %r10
sbbq    24(%rdx), %r11

// set rbx = delta if carry
movq    zero, %rbx
cmovc   twoe32p977, %rbx

// q - delta if carry
subq    %rbx, %r8
sbbq    zero, %r9
sbbq    zero, %r10
sbbq    zero, %r11

// set rbx = delta if carry
movq    zero, %rbx
cmovc   twoe32p977, %rbx

// q - delta if carry
subq    %rbx, %r8
sbbq    zero, %r9
sbbq    zero, %r10
sbbq    zero, %r11

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