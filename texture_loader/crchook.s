section .text

%define fileName ebp-4
%define directory ebp-8


global _CrcHook

_CrcHook:
push ebp
mov ebp, esp
sub esp, 8

;get file name and directory
mov eax, [esp+12]
mov [fileName], eax
mov eax, [esp+24]
mov [directory], eax


mov esp, ebp
pop ebp
ret

