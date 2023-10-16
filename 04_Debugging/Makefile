range: range.c
	gcc -O0 -g $< -o $@

test: range
	gdb -x 1.gdb ./$< --batch --quiet | grep "^@@@" | cut -c 4- > 1.out
	cmp 1.out 1.out.expected
	gdb -x 2.gdb ./$< --batch --quiet | grep "^@@@" | cut -c 4- > 2.out
	cmp 2.out 2.out.expected

clean:
	rm -f range *.out