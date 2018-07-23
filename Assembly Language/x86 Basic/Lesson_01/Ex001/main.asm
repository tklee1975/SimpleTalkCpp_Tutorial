.386
.model flat, stdcall

option casemap :none

.data
.code

mainCRTStartup PROC
	mov ecx, 0

my_label:
	add ecx, 1
	cmp ecx, 3
	jl my_label

	mov eax, 100

mainCRTStartup ENDP
END