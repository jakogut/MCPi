CC = gcc
LD = $(CC)

CFLAGS = -std=gnu99 -Wall -pipe -march=native -fomit-frame-pointer -funroll-loops 
OFLAGS = 
LFLAGS = -lm -lc -lpthread

OPTIMIZATION = -O3

CFLAGS += $(OPTIMIZATION)

all: mcpi mcpi_t

mcpi: mcpi.o
	$(LD) $< $(LFLAGS) -o mcpi

mcpi_t: mcpi_t.o
	$(LD) $< $(LFLAGS) -o mcpi_t


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf mcpi mcpi_t  gmon.out *.save *.o core* vgcore*

rebuild: clean all

.PHONY : clean
.SILENT : clean
