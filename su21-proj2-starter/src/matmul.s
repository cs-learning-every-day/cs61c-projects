.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 34
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 34
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 34
# =======================================================

# 伪代码
# m0: m * n m1: n * k
# for (int i = 0; i < m; i++) 
#   for (int j = 0; j < k; j++)
#       ouput[i][j] = dot(m0[i], m1[j], n, 1, k)
matmul:

    # Error checks
    li t0, 1

    ble a1, t0, error
    ble a2, t0, error
    ble a4, t0, error
    ble a5, t0, error
    bne a2, a4, error


    # Prologue
    addi sp, sp, -32
    
    # 保存寄存器，后面有函数调用
    # 寄存器保存时机见lec09_lec10
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw ra, 28(sp)

    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a4
    mv s5, a5
    mv s6, a6

    li t0, 0 # i = 0
outer_loop_start:
    li t1, 0 # j = 0

inner_loop_start:
    # call dot
    # 准备参数 
    mv a0, s0

    # 求出m1列地址
    ebreak
    li t2, 4
    mul t2, t2, t1
    add a1, s3, t2

    mv a2, s2
    li a3, 1
    mv a4, s5

    # 保存t0、t1寄存器
    addi sp, sp, -8
    sw t0, 0(sp)
    sw t1, 4(sp)

    jal dot
    ebreak
    # save
    sw a0, 0(s6)
    addi s6, s6, 4

    # 恢复
    lw t0, 0(sp)
    lw t1, 4(sp)
    addi sp, sp, 8

    addi t1, t1, 1
    beq t1, s5, inner_loop_end
    j inner_loop_start

inner_loop_end:
    addi t0, t0, 1
    beq t0, s1, outer_loop_end
    # 更新m0 行地址
    li t2, 4
    mul t2, t2, s2
    add s0, s0, t2
    j outer_loop_start

outer_loop_end:
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32
    ret

error:
    li a1, 34
    j exit2