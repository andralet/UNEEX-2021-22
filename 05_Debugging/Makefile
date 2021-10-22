CFLAGS = -O0 -g

all: range

clean:
	rm ./range # That was easy:)

test: range
	@if gdb ./range -ex="source test1.py" --quiet --batch | grep @@@ | cmp - test1.out -s; \
		then echo "Test1... OK"; \
		else echo "Test1... FAULT"; \
	fi
	@if gdb ./range -ex="source test2.py" --quiet --batch | grep @@@ | cmp - test2.out -s; \
		then echo "Test2... OK"; \
		else echo "Test2... FAULT"; \
	fi
