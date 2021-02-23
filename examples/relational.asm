iconst example1
jmp

# relational equals
    example1:
        iconst 3
        iconst 3
        equ

        iconst 3
        iconst 4
        equ

        dump

        pop
        pop

    iconst exit
    jmp

# relational not equals
    example2:
        iconst 3
        iconst 3
        neq

        iconst 3
        iconst 4
        neq

        dump

        pop
        pop

    iconst exit
    jmp

# relational greater than
    example3:
        iconst 3
        iconst 3
        gtr

        iconst 3
        iconst 4
        gtr

        iconst 4
        iconst 3
        gtr

        dump

        pop
        pop
        pop

    iconst exit
    jmp

# relational greater than or equals
    example4:
        iconst 3
        iconst 3
        gte

        iconst 3
        iconst 4
        gte

        iconst 4
        iconst 3
        gte

        dump

        pop
        pop
        pop

    iconst exit
    jmp

# relational less than
    example5:
        iconst 3
        iconst 3
        lst

        iconst 3
        iconst 4
        lst

        dump

        pop
        pop

    iconst exit
    jmp

# relational less than or equals
    example6:
        iconst 3
        iconst 3
        lte

        iconst 3
        iconst 4
        lte

        iconst 5
        iconst 4
        lte

        dump

        pop
        pop
        pop

    iconst exit
    jmp

exit:
    halt
