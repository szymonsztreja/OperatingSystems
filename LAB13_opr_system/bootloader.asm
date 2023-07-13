ORG 0x7C00
[bits 16]

   XOR AX, AX
   MOV DS, AX
   MOV ES, AX
   MOV SS, AX
   MOV SP, 0x8000

   MOV DH, 17
   MOV BX, 0x9000
   CALL get_kernel

   cli
   lgdt [gdtr]

   MOV EAX, CR0      
   OR AL, 1
   MOV CR0, EAX
   JMP CODE_SEG:init_pm

   [bits 32]
   init_pm:
      MOV AX, DATA_SEG
      MOV DS, AX
      MOV SS, AX
      MOV ES, AX
      MOV FS, AX
      MOV GS, AX
      MOV EBP, 0x90000
      MOV ESP, EBP

   PUSH EBX
   MOV EBX, 0xB8000
   MOV [EBX], BYTE 'O'
   ADD EBX, 2
   MOV [EBX], BYTE 'S'
   POP EBX

   CALL 0x9000
   JMP $


   [bits 16]
   %include "gdt.inc"

   get_kernel:
   PUSH DX
   MOV AH, 0x2
   MOV AL, DH
   MOV CL, 0x2
   MOV CH, 0x0
   MOV DH, 0x0
   INT 0x13
   POP DX
   RET



   TIMES 510 - ($-$$) DB 0
   DW 0xAA55   