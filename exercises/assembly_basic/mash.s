# mash(x, y)
# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
mash:
   # PROLOGUE
   subu $sp, $sp, 12
   sw   $ra, 12($sp)
   sw   $fp, 8($sp)
   li   $t0, 10 # loads 10 on stack
   sw   $t0, 4($sp)
   addu $fp, $sp, 12

   # BODY
   lw	$t1, 4($fp)	# $t1 = x
   lw	$t2, 8($fp)	# $t2 = y
   mul $t0, $t0, $t1 # 10 * x = $t0
   add $t0, $t0, $t2 # 10 * x + y = $t0
   move $v0, $t0

   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra
