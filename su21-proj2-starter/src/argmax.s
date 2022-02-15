.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# =================================================================
argmax:

    # Prologue
    bne a1, zero, init
    li a1, 32
    j exit2
init:
    lw t0, 0(a0) # max = arr[0]
    li t1, 0 # max index
    li t2, 0 # i
    add t3, a0, x0 # arr

loop_start:
    lw t4, 0(t3)
    blt t4, t0, loop_continue
    # save max value and index
    add t1, t2, x0
    add t0, t4, x0

loop_continue:
    addi t2, t2, 1
    addi t3, t3, 4
    blt t2, a1, loop_start
loop_end:
    # Epilogue
    add a0, t1, x0
    ret