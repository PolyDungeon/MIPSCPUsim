lw $1, 5($3)
lw 
add $1, $2, $3
j 0x10010101
sw $1, 10($2)
sub $2, $3, $4
and $5, $6, $4
beq $3, $4, 2
j 0x203000
sw $2, 3($4)
or $7, $1, $3

addi $1, $1, 5
addi $2, $2, 10
addi $3, $3, 1
addi $4, $4, 2
addi $5, $31, 20
add $6, $2, $3
sub $7, $3, $4
and $2, $6, $4
or $8, $7, $3
beq $3, $4, 2
sw $2, 3($4)
lw $10, 3($4)
j 0x2