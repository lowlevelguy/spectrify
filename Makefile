CC=gcc
SRCDIR=src
CFLAGS=-I$(SRCDIR) -g -DDEBUG_MODEc -Wall -Werror -Wpedantic -fsanitize=address -static-libasan
LDFLAGS=-lfftw3 -lm

ODIR=obj
_OBJ=main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS=
DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spectrify: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*/*.o *~ core
