SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
BINS = prog prog-a prog-so
LIB_OBJECTS = fun.o const.o
CFLAGS = -Wall -Wextra -O2 -fPIC
STATIC_LIB = output_static
DYNAMIC_LIB = output

all: libs bin

# header dependencies

fun.o prog.o: outlib.h

# building program

bin: $(BINS)

prog: $(OBJECTS)

prog-a: prog.o lib$(STATIC_LIB).a
	$(CC) prog.o -o $@ $(CFLAGS) -l$(STATIC_LIB) -L.

prog-so: prog.o lib$(DYNAMIC_LIB).so
	$(CC) prog.o -o $@ $(CFLAGS) -l$(DYNAMIC_LIB) -L.

# building libs

libs: lib$(STATIC_LIB).a lib$(DYNAMIC_LIB).so

lib$(STATIC_LIB).a: $(LIB_OBJECTS)
	ar -rcs lib$(STATIC_LIB).a $(LIB_OBJECTS)

lib$(DYNAMIC_LIB).so: $(LIB_OBJECTS)
	$(CC) -shared $(LIB_OBJECTS) -o lib$(DYNAMIC_LIB).so

# cleaning

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(BINS) lib$(STATIC_LIB).a lib$(DYNAMIC_LIB).so
	@# just in case something happened during last testing:
	rm -f $(wildcard prog*.txt)

# testing:
ARGS=

test: bin lib$(DYNAMIC_LIB).so
	@make run_test -s -e
	@ARGS=1 make run_test -s -e
	@make run_test -s -e "ARGS=-10 230 42"

run_test: bin lib$(DYNAMIC_LIB).so
	echo "Test for arguments: $(ARGS)"
	-./prog $(ARGS) > prog_out.txt 2> prog_err.txt
	-./prog-a $(ARGS) > prog-a_out.txt 2> prog-a_err.txt
	-LD_LIBRARY_PATH="." ./prog-so $(ARGS) > prog-so_out.txt 2> prog-so_err.txt
	cmp prog_out.txt prog-a_out.txt
	cmp prog_out.txt prog-so_out.txt
	# err-files comparison is not very useful: program names differ
	-cmp prog_err.txt prog-a_err.txt
	-cmp prog_err.txt prog-so_err.txt
	rm -f $(wildcard prog*.txt)
