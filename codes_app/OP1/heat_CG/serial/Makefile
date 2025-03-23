FLAGS=-O3 -Wall
RM=rm -f
CC=gcc

EXEC=htcg

all: $(EXEC)

$(EXEC):
	$(CC) $(FLAGS) $(EXEC).c -o $(EXEC) -lm -fopenmp

clean:
	$(RM) $(EXEC).o $(EXEC)
