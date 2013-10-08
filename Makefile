CC = gcc
LD = $(CC)

CFLAGS = -std=gnu99 -Wall -pipe -march=native -fomit-frame-pointer -funroll-loops 
OFLAGS = 
LFLAGS = -lm -lc -lpthread

DEBUG = no
PROFILE = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
endif

ifeq ($(PROFILE), yes)
	CFLAGS += -g -pg -DPROFILE
endif

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
