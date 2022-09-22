# Case1
## Conditions
```
Arch:     amd64-64-little
RELRO:    No RELRO
Stack:    Canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

## Writeup
- SSPが有効であり、スタックカナリアが書き変わると__stack_chk_fail関数が呼ばれ、プログラムが強制終了する。
- FSAを用いてGOT overwirteを行うことで__stack_chk_fail関数のGOTエントリにret命令のアドレスを書き込んでこれを無効化する。

