section .data
slm_mat4_mul_mask:  dd 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
slm_mat4_transpose_indices: dd 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15

section .text

global slm_mat4_mul, slm_mat4_transpose

; void slm_mat4_mul(Matrix4 *m, Vector *v1, Vector *v2);
slm_mat4_mul:
    vbroadcastf32x4  zmm0, [rdx]
    vmovdqu64        zmm1, [slm_mat4_mul_mask - $]
    vpermilps        zmm0, zmm0, zmm1
    vmulps           zmm0, [rcx]
    vextractf64x4    ymm1, zmm0, 0x0
    vextractf64x4    ymm0, zmm0, 0x1
    vaddps           ymm0, ymm1
    vextractf64x2    xmm1, ymm0, 0x0
    vextractf64x2    xmm0, ymm0, 0x1
    addps            xmm0, xmm1
    movups           [r8], xmm0
    ret

; void slm_mat4_transpose(Matrix4 *m1, Matrix4 *m2);
slm_mat4_transpose:
    vmovdqa64  zmm0, [slm_mat4_transpose_indices - $]
    vpermd     zmm1, zmm0, [rcx]
    vmovdqu64 [rdx], zmm1
    ret
