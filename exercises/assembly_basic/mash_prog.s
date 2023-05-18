# read x, read y (entered on separate lines)
# call mash(x, y), passing args on stack
# check that result is 10*x + y

   .text
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   li   $v0, 5             # read x into t0
   syscall
   move $t0, $v0

   li   $v0, 5             # read y into t1
   syscall
   move $t1, $v0

   # call mash(x,y), passing args x,y on stack
   subu $sp, $sp, 8        # grow stack for args x, y
   sw   $t1, 8($sp)        # push y
   sw   $t0, 4($sp)        # push x

   jal  mash               # rval (in v0) = mash(x, y)

   move $a0, $v0           # print_int(rval)
   li   $v0, 1
   syscall
   li   $v0, 11            # print_char(\n)
   li   $a0, 10
   syscall

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
# mash(x, y)

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
