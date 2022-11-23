.CODE

GetKernelCallbackTable PROC
    mov rdx,gs:[60h]
    add ecx,ecx
    add rcx, [rdx+58h]
    mov rax, rcx
    ret
GetKernelCallbackTable ENDP

GetPEB PROC
     mov rax, gs:[60h]
     ret
GetPEB ENDP


END