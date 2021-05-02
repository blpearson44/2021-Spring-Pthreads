#
# if you don't give make a 'target', it will try the first one:
#
default: test

#
# you will start with the serial version:
#
sumsq: sumsq.c
	@echo "==== (compiling 'sumsq') ===="
	gcc -g -O2 --std=c99 -Wall -o sumsq sumsq.c

#
# eventually you will need to create a parallel version:
#
par_sumsq: par_sumsq.c
	@echo "==== (compiling 'par_sumsq') ===="
	gcc --std=c99  -o par_sumsq par_sumsq.c -lpthread 

#
# whenever you want to 'clean and try again', do 'make clean':
#
clean:
	@echo "==== Removing files ... ===="
	rm -f sumsq par_sumsq test.txt


#
# below is some makefile magic, don't worry about how it works for now
#
test: sumsq par_sumsq test.txt test2.txt test3.txt
#	@echo "==== Test 01 (baseline - serial) ===="
#	@echo ">>>> Result should be 14 2 1 3"
#	@echo ">>>> ... with 1 worker 'real' time should be 8 seconds"
#	time -p ./sumsq ./test.txt 2>&1
#	@echo ">>>> (above, you should see 'real' as 8 seconds and the others as zero)"
#	@echo ">>>> (make sure to read the Project notes to understand *why* it's 8 seconds)"
#	@echo "==== Test 01 (DONE) ===="

	@echo "==== Test 01 ===="
	@echo "    (1 thread)"
	time -p ./par_sumsq ./test.txt 1
	@echo "    (2 threads)"
	time -p ./par_sumsq ./test.txt 2
	@echo "=================="	

	@echo "==== Test 02 ===="
	@echo "    (1 thread)   "
	time -p ./par_sumsq ./test2.txt 1
	@echo "    (2 threads)"
	time -p ./par_sumsq ./test2.txt 2
	@echo "    (3 threads)"
	time -p ./par_sumsq ./test2.txt 3
	@echo "    (4 threads)"
	time -p ./par_sumsq ./test2.txt 4
	@echo "=================="

	@echo "==== Test 03 ===="
	@echo "    (1 thread)   "
	time -p ./par_sumsq ./test3.txt 1
	@echo "    (2 threads)"
	time -p ./par_sumsq ./test3.txt 2
	@echo "    (3 threads)"
	time -p ./par_sumsq ./test3.txt 3
	@echo "    (4 threads)"
	time -p ./par_sumsq ./test3.txt 4
	@echo "=================="

test.txt:
	@echo "==== (creating test.txt) ===="
	@printf "p 1\nw 2\np 2\np 3\n" > test.txt
test2.txt:
	@echo "==== (creating test2.txt) ===="
	@printf "p 1\np 1\nw 2\np 1\np 1\np 1\np 1\n" > test2.txt
test3.txt:
	@echo "==== (creating test3.txt) ===="
	@printf "p 1\np 1\nw 3\np 1\np 1\np 1\np 1\n" > test3.txt
