CC = gcc

RM-F := rm

SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)

des: $(OBJ)
	$(CC) $^ -o $@  

# $^ : the target
# $@ : the dependencies
# $< : the first elements of the dependencies;

.PHONY: clean
clean:
	@$(RM-F) des.exe $(OBJ) ./encoded/* ./decoded/*