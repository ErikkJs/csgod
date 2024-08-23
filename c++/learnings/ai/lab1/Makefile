CC=g++
CFLAGS=-Wall -std=c++11
RM=rm -f
TESTFILE=setosa_v_versicolor.csv

lab0.out: lab0.o
	$(CC) $(CFLAGS) -o lab0.out lab0.o

lab0.o: lab0.cpp
	$(CC) $(CFLAGS) -c lab0.cpp

clean: 
	$(RM) *.o lab0

test: lab0.out $(TESTFILE)
	./lab0.out $(TESTFILE)
