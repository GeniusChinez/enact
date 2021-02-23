# A naive implementation of functions

#*******************************************************************************
# bootstrap code

iconst done_executing
iconst main
jmp

done_executing:
    # the return value is at the top of the stack...
    iconst exit
    jmp

#*******************************************************************************

################################################################################
#           function sum_plus_1(a, b):
#               return a + b + 1
#
#           function main():
#               x = 1
#               y = 2
#               z = sum_plus_1(x, y)
#
################################################################################

sum_plus_1:
    add
    inc

    # make sure the return value address is at the top of the stack, and the 
    # return value beneath it.
    flip

    # store the return value at the return address
    istore

    # return to the function return address
    jmp

main:
    # record the address which to jump to after calling the function
    iconst sum_call_0_ret

    # record the memory address at which to put the result
    iconst 20

    # pass in the arguments
    iconst 1
    iconst 2

    # jump to the function's location
    iconst sum_plus_1
    jmp

    # this is where we come after the function has returned
    sum_call_0_ret:
        # load the memory location where we said the return value should be put
        iconst 20 
        iload

        # let's print the stack just to see the result. The other value is the
        # address which to jump to after returning from this function.
        dump

        # remove the value from the stack.. we don't need it anymore
        pop

        # put main's return value on the stack
        iconst 0

        # make sure the return value is beneath the return address on the stack
        flip

        # return from the function
        jmp

exit:
    # now we're exiting. The exit code is the only value left on the stack
    halt
