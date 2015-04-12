CC = gcc
LD = $(CC)

CFLAGS = -std=gnu99 -Wall -pipe -march=native
OFLAGS =
LFLAGS = -lm -lc -lcl -lpthread

OPTIMIZATION = -Ofast

CFLAGS += $(OPTIMIZATION)

all: mcpi mcpi_t mcpi_cl

mcpi: mcpi.o
	$(LD) $< $(LFLAGS) -o mcpi

mcpi_t: mcpi_t.o
	$(LD) $< $(LFLAGS) -o mcpi_t

mcpi_cl: mcpi_cl.o
	$(LD) $< $(LFLAGS) -o mcpi_cl

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf mcpi mcpi_t  gmon.out *.save *.o core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
