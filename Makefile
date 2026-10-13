CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O0

# Part 1 -------------------------------------------------------------
test: list.o msort.o test_part1.o
	$(CC) $(CFLAGS) -o test_part1 $^
	./test_part1

valgrind: list.o msort.o test_part1.o
	$(CC) $(CFLAGS) -o test_part1 $^
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./test_part1

# Part 2 -------------------------------------------------------------
# 'audit' keeps -g -O0 so gdb backtraces are easy to read.
# Tip: a crash that depends on input SIZE can be a stack-size limit.
# Check yours with:  ulimit -s   (typically 8192 KB on Linux)
audit: list.c candidates.c fixed.c audit.c
	$(CC) -Wall -Wextra -std=c11 -g -O0 -o audit list.c candidates.c fixed.c audit.c

clean:
	rm -f *.o test_part1 audit
