// General multiplication of two vector quantities using (5+5)-karatsuba
// <q1, q2, q3, q4> = <n1, n2, n3, n4> * <p1, p2, p3, p4> 
// input: %rdi = &q, %rsi = &n, %rdx = &p

.p2align 5
.globl vecp256k1mul
vecp256k1mul:

// Move callee saved registers to stack and prepare local variables
movq 	  %rsp,%r11
andq 	  $-32,%rsp
subq 	  $928,%rsp

movq 	  %r11,0(%rsp)

// Load <n1,n2,n3,n4>
// vmovdqa     0(%rsi),    %ymm0
// vmovdqa     32(%rsi),   %ymm1
// vmovdqa     64(%rsi),   %ymm2
// vmovdqa     96(%rsi),   %ymm3
// vmovdqa     128(%rsi),  %ymm4
// vmovdqa     160(%rsi),  %ymm5
// vmovdqa     192(%rsi),  %ymm6
// vmovdqa     224(%rsi),  %ymm7
// vmovdqa     256(%rsi),  %ymm8
// vmovdqa     288(%rsi),  %ymm9

// Load <p1,p2,p3,p4>
// vmovdqa     0(%rdx),    %ymm5
// vmovdqa     32(%rdx),   %ymm6
// vmovdqa     64(%rdx),   %ymm7
// vmovdqa     96(%rdx),   %ymm8
// vmovdqa     128(%rdx),  %ymm9
// vmovdqa     160(%rdx),  %ymm10
// vmovdqa     192(%rdx),  %ymm11
// vmovdqa     224(%rdx),  %ymm12
// vmovdqa     256(%rdx),  %ymm13
// vmovdqa     288(%rdx),  %ymm14

// <q1,q2,q3,q4> ← Mul(<n1,n2,n3,n4>, <p1,p2,p3,p4>) (27 local variables)

// load upper 5 limbs of first multiplicand
vmovdqa     160(%rsi),  %ymm0
vmovdqa     192(%rsi),  %ymm1
vmovdqa     224(%rsi),  %ymm2
vmovdqa     256(%rsi),  %ymm3
vmovdqa     288(%rsi),  %ymm4

// load upper 5 limbs of second multiplicand
vmovdqa     160(%rdx),  %ymm5
vmovdqa     192(%rdx),  %ymm6
vmovdqa     224(%rdx),  %ymm7
vmovdqa     256(%rdx),  %ymm8
vmovdqa     288(%rdx),  %ymm9

// perform (schoolbook) multiplication of upper 5 limbs and store on the stack (9 local variables - start from 32(%rsp))
vpmuludq    %ymm5,%ymm0,%ymm15
vmovdqa     %ymm15,32(%rsp)

vpmuludq    %ymm6,%ymm0,%ymm15
vpmuludq    %ymm5,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,64(%rsp)

vpmuludq    %ymm7,%ymm0,%ymm15
vpmuludq    %ymm6,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,96(%rsp)

vpmuludq    %ymm8,%ymm0,%ymm15
vpmuludq    %ymm7,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,128(%rsp)

vpmuludq    %ymm9,%ymm0,%ymm15
vpmuludq    %ymm8,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,160(%rsp)

vpmuludq    %ymm9,%ymm1,%ymm15
vpmuludq    %ymm8,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,192(%rsp)

vpmuludq    %ymm9,%ymm2,%ymm15
vpmuludq    %ymm8,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,224(%rsp)

vpmuludq    %ymm9,%ymm3,%ymm15
vpmuludq    %ymm8,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,256(%rsp)

vpmuludq    %ymm9,%ymm4,%ymm15
vmovdqa     %ymm15,288(%rsp)

// load Lower 5 limbs of first multiplicand
vmovdqa     0(%rsi),    %ymm10
vmovdqa     32(%rsi),   %ymm11
vmovdqa     64(%rsi),   %ymm12
vmovdqa     96(%rsi),   %ymm13
vmovdqa     128(%rsi),  %ymm14

