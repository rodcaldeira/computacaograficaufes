CODIGO = $(wildcard *.c)

all:
	g++ main.cpp tinyxml2/tinyxml2.cpp -o trabalhocg -lGL -lGLU -lglut

trab1: $(CODIGO:.c=.o)
	gcc -lm -o $@ $^

clean:
	-rm -f *.o trabalhocg *~

remade:
	$(MAKE) clean
	$(MAKE)
