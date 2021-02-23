iconst example1
jmp

# logical AND
    example1:
        iconst 7
        iconst 3
        and
        dump
        pop

    iconst exit
    jmp

# logical OR 
    example2:
        iconst 4
        iconst 1
        or
        dump
        pop

    iconst exit
    jmp

# logical NOT 
    example3:
        iconst 8
        not
        dump
        pop

        iconst exit
        jmp

# logical XOR
    example4:
        iconst 4
        iconst 8
        xor
        dump
        pop

    iconst exit
    jmp

exit:
    halt