// Add the lower 5 limbs to the upper 5 limbs of each multiplicand
// First
vpaddq      %ymm10,%ymm0,%ymm0
vpaddq      %ymm11,%ymm1,%ymm1
vpaddq      %ymm12,%ymm2,%ymm2
vpaddq      %ymm13,%ymm3,%ymm3
vpaddq      %ymm14,%ymm4,%ymm4
//Second
vpaddq      0(%rdx),  %ymm5,%ymm5
vpaddq      32(%rdx), %ymm6,%ymm6
vpaddq      64(%rdx), %ymm7,%ymm7
vpaddq      96(%rdx), %ymm8,%ymm8
vpaddq      128(%rdx),%ymm9,%ymm9

// perform (schoolbook) multiplication of lower 5 limbs and store on the stack (9 local variables)
// at each stage add the corresponding upper limb product and store on the stack (9 local variables)
vpmuludq    0(%rdx),%ymm10,%ymm15
vmovdqa     %ymm15,320(%rsp)
vpaddq      32(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,608(%rsp)

vpmuludq    32(%rdx),%ymm10,%ymm15
vpmuludq    0(%rdx),%ymm11,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,352(%rsp)
vpaddq      64(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,640(%rsp)

vpmuludq    64(%rdx),%ymm10,%ymm15
vpmuludq    32(%rdx),%ymm11,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    0(%rdx),%ymm12,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,384(%rsp)
vpaddq      96(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,672(%rsp)

vpmuludq    96(%rdx),%ymm10,%ymm15
vpmuludq    64(%rdx),%ymm11,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    32(%rdx),%ymm12,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    0(%rdx),%ymm13,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,416(%rsp)
vpaddq      128(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,704(%rsp)

vpmuludq    128(%rdx),%ymm10,%ymm15
vpmuludq    96(%rdx),%ymm11,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    64(%rdx),%ymm12,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    32(%rdx),%ymm13,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     128(%rsi),%ymm10
vpmuludq    0(%rdx),%ymm10,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,448(%rsp)
vpaddq      160(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,736(%rsp)

vpmuludq    128(%rdx),%ymm11,%ymm15
vpmuludq    96(%rdx),%ymm12,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    64(%rdx),%ymm13,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    32(%rdx),%ymm10,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,480(%rsp)
vpaddq      192(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,768(%rsp)

vpmuludq    128(%rdx),%ymm12,%ymm15
vpmuludq    96(%rdx),%ymm13,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    64(%rdx),%ymm10,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,512(%rsp)
vpaddq      224(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,800(%rsp)

vpmuludq    128(%rdx),%ymm13,%ymm15
vpmuludq    96(%rdx),%ymm10,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vmovdqa     %ymm15,544(%rsp)
vpaddq      256(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,832(%rsp)

vpmuludq    128(%rdx),%ymm10,%ymm15
vmovdqa     %ymm15,576(%rsp)
vpaddq      288(%rsp),%ymm15,%ymm15
vmovdqa     %ymm15,864(%rsp)

// perform (schoolbook) multiplication of mixed 5 limbs
vpmuludq    %ymm5,%ymm0,%ymm15
vmovdqa     %ymm15,896(%rsp)

vpmuludq    %ymm6,%ymm0,%ymm15
vpmuludq    %ymm5,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm10

vpmuludq    %ymm7,%ymm0,%ymm15
vpmuludq    %ymm6,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm11

vpmuludq    %ymm8,%ymm0,%ymm15
vpmuludq    %ymm7,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm12

vpmuludq    %ymm9,%ymm0,%ymm15
vpmuludq    %ymm8,%ymm1,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm5,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm13

vpmuludq    %ymm9,%ymm1,%ymm15
vpmuludq    %ymm8,%ymm2,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm6,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm0

vpmuludq    %ymm9,%ymm2,%ymm15
vpmuludq    %ymm8,%ymm3,%ymm14
vpaddq      %ymm14,%ymm15,%ymm15
vpmuludq    %ymm7,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm1

vpmuludq    %ymm9,%ymm3,%ymm15
vpmuludq    %ymm8,%ymm4,%ymm14
vpaddq      %ymm14,%ymm15,%ymm2

vpmuludq    %ymm9,%ymm4,%ymm3

vmovdqa     896(%rsp),%ymm9

// Subtract the sum of upper and lower limb products from the mixed limb products
// Add the overlap of upper or lower limb products to the mixed limb products
vpsubq      608(%rsp),%ymm9,%ymm9
vpaddq    480(%rsp),%ymm9,%ymm9
vpsubq      640(%rsp),%ymm10,%ymm10
vpaddq    512(%rsp),%ymm10,%ymm10
vpsubq      672(%rsp),%ymm11,%ymm11
vpaddq    544(%rsp),%ymm11,%ymm11
vpsubq      704(%rsp),%ymm12,%ymm12
vpaddq    576(%rsp),%ymm12,%ymm12
vpsubq      736(%rsp),%ymm13,%ymm13
//vpaddq    32(%rsp),%ymm13,%ymm13
vpsubq      768(%rsp),%ymm0,%ymm0
vpaddq    32(%rsp),%ymm0,%ymm0
vpsubq      800(%rsp),%ymm1,%ymm1
vpaddq    64(%rsp),%ymm1,%ymm1
vpsubq      832(%rsp),%ymm2,%ymm2
vpaddq    96(%rsp),%ymm2,%ymm2
vpsubq      864(%rsp),%ymm3,%ymm3
vpaddq    128(%rsp),%ymm3,%ymm3

// perform reduction using algorithm 6.1 <h1,h2,h3,h4> = reduce1(<c1,c2,c3,c4>)
vpsrlq      $26,%ymm0,%ymm14
vpaddq      %ymm14,%ymm1,%ymm1
vpand       vecmask26,%ymm0,%ymm0
vpsllq      $36,%ymm0,%ymm15
vpmuludq    vec15632,%ymm0,%ymm0
vpaddq      %ymm15,%ymm0,%ymm0
vpaddq      320(%rsp),%ymm0,%ymm0

vpsrlq      $26,%ymm1,%ymm14
vpaddq      %ymm14,%ymm2,%ymm2
vpand       vecmask26,%ymm1,%ymm1
vpsllq      $36,%ymm1,%ymm15
vpmuludq    vec15632,%ymm1,%ymm1
vpaddq      %ymm15,%ymm1,%ymm1
vpaddq      352(%rsp),%ymm1,%ymm1

vpsrlq      $26,%ymm2,%ymm14
vpaddq      %ymm14,%ymm3,%ymm3
vpand       vecmask26,%ymm2,%ymm2
vpsllq      $36,%ymm2,%ymm15
vpmuludq    vec15632,%ymm2,%ymm2
vpaddq      %ymm15,%ymm2,%ymm2
vpaddq      384(%rsp),%ymm2,%ymm2

vpsrlq      $26,%ymm3,%ymm4
vpaddq      160(%rsp),%ymm4,%ymm4
vpand       vecmask26,%ymm3,%ymm3
vpsllq      $36,%ymm3,%ymm15
vpmuludq    vec15632,%ymm3,%ymm3
vpaddq      %ymm15,%ymm3,%ymm3
vpaddq      416(%rsp),%ymm3,%ymm3

vpsrlq      $26,%ymm4,%ymm5
vpaddq      192(%rsp),%ymm5,%ymm5
vpand       vecmask26,%ymm4,%ymm4
vpsllq      $36,%ymm4,%ymm15
vpmuludq    vec15632,%ymm4,%ymm4
vpaddq      %ymm15,%ymm4,%ymm4
vpaddq      448(%rsp),%ymm4,%ymm4

vpsrlq      $26,%ymm5,%ymm6
vpaddq      224(%rsp),%ymm6,%ymm6
vpand       vecmask26,%ymm5,%ymm5
vpsllq      $36,%ymm5,%ymm15
vpmuludq    vec15632,%ymm5,%ymm5
vpaddq      %ymm15,%ymm5,%ymm5
vpaddq      %ymm9,%ymm5,%ymm5

vpsrlq      $26,%ymm6,%ymm7
vpaddq      256(%rsp),%ymm7,%ymm7
vpand       vecmask26,%ymm6,%ymm6
vpsllq      $36,%ymm6,%ymm15
vpmuludq    vec15632,%ymm6,%ymm6
vpaddq      %ymm15,%ymm6,%ymm6
vpaddq      %ymm10,%ymm6,%ymm6

vpsrlq      $26,%ymm7,%ymm8
vpaddq      288(%rsp),%ymm8,%ymm8
vpand       vecmask26,%ymm7,%ymm7
vpsllq      $36,%ymm7,%ymm15
vpmuludq    vec15632,%ymm7,%ymm7
vpaddq      %ymm15,%ymm7,%ymm7
vpaddq      %ymm11,%ymm7,%ymm7

vpsrlq      $26,%ymm8,%ymm9
vpand       vecmask26,%ymm8,%ymm8
vpsllq      $36,%ymm8,%ymm15
vpmuludq    vec15632,%ymm8,%ymm8
vpaddq      %ymm15,%ymm8,%ymm8
vpaddq      %ymm12,%ymm8,%ymm8

vpsllq      $36,%ymm9,%ymm15
vpmuludq    vec15632,%ymm9,%ymm9
vpaddq      %ymm15,%ymm9,%ymm9
vpaddq      %ymm13,%ymm9,%ymm9

// perform reduction chain reduce2(<q1,q2,q3,q4>)
//5->6
vpsrlq      $26,%ymm5,%ymm10
//vpaddq      %ymm10,%ymm6,%ymm6

vpsrlq      $32,%ymm6,%ymm11
vpand       vecmask32,%ymm6,%ymm6
vpaddq      %ymm10,%ymm6,%ymm6
vpsrlq      $32,%ymm6,%ymm12
vpand       vecmask32,%ymm6,%ymm6
vpaddq      %ymm11,%ymm12,%ymm11
vpsrlq      $32,%ymm11,%ymm12
vpsllq      $32,%ymm11,%ymm11
vpaddq      %ymm11,%ymm6,%ymm6

vpand       vecmask26,%ymm5,%ymm5

//0->1
vpsrlq      $26,%ymm0,%ymm10
//vpaddq      %ymm10,%ymm1,%ymm1

vpsrlq      $32,%ymm1,%ymm13
vpand       vecmask32,%ymm1,%ymm1
vpaddq      %ymm10,%ymm1,%ymm1
vpsrlq      $32,%ymm1,%ymm14
vpand       vecmask32,%ymm1,%ymm1
vpaddq      %ymm13,%ymm14,%ymm13
vpsrlq      $32,%ymm13,%ymm14
vpsllq      $32,%ymm13,%ymm13
vpaddq      %ymm13,%ymm1,%ymm1

vpand       vecmask26,%ymm0,%ymm0

//6->7
vpsrlq      $26,%ymm6,%ymm10
//vpaddq      %ymm10,%ymm7,%ymm7

vpaddq      %ymm12,%ymm10,%ymm10
vpsrlq      $32,%ymm7,%ymm11
vpand       vecmask32,%ymm7,%ymm7
vpaddq      %ymm10,%ymm7,%ymm7
vpsrlq      $32,%ymm7,%ymm12
vpand       vecmask32,%ymm7,%ymm7
vpaddq      %ymm11,%ymm12,%ymm11
vpsrlq      $32,%ymm11,%ymm12
vpsllq      $32,%ymm11,%ymm11
vpaddq      %ymm11,%ymm7,%ymm7

vpand       vecmask26,%ymm6,%ymm6

//1->2
vpsrlq      $26,%ymm1,%ymm10
//vpaddq      %ymm10,%ymm2,%ymm2

vpaddq      %ymm14,%ymm10,%ymm10
vpsrlq      $32,%ymm2,%ymm13
vpand       vecmask32,%ymm2,%ymm2
vpaddq      %ymm10,%ymm2,%ymm2
vpsrlq      $32,%ymm2,%ymm14
vpand       vecmask32,%ymm2,%ymm2
vpaddq      %ymm13,%ymm14,%ymm13
vpsrlq      $32,%ymm13,%ymm14
vpsllq      $32,%ymm13,%ymm13
vpaddq      %ymm13,%ymm2,%ymm2

vpand       vecmask26,%ymm1,%ymm1

//7->8
vpsrlq      $26,%ymm7,%ymm10
//vpaddq      %ymm10,%ymm8,%ymm8

vpaddq      %ymm12,%ymm10,%ymm10
vpsrlq      $32,%ymm8,%ymm11
vpand       vecmask32,%ymm8,%ymm8
vpaddq      %ymm10,%ymm8,%ymm8
vpsrlq      $32,%ymm8,%ymm12
vpand       vecmask32,%ymm8,%ymm8
vpaddq      %ymm11,%ymm12,%ymm11
vpsrlq      $32,%ymm11,%ymm12
vpsllq      $32,%ymm11,%ymm11
vpaddq      %ymm11,%ymm8,%ymm8

vpand       vecmask26,%ymm7,%ymm7

//2->3
vpsrlq      $26,%ymm2,%ymm10
//vpaddq      %ymm10,%ymm3,%ymm3

vpaddq      %ymm14,%ymm10,%ymm10
vpsrlq      $32,%ymm3,%ymm13
vpand       vecmask32,%ymm3,%ymm3
vpaddq      %ymm10,%ymm3,%ymm3
vpsrlq      $32,%ymm3,%ymm14
vpand       vecmask32,%ymm3,%ymm3
vpaddq      %ymm13,%ymm14,%ymm13
vpsrlq      $32,%ymm13,%ymm14
vpsllq      $32,%ymm13,%ymm13
vpaddq      %ymm13,%ymm3,%ymm3

vpand       vecmask26,%ymm2,%ymm2

//8->9
vpsrlq      $26,%ymm8,%ymm10
vpaddq      %ymm12,%ymm10,%ymm10
vpaddq      %ymm10,%ymm9,%ymm9
vpand       vecmask26,%ymm8,%ymm8

//3->4
vpsrlq      $26,%ymm3,%ymm10
vpaddq      %ymm10,%ymm4,%ymm4

vpaddq      %ymm14,%ymm10,%ymm10
vpsrlq      $32,%ymm4,%ymm13
vpand       vecmask32,%ymm4,%ymm4
vpaddq      %ymm10,%ymm4,%ymm4
vpsrlq      $32,%ymm4,%ymm14
vpand       vecmask32,%ymm4,%ymm4
vpaddq      %ymm13,%ymm14,%ymm13
vpsrlq      $32,%ymm13,%ymm14
vpsllq      $32,%ymm13,%ymm13
vpaddq      %ymm13,%ymm4,%ymm4

vpand       vecmask26,%ymm3,%ymm3

//9->(0,1)
vpsrlq      $22,%ymm9,%ymm10
vpsllq      $6,%ymm10,%ymm10
vpaddq      %ymm10,%ymm1,%ymm1
vpsrlq      $6,%ymm10,%ymm10
vpmuludq    vec977,%ymm10,%ymm10
vpaddq      %ymm10,%ymm0,%ymm0
vpand       vecmask22,%ymm9,%ymm9

//4->5
vpsrlq      $26,%ymm4,%ymm10
vpaddq      %ymm14,%ymm10,%ymm10
vpaddq      %ymm10,%ymm5,%ymm5
vpand       vecmask26,%ymm4,%ymm4

//0->1
vpsrlq      $26,%ymm0,%ymm10
vpaddq      %ymm10,%ymm1,%ymm1
vpand       vecmask26,%ymm0,%ymm0

//5->6
vpsrlq      $26,%ymm5,%ymm10
vpaddq      %ymm10,%ymm6,%ymm6
vpand       vecmask26,%ymm5,%ymm5

//1->2
vpsrlq      $26,%ymm1,%ymm10
vpaddq      %ymm10,%ymm2,%ymm2
vpand       vecmask26,%ymm1,%ymm1

// perform reduction chain reduce2(<q1,q2,q3,q4>) [Only needed to ensure complete reduction]
vpsrlq      $26,%ymm5,%ymm10
vpaddq      %ymm10,%ymm6,%ymm6
vpand       vecmask26,%ymm5,%ymm5

vpsrlq      $26,%ymm0,%ymm10
vpaddq      %ymm10,%ymm1,%ymm1
vpand       vecmask26,%ymm0,%ymm0

vpsrlq      $26,%ymm6,%ymm10
vpaddq      %ymm10,%ymm7,%ymm7
vpand       vecmask26,%ymm6,%ymm6

vpsrlq      $26,%ymm1,%ymm10
vpaddq      %ymm10,%ymm2,%ymm2
vpand       vecmask26,%ymm1,%ymm1

vpsrlq      $26,%ymm7,%ymm10
vpaddq      %ymm10,%ymm8,%ymm8
vpand       vecmask26,%ymm7,%ymm7

vpsrlq      $26,%ymm2,%ymm10
vpaddq      %ymm10,%ymm3,%ymm3
vpand       vecmask26,%ymm2,%ymm2

vpsrlq      $26,%ymm8,%ymm10
vpaddq      %ymm10,%ymm9,%ymm9
vpand       vecmask26,%ymm8,%ymm8

vpsrlq      $26,%ymm3,%ymm10
vpaddq      %ymm10,%ymm4,%ymm4
vpand       vecmask26,%ymm3,%ymm3

vpsrlq      $22,%ymm9,%ymm10
vpsllq      $6,%ymm10,%ymm10
vpaddq      %ymm10,%ymm1,%ymm1
vpsrlq      $6,%ymm10,%ymm10
vpmuludq    vec977,%ymm10,%ymm10
vpaddq      %ymm10,%ymm0,%ymm0
vpand       vecmask22,%ymm9,%ymm9

vpsrlq      $26,%ymm4,%ymm10
vpaddq      %ymm10,%ymm5,%ymm5
vpand       vecmask26,%ymm4,%ymm4

vpsrlq      $26,%ymm0,%ymm10
vpaddq      %ymm10,%ymm1,%ymm1
vpand       vecmask26,%ymm0,%ymm0

vpsrlq      $26,%ymm5,%ymm10
vpaddq      %ymm10,%ymm6,%ymm6
vpand       vecmask26,%ymm5,%ymm5

vpsrlq      $26,%ymm1,%ymm10
vpaddq      %ymm10,%ymm2,%ymm2
vpand       vecmask26,%ymm1,%ymm1

// Store <q1,q2,q3,q4>
vmovdqa     %ymm0, 0(%rdi)
vmovdqa     %ymm1, 32(%rdi)
vmovdqa     %ymm2, 64(%rdi)
vmovdqa     %ymm3, 96(%rdi)
vmovdqa     %ymm4, 128(%rdi)
vmovdqa     %ymm5,  160(%rdi)
vmovdqa     %ymm6,  192(%rdi)
vmovdqa     %ymm7,  224(%rdi)
vmovdqa     %ymm8,  256(%rdi)
vmovdqa     %ymm9,  288(%rdi)

// Return callee saved registers from stack and remove local variables
movq 	  0(%rsp),%r11

movq 	  %r11,%rsp