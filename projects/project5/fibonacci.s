# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
fibonacci:
   # PROLOGUE
   subu $sp, $sp, 12
   sw   $ra, 12($sp)
   sw   $fp, 8($sp)
   sw   $a0, 4($sp) # stores the argument n on the stack
   addu $fp, $sp, 12

   # BODY
   beq   $a0, 1, return
   beqz  $a0, return

   sub   $a0, $a0, 1 # f(n - 1)
   jal   fibonacci

   lw    $a0, 4($sp) # restore n into $a0 for next call
   sub   $a0, $a0, 2 # f(n - 2)
   sw    $v0, 4($sp)  # replace n with f(n - 1) on stack
   jal   fibonacci

   lw    $t0, 4($sp) # get f(n - 1) from stack
   add   $v0, $v0, $t0 #  f(n - 2) + f(n - 1)
   j     epilogue

return:
   move  $v0, $a0
   
epilogue:
   # EPILOGUE
   move	$sp, $fp
   lw	   $ra, ($fp)
   lw	   $fp, -4($sp)
   jr 	$ra