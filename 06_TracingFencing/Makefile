GENERATES=move *.txt *.copy *.so
SRC=move.c


all: $(SRC) myUnlink.so input.txt
	cc $< -g -Wall -Wextra -o move

myUnlink.so:	myUnlink.c
	cc -shared -fPIC -g -Wall -Wextra -o $@ $< -ldl

input.txt:
	cat $(SRC) > $@
	cp $@ PROTECT_$@

test:	tests.sh
	./tests.sh

clean:
	rm -rf $(GENERATES)