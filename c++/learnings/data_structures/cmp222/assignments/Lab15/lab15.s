# Soto Erik
# CMPS 2240 Assembly
#
# Completed decrypt function.
.text
.global decrypt15
.type decrypt15, @function  # <--- this line is important

decrypt15:
    add %ecx, 2
    ror %cl , 3  	 # rolled left bits are now rotated right
    not %eax		 # perform ones comp on eax. reverse bits
    add %eax, 4      #
    xor %eax, %ecx   # exclusive or between registers.
    ret
