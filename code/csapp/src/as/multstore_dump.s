
m:     file format elf64-x86-64


Disassembly of section .init:

0000000000400428 <_init>:
  400428:       f3 0f 1e fa             endbr64 
  40042c:       48 83 ec 08             sub    $0x8,%rsp
  400430:       48 8b 05 b9 0b 20 00    mov    0x200bb9(%rip),%rax        # 600ff0 <__gmon_start__>
  400437:       48 85 c0                test   %rax,%rax
  40043a:       74 02                   je     40043e <_init+0x16>
  40043c:       ff d0                   callq  *%rax
  40043e:       48 83 c4 08             add    $0x8,%rsp
  400442:       c3                      retq   

Disassembly of section .text:

0000000000400450 <_start>:
  400450:       f3 0f 1e fa             endbr64 
  400454:       31 ed                   xor    %ebp,%ebp
  400456:       49 89 d1                mov    %rdx,%r9
  400459:       5e                      pop    %rsi
  40045a:       48 89 e2                mov    %rsp,%rdx
  40045d:       48 83 e4 f0             and    $0xfffffffffffffff0,%rsp
  400461:       50                      push   %rax
  400462:       54                      push   %rsp
  400463:       49 c7 c0 10 06 40 00    mov    $0x400610,%r8
  40046a:       48 c7 c1 a0 05 40 00    mov    $0x4005a0,%rcx
  400471:       48 c7 c7 8e 05 40 00    mov    $0x40058e,%rdi
  400478:       ff 15 6a 0b 20 00       callq  *0x200b6a(%rip)        # 600fe8 <__libc_start_main@GLIBC_2.2.5>
  40047e:       f4                      hlt    

000000000040047f <.annobin_init.c>:
  40047f:       90                      nop

0000000000400480 <_dl_relocate_static_pie>:
  400480:       f3 0f 1e fa             endbr64 
  400484:       c3                      retq   

0000000000400485 <.annobin__dl_relocate_static_pie.end>:
  400485:       66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
  40048c:       00 00 00 
  40048f:       90                      nop

0000000000400490 <deregister_tm_clones>:
  400490:       48 8d 3d 89 0b 20 00    lea    0x200b89(%rip),%rdi        # 601020 <__TMC_END__>
  400497:       48 8d 05 82 0b 20 00    lea    0x200b82(%rip),%rax        # 601020 <__TMC_END__>
  40049e:       48 39 f8                cmp    %rdi,%rax
  4004a1:       74 15                   je     4004b8 <deregister_tm_clones+0x28>
  4004a3:       48 8b 05 36 0b 20 00    mov    0x200b36(%rip),%rax        # 600fe0 <_ITM_deregisterTMCloneTable>
  4004aa:       48 85 c0                test   %rax,%rax
  4004ad:       74 09                   je     4004b8 <deregister_tm_clones+0x28>
  4004af:       ff e0                   jmpq   *%rax
  4004b1:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
  4004b8:       c3                      retq   
  4004b9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

00000000004004c0 <register_tm_clones>:
  4004c0:       48 8d 3d 59 0b 20 00    lea    0x200b59(%rip),%rdi        # 601020 <__TMC_END__>
  4004c7:       48 8d 35 52 0b 20 00    lea    0x200b52(%rip),%rsi        # 601020 <__TMC_END__>
  4004ce:       48 29 fe                sub    %rdi,%rsi
  4004d1:       48 c1 fe 03             sar    $0x3,%rsi
  4004d5:       48 89 f0                mov    %rsi,%rax
  4004d8:       48 c1 e8 3f             shr    $0x3f,%rax
  4004dc:       48 01 c6                add    %rax,%rsi
  4004df:       48 d1 fe                sar    %rsi
  4004e2:       74 14                   je     4004f8 <register_tm_clones+0x38>
  4004e4:       48 8b 05 0d 0b 20 00    mov    0x200b0d(%rip),%rax        # 600ff8 <_ITM_registerTMCloneTable>
  4004eb:       48 85 c0                test   %rax,%rax
  4004ee:       74 08                   je     4004f8 <register_tm_clones+0x38>
  4004f0:       ff e0                   jmpq   *%rax
  4004f2:       66 0f 1f 44 00 00       nopw   0x0(%rax,%rax,1)
  4004f8:       c3                      retq   
  4004f9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

0000000000400500 <__do_global_dtors_aux>:
  400500:       f3 0f 1e fa             endbr64 
  400504:       80 3d 11 0b 20 00 00    cmpb   $0x0,0x200b11(%rip)        # 60101c <_edata>
  40050b:       75 13                   jne    400520 <__do_global_dtors_aux+0x20>
  40050d:       55                      push   %rbp
  40050e:       48 89 e5                mov    %rsp,%rbp
  400511:       e8 7a ff ff ff          callq  400490 <deregister_tm_clones>
  400516:       c6 05 ff 0a 20 00 01    movb   $0x1,0x200aff(%rip)        # 60101c <_edata>
  40051d:       5d                      pop    %rbp
  40051e:       c3                      retq   
  40051f:       90                      nop
  400520:       c3                      retq   
  400521:       66 66 2e 0f 1f 84 00    data16 nopw %cs:0x0(%rax,%rax,1)
  400528:       00 00 00 00 
  40052c:       0f 1f 40 00             nopl   0x0(%rax)

