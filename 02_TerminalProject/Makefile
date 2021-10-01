OUT = Show
SRC = Show.c
CFLAGS = -Wall -Wextra -O2
LIBS = -lncursesw

default: run

$(OUT): $(SRC)
	gcc $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

run: $(OUT) text.txt
	./$(OUT) text.txt

clean:
	rm -f *.o ./$(OUT)
