# relational equals
    iconst 3
    iconst 3
    equ

    iconst 3
    iconst 4
    equ

    dump

    pop
    pop

# relational not equals
    iconst 3
    iconst 3
    neq

    iconst 3
    iconst 4
    neq

    dump

    pop
    pop

# relational greater than
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

# relational greater than or equals
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

# relational less than
    iconst 3
    iconst 3
    lst

    iconst 3
    iconst 4
    lst

    dump

    pop
    pop

# relational less than or equals
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

halt
