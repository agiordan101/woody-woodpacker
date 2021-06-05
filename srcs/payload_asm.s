[BITS 64]

global payload:function
global g_payload_len:data
global g_payload_offset:data
global g_payload_jmp_offset:data

g_payload_len dd end - payload
g_payload_offset dd strt - payload
g_payload_jmp_offset dd jmp_offset - payload

section .text

payload:
  woody db "....WOODY.....", 10
  len equ $ - woody
strt:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rdi, 1
  lea rsi, [rel woody]
  mov rdx, len
  mov rax, 0x01
  syscall
  add rsp, 16
  pop rbp
jmp_offset:
  jmp 0xffffffff
  ret
end: