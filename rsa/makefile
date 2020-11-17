CC = g++

RM-F := rm

LIBDIR:=C:/boost_1_74_0/
INCLUDES:=. C:/boost_1_74_0/
SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)  

rsa: $(OBJ)
	$(CC) $^ -o $@  $(addprefix -L,$(LIBDIR))  $(addprefix -l,$(LIBS))

# $^ : the target
# $@ : the dependencies
# $< : the first elements of the dependencies;

.PHONY: clean
clean:
	@$(RM-F) *.exe $(OBJ)