0000000000400530 <frame_dummy>:
  400530:       f3 0f 1e fa             endbr64 
  400534:       eb 8a                   jmp    4004c0 <register_tm_clones>

0000000000400536 <mult2>:
  400536:       55                      push   %rbp
  400537:       48 89 e5                mov    %rsp,%rbp
  40053a:       48 89 7d e8             mov    %rdi,-0x18(%rbp)
  40053e:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
  400542:       48 8b 45 e8             mov    -0x18(%rbp),%rax
  400546:       48 0f af 45 e0          imul   -0x20(%rbp),%rax
  40054b:       48 89 45 f8             mov    %rax,-0x8(%rbp)
  40054f:       48 8b 45 f8             mov    -0x8(%rbp),%rax
  400553:       5d                      pop    %rbp
  400554:       c3                      retq   

0000000000400555 <multstore>:
  400555:       55                      push   %rbp
  400556:       48 89 e5                mov    %rsp,%rbp
  400559:       48 83 ec 28             sub    $0x28,%rsp
  40055d:       48 89 7d e8             mov    %rdi,-0x18(%rbp)
  400561:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
  400565:       48 89 55 d8             mov    %rdx,-0x28(%rbp)
  400569:       48 8b 55 e0             mov    -0x20(%rbp),%rdx
  40056d:       48 8b 45 e8             mov    -0x18(%rbp),%rax
  400571:       48 89 d6                mov    %rdx,%rsi
  400574:       48 89 c7                mov    %rax,%rdi
  400577:       e8 ba ff ff ff          callq  400536 <mult2>
  40057c:       48 89 45 f8             mov    %rax,-0x8(%rbp)
  400580:       48 8b 45 d8             mov    -0x28(%rbp),%rax
  400584:       48 8b 55 f8             mov    -0x8(%rbp),%rdx
  400588:       48 89 10                mov    %rdx,(%rax)
  40058b:       90                      nop
  40058c:       c9                      leaveq 
  40058d:       c3                      retq   

000000000040058e <main>:
  40058e:       55                      push   %rbp
  40058f:       48 89 e5                mov    %rsp,%rbp
  400592:       b8 00 00 00 00          mov    $0x0,%eax
  400597:       5d                      pop    %rbp
  400598:       c3                      retq   
  400599:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)

00000000004005a0 <__libc_csu_init>:
  4005a0:       f3 0f 1e fa             endbr64 
  4005a4:       41 57                   push   %r15
  4005a6:       49 89 d7                mov    %rdx,%r15
  4005a9:       41 56                   push   %r14
  4005ab:       49 89 f6                mov    %rsi,%r14
  4005ae:       41 55                   push   %r13
  4005b0:       41 89 fd                mov    %edi,%r13d
  4005b3:       41 54                   push   %r12
  4005b5:       4c 8d 25 84 08 20 00    lea    0x200884(%rip),%r12        # 600e40 <__frame_dummy_init_array_entry>
  4005bc:       55                      push   %rbp
  4005bd:       48 8d 2d 84 08 20 00    lea    0x200884(%rip),%rbp        # 600e48 <__init_array_end>
  4005c4:       53                      push   %rbx
  4005c5:       4c 29 e5                sub    %r12,%rbp
  4005c8:       48 83 ec 08             sub    $0x8,%rsp
  4005cc:       e8 57 fe ff ff          callq  400428 <_init>
  4005d1:       48 c1 fd 03             sar    $0x3,%rbp
  4005d5:       74 1f                   je     4005f6 <__libc_csu_init+0x56>
  4005d7:       31 db                   xor    %ebx,%ebx
  4005d9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
  4005e0:       4c 89 fa                mov    %r15,%rdx
  4005e3:       4c 89 f6                mov    %r14,%rsi
  4005e6:       44 89 ef                mov    %r13d,%edi
  4005e9:       41 ff 14 dc             callq  *(%r12,%rbx,8)
  4005ed:       48 83 c3 01             add    $0x1,%rbx
  4005f1:       48 39 dd                cmp    %rbx,%rbp
  4005f4:       75 ea                   jne    4005e0 <__libc_csu_init+0x40>
  4005f6:       48 83 c4 08             add    $0x8,%rsp
  4005fa:       5b                      pop    %rbx
  4005fb:       5d                      pop    %rbp
  4005fc:       41 5c                   pop    %r12
  4005fe:       41 5d                   pop    %r13
  400600:       41 5e                   pop    %r14
  400602:       41 5f                   pop    %r15
  400604:       c3                      retq   

0000000000400605 <.annobin___libc_csu_fini.start>:
  400605:       66 66 2e 0f 1f 84 00    data16 nopw %cs:0x0(%rax,%rax,1)
  40060c:       00 00 00 00 

0000000000400610 <__libc_csu_fini>:
  400610:       f3 0f 1e fa             endbr64 
  400614:       c3                      retq   

Disassembly of section .fini:

0000000000400618 <_fini>:
  400618:       f3 0f 1e fa             endbr64 
  40061c:       48 83 ec 08             sub    $0x8,%rsp
  400620:       48 83 c4 08             add    $0x8,%rsp
  400624:       c3                      retq   