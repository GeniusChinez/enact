iconst example1
jmp

# jump
    example1:
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
    example2:
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
    example3:
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
    example4:
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
    example5:
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
    example6:
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
    example7:
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
    example8:
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
    example9:
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
    example10:
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
    example11:
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
    example12:
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
    example13:
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
    example14:
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
    example15:
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
