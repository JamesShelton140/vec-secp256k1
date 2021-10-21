// q = n - curve order complement 
// On call: %rdi = &q, %rsi = &n 
 
.p2align 5 
.globl curve256k1subc 
curve256k1subc: 
 
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
subq    SECP256K1_N_C_0, %r8 
sbbq    SECP256K1_N_C_1, %r9 
sbbq    SECP256K1_N_C_2, %r10 
sbbq    zero, %r11 
 
// set rbx, r12, r13 = delta if carry 
movq    zero, %rbx 
movq    zero, %r12 
movq    zero, %r13 
cmovc   SECP256K1_N_C_0, %rbx 
cmovc   SECP256K1_N_C_1, %r12 
cmovc   SECP256K1_N_C_2, %r13 
 
// q - delta if carry 
subq    %rbx, %r8 
sbbq    %r12, %r9 
sbbq    %r13, %r10 
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