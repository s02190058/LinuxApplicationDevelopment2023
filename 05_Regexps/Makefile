esub: esub.c
	gcc $< -o $@

test: esub
	@./$< '\+7 \(([0-9]{3})\) ([0-9]{3})-([0-9]{2})-([0-9]{2})' '8\1\2\3\4' '+7 (931) 418-20-34' > 1.out
	@echo -E '+7 (931) 418-20-34' | sed -E 's/\+7 \(([0-9]{3})\) ([0-9]{3})-([0-9]{2})-([0-9]{2})/8\1\2\3\4/' > 1.out.expected

	cmp 1.out 1.out.expected

	@./$< 'e(l+)o' 'e\1\\\\' 'Hello, World!\n' > 2.out
	@echo -E 'Hello, World!\n' | sed -E 's/e(l+)o/e\1\\\\/' > 2.out.expected

	cmp 2.out 2.out.expected

clean:
	rm -rf esub *.out *.out.expected