section .data

fileDirectory dw 0x00000000 
fileName dw 0x00000000 

section .text

global _FreadHook, fileDirectory, fileName
extern OpenFileFromDisk

_FreadHook:
push ebp
mov ebp, esp

;filename is from the walk pkr part in PKR_ReadFile
; cant take it from the arguments to the function since
; it passes all lower cases and compares with strcmpi(insesitive)
mov eax, [ebp+0x2C]
mov [fileName], eax
mov eax, [ebp+0xAC]
mov [fileDirectory], eax
jmp OpenFileFromDisk
