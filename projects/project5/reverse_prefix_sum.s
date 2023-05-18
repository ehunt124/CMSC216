# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
reverse_prefix_sum:
   # PROLOGUE
   subu $sp, $sp, 16
   sw	$ra, 16($sp)
   sw	$fp, 12($sp)
   li   $s0, 0        # initialize r to 0
   sw   $s0, 8($sp)   # save r on stack
   sw   $a0, 4($sp)   # save arr on stack
   addu	$fp, $sp, 16

   # BODY
   lw   $t0, ($a0)        # *arr into temporary register
   beq  $t0, -1, return   # if *arr == -1
   
   add  $s0, $s0, $t0     # r = r + *arr
   addi $a0, $a0, 4       # arr++
   jal  reverse_prefix_sum

   lw   $s0, 8($sp)     # restore r
   lw   $t0, 4($sp)     # restore arr
   add  $s0, $s0, $v0   # r = (r + *arr) + reverse_prefix_sum(arr+1)
   sw   $s0, ($t0)      # *arr = r
   move $v0, $s0
   j epilogue

return:
   li   $v0, 0  # return 0

epilogue:
   # EPILOGUE
   move $sp, $fp
   lw   $ra, ($fp)
   lw   $fp, -4($sp)
   j    $ra