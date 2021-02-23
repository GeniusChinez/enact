iconst example1
jmp

# exchanging data
    example1:
        iconst 3
        iconst 0
        istore

        iconst 6
        iconst 20
        istore

        iconst 0
        iload
        iconst 20
        iload
        dump
        pop
        pop

        iconst 0
        iconst 20
        xchg

        iconst 0
        iload
        iconst 20
        iload
        dump
        pop
        pop

        iconst exit
        jmp

# loading an integer constant
    example2:
        iconst 3
        dump

        iconst exit
        jmp

# popping data off the stack
    example3:
        iconst 33
        dump
        pop
        dump

        iconst exit
        jmp

# loading data from memory onto the stack
    example4:
        iconst 48
        iconst 10
        istore
        iconst 10
        iload
        dump

        iconst exit
        jmp

# storing data in memory from the stack
    example5:
        iconst 97
        iconst 10
        istore
        iconst 10
        iload
        dump

        iconst exit
        jmp

exit:
    halt
