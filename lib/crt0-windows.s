	section .text
	[BITS 32]

; user provided entry point
extern Start

; executable entry point
  global _start
_start:

  ; branch to user entry point
  call Start

  ; return as loader code will handle exit for us
  ret

