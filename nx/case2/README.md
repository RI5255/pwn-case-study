# Case2
## Conditions
```
Arch:     amd64-64-little
RELRO:    No RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

## Writeup
- ret2libcでmprotectを呼ぶことでスタック領域に実行権限を付加してスタックに置いたshellcodeを実行する

## Reference
ももいろテクノロジー「Return-to-libcとmprotect(2)でDEPを回避してみる」(https://inaz2.hatenablog.com/entry/2014/04/20/010545)
