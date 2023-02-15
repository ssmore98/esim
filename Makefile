CC = g++
C_FILES = sim.c
O_FILES = $(C_FILES:.c=.o)

default: esim

%.o: %.c Makefile
	${CC} ${CFLAGS} -c $<

esim: $(O_FILES)
	$(CC) $(O_FILES) -o $@ -lyaml 
