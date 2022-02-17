.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 48
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 64
# - If you receive an fread error or eof,
#   this function terminates the program with error code 66
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 65
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -28
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw ra, 24(sp)

    # save arguments
    mv s0, a0
    mv s1, a1
    mv s2, a2
    li s5, -1

    # open file
    mv a1, s0
    li a2, 0
    ebreak
    jal fopen
    beq a0, s5, open_error
    mv s0, a0 # s0 = file descriptor

    # read file
    # row
    mv a1, s0
    mv a2, s1
    li a3, 4
    jal fread
    beq a0, s5, read_error

    # col
    mv a1, s0
    mv a2, s2
    li a3, 4
    jal fread
    beq a0, s5, read_error

    # get the number of elements
    lw t1, 0(s1)
    lw t2, 0(s2)
    mul s3, t1, t2 
    li t0, 4
    mul s3, s3, t0 # s3 = m * n * 4

    # malloc
    mv a0, s3
    jal malloc
    beq a0, x0, malloc_eror
    mv s4, a0 # s4 = the pointer to the matrix buffer

    # read matrix
    mv a1, s0
    mv a2, s4
    mv a3, s3
    jal fread
    beq a0, s5, read_error
    # Epilogue

    # close file
    mv a1, s0
    jal fclose
    beq a0, s5, close_error
    
    # return result
    mv a0, s4

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw ra, 24(sp)
    addi sp, sp, 28

    ret

malloc_eror:
    li a1, 48
    j exit2

open_error:
    li a1, 64
    j exit2

read_error:
    li a1, 66
    j exit2

close_error:
    li a1, 65
    j exit2