.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 64
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 67
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 65
# ==============================================================================
write_matrix:

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
    mv s3, a3

    # open file
    mv a1, s0
    li a2, 1
    jal fopen
    blt a0, x0, open_error
    mv s4, a0 # s4 = fd

    # write row and col
    li a0, 8
    jal malloc
    beq a0, x0, malloc_eror

    mv s5, a0 # the adress of the row and column
    sw s2, 0(a0)
    sw s3, 4(a0)

    mv a1, s4
    mv a2, s5
    li a3, 2
    li a4, 4
    jal fwrite
    li t0, 2
    blt a0, t0, write_error

    mul s5, s2, s3

    # write matrix
    mv a1, s4
    mv a2, s1
    mv a3, s5
    li a4, 4
    jal fwrite
    blt a0, s5, write_error

    # close file
    mv a1, s4
    jal fclose
    blt a0, x0, close_error


    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw ra, 24(sp)
    addi sp, sp, 28

    ret

open_error:
    li a1, 64
    j exit2

write_error:
    li a1, 67
    j exit2

close_error:
    li a1, 65
    j exit2

malloc_eror:
    li a1, 48
    j exit2