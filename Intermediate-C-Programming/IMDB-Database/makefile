#OBJS specifies which files to compile as part of the project
OBJS = main.c

#CC specifies which compiler we're using 
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -std=c99 -I/opt/local/include

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lm -lsqlite3 -L/opt/local/lib 

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = project06_01

#This is the target that compiles our executable 
all : $(OBJS) 
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#Clean
.PHONY: clean

clean:
	rm -f *.o *~ project06_01
