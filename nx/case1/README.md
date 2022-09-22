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
- ROP chainを構築してshellを起動する。
- 最終的にsystem関数を呼ぶことになるのだが、system関数は内部で16byteアラインメントを要求する命令を使用するのでアラインメントに気を付けないとSegmentation Faultが出ることに注意。
