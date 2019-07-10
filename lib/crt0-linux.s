	section .text
	[BITS 32]

; user provided entry point
extern Start

; executable entry point
  global _start
_start:

  ; branch to user entry point
  call Start

  ; move return value into ebx
  mov ebx, eax
  ; syscall 1 is sys_exit
  mov eax, 1
  ; call sycall handler
  int 80h

