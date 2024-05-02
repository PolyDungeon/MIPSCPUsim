addi r1 r2 0x1
addi r2 r1 0x2
add r3 r1 r2
add r4 r2 r3
sub r5 r3 r4
slt r6 r3 r4
sw r4 0x40(r7)
lw r7 0x40(r8)