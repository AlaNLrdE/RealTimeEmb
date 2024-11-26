INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O3 $(INCLUDE_DIRS) $(CDEFS) -Wall
LIBS= 

HFILES= logger.h
CFILES= pthread.c logger.c

SRCS= ${HFILES} ${CFILES}
OBJS= $(patsubst %.c,build/%.o,$(CFILES))

all: build_dir pthread

build_dir:
	mkdir -p build

clean:
	-rm -f build/*.o build/*.d
	-rm -f build/pthread

distclean: clean
	-rm -rf build

pthread: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) -lpthread

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<