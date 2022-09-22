from pwn import *

binfile = './a.out'
context.binary = elf = ELF(binfile)
context.log_level = 'critical'
libc = elf.libc

gs = '''
b main
c
'''

def start():
    if args.GDB:
        return gdb.debug(binfile, gdbscript = gs)
    else:
        return process(binfile)

io = start()