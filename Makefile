##***************************************************************************
##    Original code written in: Oct 2001    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
# Makefile

CC = gcc
CC_MPI = mpicc
CFLAGS = -g -Wall -I/usr/include/x86_64-linux-gnu/mpi $D $$(sdl-config --cflags) -c
LDFLAGS = -g -Wall $D $$(sdl-config --libs) -lm

OBJS = \
    src/hashtable.o \
    src/dlist.o \
    src/heap.o \
    src/list.o \
    src/waypoint.o \
    src/node.o \
    src/nodetable.o \
    src/path.o \
    src/primitives.o \
    src/timer.o \
    src/world.o \
    src/astar.o \

SINGLE_PROC_OBJS = \
    src/main.o \

MULTI_PROC_OBJS = \
    src/astar-mpi.o \
    src/comm-mpi.o \
    src/main-mpi.o \


all: pathplanning pathplanning-mpi genmaze



##
## Common part
##
timer.o: timer.c timer.h
	$(CC) $(CFLAGS) timer.c

list.o: list.c list.h
	$(CC) $(CFLAGS) list.c

dlist.o: dlist.c dlist.h
	$(CC) $(CFLAGS) dlist.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) heap.c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) hashtable.c

primitives.o: primitives.c primitives.h
	$(CC) $(CFLAGS) primitives.c

node.o: node.c node.h
	$(CC) $(CFLAGS) node.c

nodetable.o: nodetable.c nodetable.h
	$(CC) $(CFLAGS) nodetable.c

path.o: path.c path.h
	$(CC) $(CFLAGS) path.c

waypoint.o: waypoint.c waypoint.h
	$(CC) $(CFLAGS) waypoint.c

astar.o: astar.c astar.h
	$(CC) $(CFLAGS) astar.c



##
## Single process part
##
world.o: world.c world.h
	$(CC) $(CFLAGS) world.c

main.o: main.c main.h
	$(CC) $(CFLAGS) main.c

pathplanning: $(OBJS) $(SINGLE_PROC_OBJS)
	$(CC) -o pathplanning $(LDFLAGS) $(OBJS) $(SINGLE_PROC_OBJS)  $$(sdl-config --libs) -lm



##
## Multi Process part
##
astar-mpi.o: astar-mpi.c astar-mpi.h
	$(CC_MPI) $(CFLAGS) astar-mpi.c

comm-mpi.o: comm-mpi.c comm-mpi.h
	$(CC_MPI) $(CFLAGS) comm-mpi.c

world-mpi.o: world-mpi.c world-mpi.h
	$(CC_MPI) $(CFLAGS) world-mpi.c

planpath-mpi.o: planpath-mpi.c planpath-mpi.h
	$(CC_MPI) $(CFLAGS) planpath-mpi.c

main-mpi.o: main-mpi.c main-mpi.h
	$(CC_MPI) $(CFLAGS) main-mpi.c

pathplanning-mpi: $(OBJS) $(MULTI_PROC_OBJS)
	$(CC_MPI) -o pathplanning-mpi $(LDFLAGS) $(OBJS) $(MULTI_PROC_OBJS)  $$(sdl-config --libs) -lm



##
## Genmaze
##
genmaze: src/genmaze.c src/genmaze.h
	$(CC) $(LDFLAGS) src/genmaze.c -o genmaze



clean:
	rm -f src/*.o pathplanning pathplanning-mpi genmaze
	rm -f core
