#OBJS specifies which files to compile as part of the project
OBJS = main.c functions.c

#CC specifies which compiler we're using 
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -std=c99

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lm

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = project01_01

#This is the target that compiles our executable 
all : $(OBJS) 
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#Clean
.PHONY: clean

clean:
	rm -f *.o *~ project01_01
