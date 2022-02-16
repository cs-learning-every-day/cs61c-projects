.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 32
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 33
# =======================================================
dot:

    # Prologue
    li t0, 1

    blt a2, t0, length_error # if a2 < 1 then length_error
    blt a3, t0, stride_error
    blt a4, t0, stride_error

    li t0, 0 # result = 0
    li t1, 0 # index
    
    # int占4个byte, 下一个int地址就为前一个+stride*4
    slli a3, a3, 2
    slli a4, a4, 2

loop_start:
    # tmp = v1[i] * v2[i]
    li t2, 0
    lw t3, 0(a0)
    lw t4, 0(a1)
    mul t2, t3, t4
    
    # save
    add t0, t0, t2

loop_continue:
    add a0, a0, a3
    add a1, a1, a4
    addi t1, t1, 1
    
    bge t1, a2 loop_end # if index > n then loop_end
    j loop_start

loop_end:
    add a0, t0, x0
    ret

length_error:
    li a1, 32
    j exit2

stride_error:
    li a1, 33
    j exit2
