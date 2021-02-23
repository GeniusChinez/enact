# addition
iconst 3
iconst 4
add
dump
pop

# subtration
iconst 4
iconst 3
sub
dump
pop

# multiplication
iconst 3
iconst 4
mul
dump
pop

# division
iconst 8
iconst 4
div
dump
pop

# modulus
iconst 8
iconst 4
mod
dump
pop

# power (raising to a power)
iconst 2
iconst 3
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

# we're done
halt
