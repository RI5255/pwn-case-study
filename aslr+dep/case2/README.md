# Case2
## Conditions
```
Arch:     amd64-64-little
RELRO:    Partial RELRO
Stack:    No canary found
NX:       NX enabled
PIE:      No PIE (0x400000)
```

## Writeup
ROP stager + return_to_dl_resolve攻撃でASLR+DEPを回避する。return_to_dl_resolve攻撃は使用されているlibcのバージョンが分からなくても使える汎用的な攻撃手法である。詳しくは[この記事](https://zenn.dev/ri5255/articles/f61dcc5c7ffd9f)を参照。


