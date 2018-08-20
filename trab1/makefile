CODIGO = $(wildcard *.c)
    
all: trab1
 
trab1: $(CODIGO:.c=.o)
	gcc -lm -o $@ $^ 
 
%.o: %.c
	gcc -g -lm -Wall -c $< -o $@
    
clean:
	-rm -f *.o trab1 *~

remade:
	$(MAKE) clean
	$(MAKE)
 
