#
# Makefile
#

# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -g

# name for executable
EXE = gbemu

# space-separated list of header files
HDRS = gbemu.h cpu.h opcodes.h video.h utils.h input.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lSDL2

# space-separated list of source files
SRCS = gbemu.c cpu.c opcodes.c video.c utils.c input.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
