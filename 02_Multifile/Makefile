RES_PREFIX = res
ZERO = zero
ONE = one
THREE = three

ONE_ARGS = one
THREE_ARGS = one two three

LIB_DEPS = const.o fun.o

.PHONY: all
all: prog

.PHONY: test
test: test_zero test_one test_three

.PHONY: test_zero
test_zero: prog prog-a prog-so
	@./prog > $(RES_PREFIX)_$(ZERO) 2>&1
	@./prog-a > $(RES_PREFIX)_$(ZERO)_a 2>&1
	@LD_LIBRARY_PATH=`pwd` ./prog-so > $(RES_PREFIX)_$(ZERO)_so 2>&1

	cmp $(RES_PREFIX)_$(ZERO) $(RES_PREFIX)_$(ZERO)_a
	cmp $(RES_PREFIX)_$(ZERO) $(RES_PREFIX)_$(ZERO)_so

	@rm $(RES_PREFIX)_$(ZERO) $(RES_PREFIX)_$(ZERO)_a $(RES_PREFIX)_$(ZERO)_so

.PHONY: test_one
test_one: prog prog-a prog-so
	@./prog $(ONE_ARGS) > $(RES_PREFIX)_$(ONE) 2>&1
	@./prog-a $(ONE_ARGS) > $(RES_PREFIX)_$(ONE)_a 2>&1
	@LD_LIBRARY_PATH=`pwd` ./prog-so $(ONE_ARGS) > $(RES_PREFIX)_$(ONE)_so 2>&1

	cmp $(RES_PREFIX)_$(ONE) $(RES_PREFIX)_$(ONE)_a
	cmp $(RES_PREFIX)_$(ONE) $(RES_PREFIX)_$(ONE)_so

	@rm $(RES_PREFIX)_$(ONE) $(RES_PREFIX)_$(ONE)_a $(RES_PREFIX)_$(ONE)_so

.PHONY: test_three
test_three: prog prog-a prog-so
	@./prog $(THREE_ARGS) > $(RES_PREFIX)_$(THREE) 2>&1
	@./prog-a $(THREE_ARGS) > $(RES_PREFIX)_$(THREE)_a 2>&1
	@LD_LIBRARY_PATH=`pwd` ./prog-so $(THREE_ARGS) > $(RES_PREFIX)_$(THREE)_so 2>&1

	cmp $(RES_PREFIX)_$(THREE) $(RES_PREFIX)_$(THREE)_a
	cmp $(RES_PREFIX)_$(THREE) $(RES_PREFIX)_$(THREE)_so

	@rm $(RES_PREFIX)_$(THREE) $(RES_PREFIX)_$(THREE)_a $(RES_PREFIX)_$(THREE)_so

prog: const.o fun.o prog.o
	cc $^ -o $@

prog-a: liboutput_static.a prog.o
	cc -L. prog.o -loutput_static -o $@

prog-so: liboutput.so prog.o
	cc -L. prog.o -loutput -o $@

liboutput_static.a: $(LIB_DEPS)
	ar -rcs $@ $^

liboutput.so: $(LIB_DEPS)
	cc -shared $^ -o $@

%.o: %.c
	cc -fPIC $< -c -o $@

fun.o: outlib.h

.PHONY: clean
clean:
	rm -f prog prog-a prog-so liboutput_static.a liboutput.so *.o