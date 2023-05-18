# max(x, y)
# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
max:
   # PROLOGUE
   subu $sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   bge $a0, $a1, if
   move $v0, $a1
   j endif
if:
   move $v0, $a0
endif:

   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra
