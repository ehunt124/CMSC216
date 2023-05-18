# Name: Ethan Hunt  UID: 117171051  DirID: ehunt124
is_palindrome:
   # PROLOGUE
   subu $sp, $sp, 12
   sw   $ra, 12($sp)
   sw   $fp, 8($sp)
   sw   $a0, 4($sp)           # store string pointer on stack
   addu $fp, $sp, 12

   # BODY
   jal  strlen
   lw   $t0, 4($sp)           # restore string pointer

   li   $t2, 0
   move $t1, $v0
   div  $v0, $v0, 2

# t0 = strpoint, v0 = len/2,  t1 = len,  t2 = i,  t3 = strpoint + i, 
# t4 = len - i - 1, t5 = str at t3, t6 = str at t4

loop:
   bge  $t2, $v0, return1     # if i >= len/2 loop ends
   add  $t3, $t0, $t2         # t3 = str pointer + i
   lb   $t5, ($t3)            # load char from string at i into t5
   sub  $t4, $t1, $t2         # t4 = length - i
   sub  $t4, 1                # t4 = (length - i) - 1
   add  $t4, $t0, $t4         # t4 = str pointer + ((length - i) - 1)
   lb   $t6, ($t4)            # load char from string at (length - i)- 1 into t6
   bne  $t5, $t6, return0     # end the loop if string[i] != string[len - i - 1]
   addi $t2, 1                # i++
   j    loop

return0:
   li   $v0, 0                # return 0
   j    epilogue

return1:
   li   $v0, 1                # return 1

epilogue:
   # EPILOGUE
   move $sp, $fp
   lw	  $ra, ($fp)
   lw	  $fp, -4($sp)
   jr   $ra


strlen:
   # PROLOGUE
   subu $sp, $sp, 8
   sw	  $ra, 8($sp)
   sw	  $fp, 4($sp)
   addu $fp, $sp, 8

   # BODY
   li   $t0, 0                # stores length of string
strlen_loop:
   lb   $t1, ($a0)            # load char from string
   beqz $t1, end_strlen_loop  # \0 == 0 so this checks for null byte
   add  $a0, $a0, 1           # increment the pointer to next char
   add  $t0, $t0, 1           # increment the length
   j    strlen_loop
end_strlen_loop:
   move $v0, $t0

   # EPILOGUE
   move $sp, $fp
   lw	  $ra, ($fp)
   lw	  $fp, -4($sp)
   jr   $ra