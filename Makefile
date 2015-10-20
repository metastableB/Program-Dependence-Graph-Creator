# MakeFile PDG
# @author: metastableB

# Variable declarations
CC=clang++
CFLAGS=-std=c++11 -Wall
ifeq ($(debug), 1)   
	CFLAGS += -g -D_DEBUG_
endif

# Define all source definitions here
FILES=./init.cpp \
	  ./pdg.cpp

# Source Targets 
all :
	$(CC) $(CFLAGS) $(FILES) -o ./pdg_cs17

clean :
	rm -vf  ./*.out   ./pdg_cs17   ./*.o   ./*.h.gch  ./*.gch
