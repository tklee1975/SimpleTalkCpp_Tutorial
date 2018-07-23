.386
;.model flat, stdcall
.model flat, c

option casemap :none

.data

	my_msg db "Good Morning", 0

.code

my_hello PROTO

my_print_int PROTO v0:DWORD, v1:DWORD, v2:DWORD

my_print_int_stdcall PROTO stdcall v0:DWORD, v1:DWORD, v2:DWORD

; Obsoleted
; my_print_int__pascal PROTO pascal v0:DWORD, v1:DWORD, v2:DWORD

my_print_string PROTO pStr:DWORD

my_func PROC
	mov eax, 1234H
	call [my_hello]
	ret
my_func ENDP

my_asm_func PROC
	call [my_func]

	push 1
	push 2
	push 3
	call [my_print_int]
	add esp, 12;  caller cleans the stack after the call

	invoke my_print_int, 4, 5, 6

	push 1
	push 2
	push 3
	call [my_print_int_stdcall]

	invoke my_print_int_stdcall, 4, 5, 6

	invoke my_print_string, addr my_msg

	ret

my_asm_func ENDP
END