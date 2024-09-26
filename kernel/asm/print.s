section .data
  hex_buffer db '0123456789ABCDEF', 0  ; Character buffer for hex digits

section .text
  global Krnl_print_decimal            ; Export the Krnl_print_decimal function
  global Krnl_print_hex                ; Export the Krnl_print_hex function

; Function to print an integer value to a buffer
; Arguments:
;   EAX - pointer to the buffer
;   EDI - the integer to print
Krnl_print_decimal:
  push edi                            ; Save the integer value
  xor ecx, ecx                        ; Clear ECX to use as a counter
  mov ebx, 10                         ; Base 10 for decimal conversion

Krnl_print_decimal_convert:
  xor edx, edx                        ; Clear EDX for division
  div ebx                             ; EAX / 10; EAX = quotient, EDX = remainder
  push dx                             ; Push remainder (digit) onto the stack
  inc ecx                             ; Increment digit count
  test eax, eax                       ; Check if quotient is zero
  jnz Krnl_print_decimal_convert      ; If not zero, continue converting

Krnl_print_decimal_print_loop:
  pop dx                              ; Pop digit from stack
  add dl, '0'                         ; Convert digit to ASCII
  mov [eax], dl                       ; Store character in buffer
  inc eax                             ; Move to the next character in the buffer
  loop Krnl_print_decimal_print_loop   ; Repeat for all digits

  mov byte [eax], 0                   ; Null-terminate the string
  pop edi                             ; Restore the original integer
  ret

; Function to print a hexadecimal value to a buffer
; Arguments:
;   EAX - pointer to the buffer
;   EDI - the integer to print
Krnl_print_hex:
  push edi                            ; Save the integer value
  mov ecx, 8                          ; We need to print 8 hex digits (32 bits)
  mov ebx, 16                         ; Base 16 for hexadecimal conversion

Krnl_print_hex_print_loop:
  mov edx, edi                        ; Copy EDI (integer) to EDX for manipulation
  shr edx, 28                         ; Shift right to get the most significant hex digit
  and edx, 0xF                        ; Isolate the last 4 bits
  mov dl, [hex_buffer + edx]         ; Get the corresponding hex character
  mov [eax], dl                       ; Store character in buffer
  inc eax                             ; Move to the next character in the buffer
  shl edi, 4                          ; Shift left by 4 bits for the next digit
  loop Krnl_print_hex_print_loop      ; Repeat for all hex digits

  mov byte [eax], 0                   ; Null-terminate the string
  pop edi                             ; Restore the original integer
  ret

