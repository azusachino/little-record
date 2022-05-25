.section .data
string:
  .ascii "hello, world\n"
string_end:
  .equ len, string_end - string
.section .text
.globl main
main:
  # first, call write(1, "hello, world\n", 13);
  movq $1, %rax # write is system call 1
  movq $1, %rdi # 
  movq $string, %rsi
  movq $len, %rdx
  syscall


  # call _exit(0)
  movq $60, %rax
  movq $0, %rdi
  syscall