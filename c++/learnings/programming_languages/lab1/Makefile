# Makefile: 
#        It uses the GNU C++ Compiler with debugging
#        will create a single executable called 'lab01'
# ---------------------------------------------------------------
# the compiler
CPP = g++ 
# compiler flags
CFLAGS = -L/usr/lib/cmplrs/cxx -DPOSIX_4D9   -w -g
# this linker flag will link in the math library libm.a 
LFLAGS = -lm
#
RM = rm -f
# ----------------------------------------------------------------
# Explanation of macros:
#     $< is any dependent file which is out of file1
#     $@ is the target's fullname
#
# add suffix .cpp since it is not a default with make utility 
.SUFFIXES:	.cpp .o
#

# make sure you include list.h as a dependency since it is a template class
HFILES=		list.h range_error.h 

# dependencies 
#
default:	lab01	
#
lab01:		lab01.cpp $(HFILES)
	g++ -g -o lab01 lab01.cpp 
#
clean:
	${RM} core lab01 log err

