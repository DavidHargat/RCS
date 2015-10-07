FILES=\
main.c \
char.c \
file.c \
list.c \
parse.c \
tree.c \
token.c \
linear_interpreter.c \
char.h \
file.h \
list.h \
parse.h \
tree.h \
token.h \
linear_interpreter.h \

all:
    gcc -o rcs $(FILES)