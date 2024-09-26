section .data
  seed dd 0

section .text
  global Krnl_rand
  global Krnl_init_random_seed

Krnl_init_random_seed:
  mov ah, 0x02            ; BIOS service 0x02, get RTC time
  int 0x1A                ; Call BIOS interrupt
  xor edx, edx
  mov dl, ch
  mov dh, cl
  shl edx, 0x10
  xor eax, eax
  mov al, dh
  mov ah, cl
  or edx, eax
  mov [seed], edx
  ret

Krnl_rand:
  ; LCG formula: Xn+1 = (a * Xn + c) % 2^32
  mov eax, [seed]
  mov ebx, 1664525       ; a
  mul ebx
  add eax, 1013904223    ; a
  mov [seed], eax        ; store new seed
  ret

