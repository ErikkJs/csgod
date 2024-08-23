# Start of the actual Makefile
# set a default level for the logging
LOGLEVEL?=3 # 0-none, 1-low, 2-med, 3 high

# this will be the compiler we want to use allong with all the options
# -Wall     ( with all warnings)
# -std=c++11  ( use c++ 2011 version  of the ANSI standards)
# set the macro LOGLEVEL to be defined with a value
COMP=g++ -Wall -std=c++11 -D LOGLEVEL=$(LOGLEVEL) 



# all is the default case , ours is to build "runme"
all:runme1 
	
# clean up all the .o files, log files and other misc stuff
clean:
	rm  -f runme* *~  core *.log *.gch  functions.o 

# clean up all the .o files and recompile everything
rebuild: clean runme1  

runme1:  main1.cpp    
	$(COMP) -o runme1 main1.cpp    


# DO NOT DELETE THIS LINE -- make depend depends on it.




