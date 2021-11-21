section .text

global slm_submit_result
slm_submit_result:
    ; NOP used for avoid compiler optimizations
    xor rax, rax
    ret
