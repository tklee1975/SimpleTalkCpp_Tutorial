.686
.XMM

.model flat, c

option casemap :none

.code

asm_add_cpp PROC arg_dst:DWORD, arg_a:DWORD, arg_b:DWORD, arg_size:DWORD
	mov ecx, arg_size
	mov esi, arg_a
	mov ebx, arg_b
	mov edi, arg_dst

my_loop:
	movss xmm0, DWORD PTR[esi]
	addss xmm0, DWORD PTR[ebx]
	movss DWORD PTR[edi], xmm0

	add esi, 16;
	add ebx, 16;
	add edi, 16;

	dec ecx
	jnz my_loop

	ret
asm_add_cpp ENDP

END