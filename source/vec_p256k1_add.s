// vector addition and reduction
// <q1, q2, q3, q4> = <n1, n2, n3, n4> + <p1, p2, p3, p4> 
// input: %rdi = &q, %rsi = &n, %rdx = &p

.p2align 5
.globl vecp256k1add
vecp256k1add:

// Move callee saved registers to stack

// Load <n1,n2,n3,n4>
vmovdqa     0(%rsi),    %ymm0
vmovdqa     32(%rsi),   %ymm1
vmovdqa     64(%rsi),   %ymm2
vmovdqa     96(%rsi),   %ymm3
vmovdqa     128(%rsi),  %ymm4
vmovdqa     160(%rsi),  %ymm5
vmovdqa     192(%rsi),  %ymm6
vmovdqa     224(%rsi),  %ymm7
vmovdqa     256(%rsi),  %ymm8
vmovdqa     288(%rsi),  %ymm9

// <n1',n2',n3',n4'> ← Pack-N2D(<n1,n2,n3,n4>)
vpsllq      $32,%ymm5,%ymm5
vpor        %ymm5,%ymm0,%ymm0
vpsllq      $32,%ymm6,%ymm6
vpor        %ymm6,%ymm1,%ymm1
vpsllq      $32,%ymm7,%ymm7
vpor        %ymm7,%ymm2,%ymm2
vpsllq      $32,%ymm8,%ymm8
vpor        %ymm8,%ymm3,%ymm3
vpsllq      $32,%ymm9,%ymm9
vpor        %ymm9,%ymm4,%ymm4

// Store <n1',n2',n3',n4'>
//vmovdqa     %ymm0,0(%rsi)
//vmovdqa     %ymm1,32(%rsi)
//vmovdqa     %ymm2,64(%rsi)
//vmovdqa     %ymm3,96(%rsi)
//vmovdqa     %ymm4,128(%rsi)

// Load <p1,p2,p3,p4>
vmovdqa     0(%rdx),    %ymm5
vmovdqa     32(%rdx),   %ymm6
vmovdqa     64(%rdx),   %ymm7
vmovdqa     96(%rdx),   %ymm8
vmovdqa     128(%rdx),  %ymm9
vmovdqa     160(%rdx),  %ymm10
vmovdqa     192(%rdx),  %ymm11
vmovdqa     224(%rdx),  %ymm12
vmovdqa     256(%rdx),  %ymm13
vmovdqa     288(%rdx),  %ymm14

// <p1',p2',p3',p4'> ← Pack-N2D(<p1,p2,p3,p4>)
vpsllq      $32,%ymm10,%ymm10
vpor        %ymm10,%ymm5,%ymm5
vpsllq      $32,%ymm11,%ymm11
vpor        %ymm11,%ymm6,%ymm6
vpsllq      $32,%ymm12,%ymm12
vpor        %ymm12,%ymm7,%ymm7
vpsllq      $32,%ymm13,%ymm13
vpor        %ymm13,%ymm8,%ymm8
vpsllq      $32,%ymm14,%ymm14
vpor        %ymm14,%ymm9,%ymm9

// <q1',q2',q3',q4'> ← Add(<n1',n2',n3',n4'>, <p1',p2',p3',p4'>)
vpaddq      %ymm0,%ymm5,%ymm10
vpaddq      %ymm1,%ymm6,%ymm11
vpaddq      %ymm2,%ymm7,%ymm12
vpaddq      %ymm3,%ymm8,%ymm13
vpaddq      %ymm4,%ymm9,%ymm14

//reduction chain reduce3(<q1',q2',q3',q4'>)
vpsrld      $26,%ymm10,%ymm15
vpaddd      %ymm15,%ymm11,%ymm11
vpand       vecmask26d,%ymm10,%ymm10

vpsrld      $26,%ymm11,%ymm15
vpaddd      %ymm15,%ymm12,%ymm12
vpand       vecmask26d,%ymm11,%ymm11

vpsrld      $26,%ymm12,%ymm15
vpaddd      %ymm15,%ymm13,%ymm13
vpand       vecmask26d,%ymm12,%ymm12

vpsrld      $26,%ymm13,%ymm15
vpaddd      %ymm15,%ymm14,%ymm14
vpand       vecmask26d,%ymm13,%ymm13

vpsrld      $26,%ymm14,%ymm15
vpsllq      $32,%ymm15,%ymm15
vpaddd      %ymm15,%ymm10,%ymm10

vpsrlq      $54,%ymm14,%ymm15
vpslld      $6,%ymm15,%ymm15
vpaddd      %ymm15,%ymm11,%ymm11
vpsrld      $6,%ymm15,%ymm15
vpmuludq    vec977,%ymm15,%ymm15
vpaddd      %ymm15,%ymm10,%ymm10
vpand       vecmask2226d,%ymm14,%ymm14

//reduction chain reduce3(<q1',q2',q3',q4'>) [Only needed to ensure complete reduction]
vpsrld      $26,%ymm10,%ymm15
vpaddd      %ymm15,%ymm11,%ymm11
vpand       vecmask26d,%ymm10,%ymm10

vpsrld      $26,%ymm11,%ymm15
vpaddd      %ymm15,%ymm12,%ymm12
vpand       vecmask26d,%ymm11,%ymm11

vpsrld      $26,%ymm12,%ymm15
vpaddd      %ymm15,%ymm13,%ymm13
vpand       vecmask26d,%ymm12,%ymm12

vpsrld      $26,%ymm13,%ymm15
vpaddd      %ymm15,%ymm14,%ymm14
vpand       vecmask26d,%ymm13,%ymm13

vpsrld      $26,%ymm14,%ymm15
vpsllq      $32,%ymm15,%ymm15
vpaddd      %ymm15,%ymm10,%ymm10

vpsrlq      $54,%ymm14,%ymm15
vpslld      $6,%ymm15,%ymm15
vpaddd      %ymm15,%ymm11,%ymm11
vpsrld      $6,%ymm15,%ymm15
vpmuludq    vec977,%ymm15,%ymm15
vpaddd      %ymm15,%ymm10,%ymm10
vpand       vecmask2226d,%ymm14,%ymm14

// <q1,q2,q3,q4> ← Pack-D2N(<q1',q2',q3',q4'>)
vpsrlq      $32,%ymm10,%ymm0
vpand       vecmask32,%ymm10,%ymm10
vpsrlq      $32,%ymm11,%ymm1
vpand       vecmask32,%ymm11,%ymm11
vpsrlq      $32,%ymm12,%ymm2
vpand       vecmask32,%ymm12,%ymm12
vpsrlq      $32,%ymm13,%ymm3
vpand       vecmask32,%ymm13,%ymm13
vpsrlq      $32,%ymm14,%ymm4
vpand       vecmask32,%ymm14,%ymm14

// Store <q1,q2,q3,q4>
vmovdqa     %ymm10, 0(%rdi)
vmovdqa     %ymm11, 32(%rdi)
vmovdqa     %ymm12, 64(%rdi)
vmovdqa     %ymm13, 96(%rdi)
vmovdqa     %ymm14, 128(%rdi)
vmovdqa     %ymm0,  160(%rdi)
vmovdqa     %ymm1,  192(%rdi)
vmovdqa     %ymm2,  224(%rdi)
vmovdqa     %ymm3,  256(%rdi)
vmovdqa     %ymm4,  288(%rdi)

//restore callee saved registers

ret
