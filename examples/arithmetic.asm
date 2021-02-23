iconst example1
jmp

# unsigned addition
    example1:
        iconst 3
        iconst 4
        uadd
        dump
        pop

    iconst exit
    jmp

# unsigned subtration
    example2:
        iconst 4
        iconst 3
        usub
        dump
        pop

    iconst exit
    jmp

# unsigned multiplication
    example3:
        iconst 3
        iconst 4
        umul
        dump
        pop

    iconst exit
    jmp

# unsigned division
    example4:
        iconst 8
        iconst 4
        udiv
        dump
        pop

    iconst exit
    jmp

# unsigned modulus
    example5:
        iconst 8
        iconst 4
        umod
        dump
        pop

    iconst exit
    jmp

# unsigned power (raising to a power)
    example6:
        iconst 2
        iconst 3
        upow
        dump
        pop

    iconst exit
    jmp

# signed addition
    example7:
        iconst 3
        iconst 4
        neg
        add
        dump
        pop

    iconst exit
    jmp

# signed subtration
    example8:
        iconst 4
        iconst 3
        neg
        sub
        dump
        pop

    iconst exit
    jmp

# signed multiplication
    example9:
        iconst 3
        iconst 4
        neg
        mul
        dump
        pop

    iconst exit
    jmp

# signed division
    example10:
        iconst 8
        iconst 4
        neg
        div
        dump
        pop

    iconst exit
    jmp

# signed modulus
    example11:
        iconst 8
        iconst 4
        neg
        mod
        dump
        pop

    iconst exit
    jmp

# signed power (raising to a power)
    example12:
        iconst 2
        iconst 3
        neg
        pow
        dump
        pop

    iconst exit
    jmp

# left-shifting
    example13:
        iconst 4
        iconst 2
        shl
        dump
        pop

    iconst exit
    jmp

# right-shifting
    example14:
        iconst 4
        iconst 2
        shr
        dump
        pop

    iconst exit
    jmp

# incrementing and decrementing
    example15:
        iconst 5
        inc
        dump
        pop

        iconst 8
        dec
        dump
        pop

    iconst exit
    jmp

# comparison
    example16:
        iconst 4
        iconst 9
        cmp             # we will actually have '-1'...so it'll be a big unsigned number
        dump

        iconst 1
        add             # we will get 0, because 1 + -1 = 0
        dump
        pop

    iconst exit
    jmp

# we're done
exit:
    halt
