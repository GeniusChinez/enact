# unsigned addition
iconst 3
iconst 4
uadd
dump
pop

# unsigned subtration
iconst 4
iconst 3
usub
dump
pop

# unsigned multiplication
iconst 3
iconst 4
umul
dump
pop

# unsigned division
iconst 8
iconst 4
udiv
dump
pop

# unsigned modulus
iconst 8
iconst 4
umod
dump
pop

# unsigned power (raising to a power)
iconst 2
iconst 3
upow
dump
pop

# signed addition
iconst 3
iconst 4
neg
add
dump
pop

# signed subtration
iconst 4
iconst 3
neg
sub
dump
pop

# signed multiplication
iconst 3
iconst 4
neg
mul
dump
pop

# signed division
iconst 8
iconst 4
neg
div
dump
pop

# signed modulus
iconst 8
iconst 4
neg
mod
dump
pop

# signed power (raising to a power)
iconst 2
iconst 3
neg
pow
dump
pop

# left-shifting
iconst 4
iconst 2
shl
dump
pop

# right-shifting
iconst 4
iconst 2
shr
dump
pop

# incrementing and decrementing
iconst 5
inc
dump
pop

iconst 8
dec
dump
pop

# comparison
iconst 4
iconst 9
cmp             # we will actually have '-1'...so it'll be a big unsigned number
dump

iconst 1
add             # we will get 0, because 1 + -1 = 0
dump
pop

# we're done
halt
