.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# ==============================================================================
relu:
    # Prologue
    add t0, x0, x0 # i = 0
    add t1, a0, x0 # t1 = arr
    bne a1, x0, loop_start
    li a1, 32
    j exit2

loop_start:
    lw t2, 0(t1) # t2 = arr[i]
    bge t2, x0, loop_continue # if t2 >= 0
    sw x0, 0(t1) # else t2 = 0

loop_continue:
    addi t1, t1, 4
    addi t0, t0, 1
    blt t0, a1, loop_start

loop_end:
    # Epilogue
	ret
