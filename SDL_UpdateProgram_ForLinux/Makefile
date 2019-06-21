#Copyright Notice
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

#OBJS specifies which files to compile as part of the project
OBJS = Main.cpp

#CC specifies which compiler we're using
CC = /mnt/user/ISHNN/DEV_KLAK/external-toolchain/bin/arm-linux-gnueabi-g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -L/mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/usr/lib
INC = /mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/usr/include
INC += /mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/lib/arm-linux-gnueabi

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Update.out

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) $(INC)
