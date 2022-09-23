.586
.MODEL FLAT, C

.CODE
PUBLIC minDistance
minDistance PROC
push ebp
mov ebp, esp
sub esp, 12
mov esi, [ebp+8]
mov ecx, 0
L1:
	;k
	mov [ebp-12], ecx
	mov ecx, 0
L2:
	;i
	mov [ebp-4], ecx
	mov ecx, 0
L3:
	;j
	mov [ebp-8], ecx
	mov eax, [ebp-4]
	mov ebx, [ebp+12]
	mul ebx
	add eax, [ebp-8]
	mov ebx, 64h
	cmp [esi+4*eax], ebx
	je @not
		;W[[ebp-4] * count + [ebp-8]] = min(W[[ebp-4] * count + [ebp-8]], W[[ebp-4] * count + [ebp-12]] + W[[ebp-12] * count + [ebp-8]]);
		mov ebx, eax
		xor eax, eax
		mov ax, [ebp-4]
		mov edx, [ebp+12]
		mul edx
		add ax, [ebp-12]
		mov eax, [esi+4*eax]
		push eax
		xor eax, eax
		mov ax, [ebp-12]
		mov edx, [ebp+12]
		mul edx
		add ax, [ebp-8]
		mov eax, dword ptr [esi+4*eax]
		pop edx
		add eax, edx
		cmp dword ptr [esi+4*ebx], eax
		jle @endif
		mov dword ptr [esi+4*ebx], eax
	jmp @endif
	@not:
		;W[[ebp-4] * count + [ebp-8]] = W[[ebp-4] * count + [ebp-12]] + W[[ebp-12] * count + [ebp-8]]
		mov ebx, eax
		xor eax, eax
		mov ax, [ebp-4]
		mov edx, [ebp+12]
		mul edx
		add ax, [ebp-12]
		mov eax, [esi+4*eax]
		push eax
		xor eax, eax
		mov ax, [ebp-12]
		mov edx, [ebp+12]
		mul edx
		add ax, [ebp-8]
		mov eax, dword ptr [esi+4*eax]
		pop edx
		add eax, edx
		mov dword ptr [esi+4*ebx], eax
	@endif:
inc ecx	
cmp ecx, [ebp+12]
je @no1
jmp L3
@no1:
	mov cx, [ebp-4]
inc ecx
cmp ecx, [ebp+12]
je @no2
jmp L2
@no2:
	mov cx, [ebp-12]
inc ecx
cmp ecx, [ebp+12]
je @no3
jmp L1
@no3:
mov eax, [ebp+16]
sub eax, 1
mov ebx, [ebp+12]
mul ebx
mov ebx, [ebp+20]
sub ebx, 1
add eax, ebx
mov ebx, eax
mov eax, [esi+4*ebx]
mov esp, ebp
pop ebp
ret
minDistance ENDP
END
