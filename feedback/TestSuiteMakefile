#
# UNR 446.646 Spring 2021
# 'testsuite' makefile for Project 5 (pthreads)
#
# run in your virtualbox directory (from Project 4):
# 
#    make -f TestSuiteMakefile
#
# (note that you can also replace you 'Makefile' with this one)
#

# questions to psm@unr.edu

# make sure your file is called 'par_sumsq.c' and nothing else
PAR_SUM=par_sumsq

# all we have to do is run 'make', and everything else should work
# note the use of '@' on echo, it suppresses make printing out
# the command (echo) itself, so you only see the string once
default: test
	@echo "==== (all done) ===="

# this sets compiler default flags:
# -g is for debug info
# -O2 sets a default optimization level (stresses your parallel code a bit)
# --std=c99 says to use the "1999 C Standard" version of "C"
# -Wall says to warn about everything; NOTE: you should fix ALL warnings
FLAGS = -g -O2 --std=c99 -Wall

# Your 'c' file should be called "$(PAR_SUM).c"
# Don't mess with this compile line unless you know what you're doing ...
$(PAR_SUM): $(PAR_SUM).c
	gcc $(FLAGS) -o $(PAR_SUM) $(PAR_SUM).c -lpthread

# And yes, in many cases you can ignore warnings from "-Wall", but
# it's not good practice - some of them might actually matter

# our tests are run automatically, and include reference output and time
# there are two little tricks in how we use 'time' here, first is the
# '-p' flag which defines 'posix' output, namely, multi-line.
# the second trick is '2>&1' which redirects stderr (2) to stdout (1),
# doing this allows us to run something like 'make > results.txt' and
# it will redirect both the output from the program as well as from the
# 'time' command - which defaults to 'stderr'
test01: $(PAR_SUM) test-01.txt
	@echo "==== Test 01 (baseline) ===="
	@echo ">>>> Result should be 14 2 1 3 in all cases"
	@echo ">>>> ... with 1 worker 'real' time should be ~7 seconds"
	time -p ./$(PAR_SUM) ./test-01.txt 1 2>&1
	@echo ">>>> ... with 2 workers 'real' time should be ~5 seconds"
	time -p ./$(PAR_SUM) ./test-01.txt 2 2>&1
	@echo ">>>> ... with 6 workers 'real' time should be ~5 seconds"
	time -p ./$(PAR_SUM) ./test-01.txt 6 2>&1

# here we use a cute feature of make, 'printf' instead of 'echo', so we
# can use newlines ('\n').  this trick allows us to embed test files in
# this makefile, so the only thing we need to distribute is the makefile.
# in other words, this next part creates the 'test-01.txt' file if it's
# not already there.  'make' can do a lot of things ...
test-01.txt:
	@echo ">>>> (creating test-01.txt)"
	@printf "p 1\nw 2\np 2\np 3\n" > test-01.txt

test02: $(PAR_SUM) test-02.txt
	@echo "==== Test 02 (a little more stress) ===="
	@echo ">>>> Result should be 6 6 1 1 in all cases"
	@echo ">>>> ... with 1 worker 'real' time should be ~6 seconds"
	time -p ./$(PAR_SUM) ./test-02.txt 1 2>&1
	@echo ">>>> ... with 2 workers 'real' time should be ~4 seconds"
	time -p ./$(PAR_SUM) ./test-02.txt 2 2>&1
	@echo ">>>> ... with 6 workers 'real' time should be ~3 seconds"
	time -p ./$(PAR_SUM) ./test-02.txt 6 2>&1

test-02.txt:
	@echo ">>>> (creating test-02.txt)"
	@printf "p 1\np 1\nw 2\np 1\np 1\np 1\np 1\n" > test-02.txt

test03: $(PAR_SUM) test-03.txt
	@echo "==== Test 03 (lots of parallelism) ===="
	@echo ">>>> Result should be 110 6 1 5 in all cases"
	@echo ">>>> ... with 3 workers 'real' time should be ~12 seconds"
	time -p ./$(PAR_SUM) ./test-03.txt 3 2>&1
	@echo ">>>> ... with 5 workers 'real' time should be ~9 seconds"
	time -p ./$(PAR_SUM) ./test-03.txt 5 2>&1
	@echo ">>>> ... with 12 workers 'real' time should be ~5 seconds"
	time -p ./$(PAR_SUM) ./test-03.txt 12 2>&1

test-03.txt:
	@echo ">>>> (creating test-03.txt)"
	@printf "p 1\np 2\np 3\np 4\np 5\np 1\np 2\np 3\np 4\np 5\n" > test-03.txt

test04: $(PAR_SUM) test-04.txt
	@echo "==== Test 04 (a little funky) ===="
	@echo ">>>> Result should be 5 5 1 1 in all cases"
	@echo ">>>> ... with 1 workers 'real' time should be ~10 seconds"
	time -p ./$(PAR_SUM) ./test-04.txt 1 2>&1
	@echo ">>>> ... with 4 workers 'real' time should also be ~10 seconds"
	time -p ./$(PAR_SUM) ./test-04.txt 4 2>&1
	@echo ">>>> ... huh, with 1000 workers 'real' time should still be ~10 seconds"
	time -p ./$(PAR_SUM) ./test-04.txt 1000 2>&1
	@echo ">>>> ... (you will have more overhead, but should still be just above 10 sec)"

test-04.txt:
	@echo ">>>> (creating test-04.txt)"
	@printf "p 1\nw 2\np 1\nw 2\np 1\nw 2\np 1\nw 2\np 1\nw 2\n" > test-04.txt


# the 'echo' part will only be run after all the dependencies, which are what
# actually run the tests
test: $(PAR_SUM) test01 test02 test03 test04
	@echo "==== ... all tests done"

clean:
	rm -f $(PAR_SUM) test-01.txt test-02.txt test-03.txt test-04.txt


