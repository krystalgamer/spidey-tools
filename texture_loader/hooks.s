extern _OpenFileFromDisk, _fileName, _fileDirectory, _buffer, _curPkr, _psxId, _PVRIdHandler, _printf, _option, _get_texture

section .data

section .text

global _FreadHook, _CreateTexturePVRInIdHook, _ConvertVQHook, _addMenuEntryRedirect

;File loader related
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
lea eax, [esp+0x5C]
mov [_curPkr], eax
jmp _OpenFileFromDisk

;Texture loader related
_CreateTexturePVRInIdHook:
mov eax, [esp+0x1D4]
mov [_psxId], eax
jmp _PVRIdHandler

%define ConvertVQToBmpAdd 0x005115D0
_ConvertVQHook:
mov eax, [esp+0xCC]
push dword [esp+8]
push dword [esp+8]
push eax
call _get_texture
add esp, 0xC
test eax, eax
jz noTexture
ret
noTexture:
mov eax, ConvertVQToBmpAdd
jmp eax 

;Options related
_addMenuEntryRedirect:
mov dword [esp], _addOptionsEntry
mov eax, 0x0043FFF0
jmp eax

_addOptionsEntry:
mov ecx, esi
push dword [_option]
push dword 0x00497171
push dword 0x0043FFF0
ret; good ol' push and ret call
