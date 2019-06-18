VER = 1.0

CC     = mpicc
CFLAGS = -O3 -Wall -fopenmp 
RM     = rm -rf
TAR    = tar -czvf

SRCDIR = src/
INCDIR = include/
BLDDIR = build/
BINDIR = bin/
DEPS = $(INCDIR)game-of-life.h
SRCS = main.c helpers.c io_functions.c play.c init.c
OBJS = $(patsubst %.c,$(BLDDIR)%.o,$(SRCS))
EXE  = $(BINDIR)game-of-life
GZOUT= game-of-life-v$(VER).tar.gz

#############################
# Compilation-linking rules #
# $@ left side
# $^ right side 
# $< first item in deps
#############################

CFLAGS += -I $(INCDIR)

$(BLDDIR)%.o: $(SRCDIR)%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< -D PRINT

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean-output:
	$(RM) $(SRCDIR)*~ $(BLDDIR)*.o $(INCDIR)*~ $(GZOUT)

compress:
	$(TAR) $(GZOUT) $(SRCDIR) $(INCDIR) $(BLDDIR) $(BINDIR) Makefile

.PHONY : clean
clean: clean-output
	$(RM) $(EXE) 

