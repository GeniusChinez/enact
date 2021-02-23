# jump
    iconst 8
    dump
    iconst end_of_block
    jmp
    inc
    end_of_block:
        dump
        pop
        iconst exit
        jmp

# jump if true
    iconst 1
    iconst if_test1_true
    jt

    iconst end_of_test1
    jmp

    if_test1_true:
        iconst 10
        dump
        pop

    end_of_test1:
        iconst exit
        jmp

# jump if false
    iconst 1
    iconst if_test2_false
    jf

    iconst end_of_test2
    jmp

    if_test2_false:
        iconst 10
        dump
        pop

    end_of_test2:
        iconst exit
        jmp

# jump if zero
    iconst 1
    iconst if_test3_true
    jz

    iconst end_of_test3
    jmp

    if_test3_true:
        iconst 10
        dump
        pop

    end_of_test3:
        iconst exit
        jmp

# jump if not zero
    iconst 1
    iconst if_test4_true
    jnz

    iconst end_of_test4
    jmp

    if_test4_true:
        iconst 10
        dump
        pop

    end_of_test4:
        iconst exit
        jmp

# jump if equal
    iconst 1
    iconst 2
    iconst if_test5_true
    je

    iconst end_of_test5
    jmp

    if_test5_true:
        iconst 10
        dump
        pop

    end_of_test5:
        iconst exit
        jmp

# jump if not equal
    iconst 1
    iconst 2
    iconst if_test6_true
    jne

    iconst end_of_test6
    jmp

    if_test6_true:
        iconst 10
        dump
        pop

    end_of_test6:
        iconst exit
        jmp

# jump if less than
    iconst 1
    iconst 2
    iconst if_test7_true
    jl

    iconst end_of_test7
    jmp

    if_test7_true:
        iconst 10
        dump
        pop

    end_of_test7:
        iconst exit
        jmp

# jump if not less than
    iconst 1
    iconst 2
    iconst if_test8_true
    jnl

    iconst end_of_test8
    jmp

    if_test8_true:
        iconst 10
        dump
        pop

    end_of_test8:
        iconst exit
        jmp

# jump if less than or equal 
    iconst 1
    iconst 2
    iconst if_test9_true
    jle

    iconst end_of_test9
    jmp

    if_test9_true:
        iconst 10
        dump
        pop

    end_of_test9:
        iconst exit
        jmp

# jump if not less than or equal 
    iconst 1
    iconst 2
    iconst if_test10_true
    jnle

    iconst end_of_test10
    jmp

    if_test10_true:
        iconst 10
        dump
        pop

    end_of_test10:
        iconst exit
        jmp

# jump if greater than
    iconst 1
    iconst 2
    iconst if_test11_true
    jg

    iconst end_of_test11
    jmp

    if_test11_true:
        iconst 11
        dump
        pop

    end_of_test11:
        iconst exit
        jmp

# jump if not greater than
    iconst 1
    iconst 2
    iconst if_test12_true
    jg

    iconst end_of_test12
    jmp

    if_test12_true:
        iconst 11
        dump
        pop

    end_of_test12:
        iconst exit
        jmp

# jump if greater than or equal
    iconst 1
    iconst 2
    iconst if_test13_true
    jge

    iconst end_of_test13
    jmp

    if_test13_true:
        iconst 11
        dump
        pop

    end_of_test13:
        iconst exit
        jmp

# jump if not greater than or equal
    iconst 1
    iconst 2
    iconst if_test14_true
    jnge

    iconst end_of_test14
    jmp

    if_test14_true:
        iconst 11
        dump
        pop

    end_of_test14:
        iconst exit
        jmp

exit:
    halt
