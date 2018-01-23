extern _OpenFileFromDisk, _fileName, _fileDirectory, _buffer

section .text

global _FreadHook

_FreadHook:
;filename is from the walk pkr part in PKR_ReadFile
; cant take it from the arguments to the function since
; it passes all lower cases and compares with strcmpi(insesitive)
mov eax, [esp+0x28]
mov [_fileName], eax
mov eax, [esp+0xA8]
mov [_fileDirectory], eax
lea eax, [esp+0x94]
mov [_buffer], eax
jmp _OpenFileFromDisk
