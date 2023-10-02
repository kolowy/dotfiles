# Makefile

CC = gcc
CPPFLAGS = 
CFLAGS = -Wall -Wextra -Werror -Wvla -std=c99 -pendatic
LDFLAGS = -fsanitize=address
LDLIBS =

SRC = 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

test: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} test

# END
