
HEADERS =\
src/char.h \
src/file.h \
src/list.h \
src/parse.h \
src/token.h \
src/inter.h

OBJECTS=\
build/main.o \
build/char.o \
build/file.o \
build/list.o \
build/parse.o \
build/token.o \
build/inter.o

.PHONY: all
all: build/rcs

build/%.o: src/%.c $(HEADERS)
	gcc -c -o $@ $<

build/rcs: $(OBJECTS)
	gcc -o $@ $^
