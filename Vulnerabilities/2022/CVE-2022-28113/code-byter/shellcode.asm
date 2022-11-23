.file	1 "asm_to_hex.c"
add:
	add     $4, $31, 0x124
	add     $4, $4, -0xf4
	lw      $24, -4($4)
	add     $22, $23, $23
	add     $22, $23, $23
	sub     $25, $24, $25
	jalr    $25
	.end	add
