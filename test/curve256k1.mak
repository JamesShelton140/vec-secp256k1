#ASM specifies which assembler we're using
ASM = as

#CC specifies which compiler we're using
CC = gcc

#LL specifies which linker we're using
LL = gcc

#CFLAGS specifies the additional compilation options we're using
CFLAGS = -march=skylake -mtune=skylake -mavx2 -m64 -O3 -funroll-loops -fomit-frame-pointer

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lm

#include directory
INCDRS = -I../include/

#Source lists
CSRCS := $(wildcard ../source/*.c)
CBINS := $(CSRCS:../source/%.c=../bin/%.o)
SSRCS := $(wildcard ../source/*.s)
SBINS := $(SSRCS:../source/%.s=../bin/%.o)

#OBJFLS specifies which files to compile as part of the project
OBJFLS = ./curve256k1_test.o $(CBINS) $(SBINS)

#EXE specifies the target exe
EXE = curve256k1_test

#This is the target that compiles our executable
$(EXE): $(OBJFLS)
	$(LL) -o $@ $(OBJFLS) -lm -no-pie

../bin/%.o: ../source/%.c
	$(CC) $(INCDRS) $(CFLAGS) -o $@ -c $<

../bin/%.o: ../source/%.s
	$(ASM) -o $@ $<

./curve256k1_test.o: ./curve256k1_test.c ./config.h
	$(CC) $(INCDRS) $(CFLAGS) -w -o $@ -c $<
	
clean:
	-rm $(EXE)
	-rm $(OBJFLS)
