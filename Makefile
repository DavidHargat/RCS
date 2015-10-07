
HEADERS =\
src/char.h \
src/file.h \
src/list.h \
src/parse.h \
src/tree.h \
src/token.h \
src/linear_interpreter.h

OBJECTS=\
build/main.o \
build/char.o \
build/file.o \
build/list.o \
build/parse.o \
build/tree.o \
build/token.o \
build/linear_interpreter.o

.PHONY: all
all: build/rcs

build/%.o: src/%.c $(HEADERS)
	gcc -c -o $@ $<

build/rcs: $(OBJECTS)
	gcc -o $@ $^
