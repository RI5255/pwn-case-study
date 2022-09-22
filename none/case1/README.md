# Case1
## Conditions
```
Arch:     amd64-64-little
RELRO:    No RELRO
Stack:    No canary found
NX:       NX disabled
PIE:      No PIE (0x400000)
RWX:      Has RWX segments
```

## Writeup
- 何も制約がない。色々と方法があるが、このcaseではshellを立ち上げるshellcodeを書いてそこに処理を飛ばす。