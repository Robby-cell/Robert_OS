section .text
global Krnl_put_pixel

extern Krnl_ScreenWidth
extern Krnl_ScreenHeight

Krnl_put_pixel:
  ; params:
  ;  x = [esp+4] (first argument)
  ;  y = [esp+8] (second argument)
  ;  color = [esp+12] (third argument)

  mov eax, [esp+4]
  mov ebx, [esp+8]

  mov edx, Krnl_ScreenWidth
  mul edx
  add eax, ebx

  ; Load color
  mov bl, [esp+12]
  ; Set the pixel color to a color
  mov [0xA0000 + eax], bl

  ret
