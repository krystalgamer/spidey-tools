section .data

openText db "Opening file %s\0"

section .text

%define buffer ebp+4
%define fileSize ebp+8
%define fileHandle ebp+10

%define fileName ebp-4
%define directory ebp-8
%define path ebp-0xC
%define pathLen ebp-0x10

global _FreadHook

extern _fopen, _fread, _fread, _fclose

_FreadHook:
push ebp
mov ebp, esp
sub esp, 0x10

;filename is from the walk pkr part in PKR_ReadFile
; cant take it from the arguments to the function since
; it passes all lower cases and compares with strcmpi(insesitive)
mov eax, [ebp+0x2C]
mov [fileName], eax
mov eax, [ebp+0xAC]
mov [directory], eax

push [fileName]



mov esp, ebp
pop ebp
ret

