section .text
global slm_vec_add, slm_vec_sub, slm_vec_mul, slm_vec_dot, slm_vec_magnitude

; void slm_vec_add(void *v1, void *v2, void *v3);
slm_vec_add:
    movdqu    xmm0,  [rcx]
    movdqu    xmm1,  [rdx]
    addps     xmm0,   xmm1
    movdqu    [r8],   xmm0
    ret

; void slm_vec_sub(void *v1, void *v2, void *v3);
slm_vec_sub:
    movdqu    xmm0,  [rcx]
    movdqu    xmm1,  [rdx]
    subps     xmm0,   xmm1
    movdqu    [r8],   xmm0
    ret

; void slm_vec_mul(void *v1, const float factor, void *v3);
slm_vec_mul:
    vbroadcastss xmm0,   xmm0
    movdqu       xmm1,  [rdx]
    mulps        xmm0,   xmm1
    movdqu       [r8],   xmm0
    ret

; float slm_vec_dot(Vector *v1, Vector *v2, float *product)
slm_vec_dot:
    movdqa   xmm0, [rcx]
    mulps    xmm0, [rdx]
    movhlps  xmm1,  xmm0 ; xmm1[127:0] = (xmm0[127:64], xmm0[127:64])
    addps    xmm0,  xmm1
    movshdup xmm1,  xmm0
    addps    xmm0,  xmm1
    ret

slm_vec_magnitude:
    movdqu   xmm0, [rcx]
    mulps    xmm0,  xmm0
    movhlps  xmm1,  xmm0 ; xmm1[127:0] = (xmm0[127:64], xmm0[127:64])
    addps    xmm0,  xmm1
    movshdup xmm1,  xmm0
    addps    xmm0,  xmm1
    sqrtps   xmm0,  xmm0
    ret
