FILES=\
./src/main.c \
./src/char.c \
./src/file.c \
./src/list.c \
./src/parse.c \
./src/tree.c \
./src/token.c \
./src/linear_interpreter.c \
./src/char.h \
./src/file.h \
./src/list.h \
./src/parse.h \
./src/tree.h \
./src/token.h \
./src/linear_interpreter.h \

make:
	gcc $(FILES) -o ./build/rcs