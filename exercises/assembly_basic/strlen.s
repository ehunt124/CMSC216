# strlen(str)
# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
strlen:
   # PROLOGUE
   subu $sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   li $t0, 0 # stores length of string
loop:
   lb $t1, ($a0) # load char from string
   beqz $t1, endloop # \0 == 0 so this checks for null byte
   add $a0, $a0, 1 # increment the pointer to next char
   add $t0, $t0, 1 # increment the length
   j loop
endloop:
   move $v0, $t0

   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra