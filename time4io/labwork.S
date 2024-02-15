  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  
.global delay
.global time2string

hexasc:#Ska returna v0	# Ska ha en inparameter a0 
	andi $a0, $a0, 0xF
	li $v0, 0x30
	add $v0, $v0, $a0
	slti $t0, $v0, 0x3A
	beq $t0, 0, checkifletter
	jr $ra
	nop
	
checkifletter:
	addi $v0, $v0, 7
	jr $ra
	nop
	
delay: 
	PUSH $ra			# pushes the return address
	jal whileloop
	nop
whileloop:
	ble $a0, $0, donewhile
	nop
	sub $a0,$a0,1			# ms = ms - 1
	li $t0, 0
	li $t1, 4840			# inner delay for loop
	j forloop
	nop
donewhile:				# returns from the nested loops to the main function
	POP $ra
	jr $ra
	nop
forloop:
	bge $t0, $t1, donefor
	nop
	addi $t0, $t0, 1		# i = i + 1
	j forloop
	nop
donefor:
	j whileloop			# returns to while loop
	nop

time2string:
	PUSH $ra
	PUSH $s2
	PUSH $s3
	PUSH $s4
	PUSH $s5
	PUSH $s6
	
	move $s2, $a0		# Moves $a0 pointer for future use in Hexasc
	sb $0, 5($s2)		# Adds a null operator to finish printing the string
	li $t7, 0x3a		
	sb $t7, 2($s2)		# These lines are responsible for the colon
	
	andi $s3, $a1, 0xF	# Extracts the bits from $a1
	andi $s4, $a1, 0xF0
	andi $s5, $a1, 0xF00
	andi $s6, $a1, 0xF000
	srl $s4, $s4, 4		# Shifts the bits to the 4 LSB
	srl $s5, $s5, 8
	srl $s6, $s6, 12
	
	add $a0, $s6, $0	# 10 x minutes added
	jal hexasc
	nop
	sb $v0, 0($s2)
	
	add $a0, $s5, $0	# 1 x minutes added
	jal hexasc
	nop
	sb $v0, 1($s2)
	
	add $a0, $s4, $0	# 10 x seconds added
	jal hexasc
	nop
	sb $v0, 3($s2)
	
	add $a0, $s3, $0	# 1 x seconds added
	jal hexasc
	nop
	sb $v0, 4($s2)
	
	POP $s6
	POP $s5
	POP $s4
	POP $s3
	POP $s2
	POP $ra
	jr $ra
	nop
