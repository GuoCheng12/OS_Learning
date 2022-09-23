.586
.MODEL FLAT, C

.DATA
	X dword 5, 2, 3

.CODE
start:
	;mov ebx, offset X
	;push ebx
	mov ebx, 2
	push ebx
	call myProc

myProc:
		push ebp
		mov ebp, esp
		mov eax, [ebp+8]
		pop ebp
		ret
end start

END

