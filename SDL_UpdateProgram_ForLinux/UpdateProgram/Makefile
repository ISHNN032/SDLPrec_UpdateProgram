#Copyright Notice
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.
SYSROOT = /mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/

#OBJS specifies which files to compile as part of the project
OBJS =  Global.cpp \
        LTexture.cpp \
        Main.cpp \
        UpdateManager.cpp \
        MainActivity.cpp
       

#CC specifies which compiler we're using
CC = /mnt/user/ISHNN/DEV_KLAK/external-toolchain/bin/arm-linux-gnueabi-g++


#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -pipe -Os -mtune=cortex-a9 -march=armv7-a -mabi=aapcs-linux -msoft-float -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -lpthread --sysroot=$(SYSROOT) -std=c++0x

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -L/mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/usr/lib
LINKER_FLAGS += -L/mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/lib/arm-linux-gnueabi \
    -L/mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/lib/ \
    -L/mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/lib/ \
    -L/mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/usr/lib/ \
    -L/mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/usr/lib/arm-linux-gnueabi/

INCLUDE = -I/mnt/user/ISHNN/DEV_KLAK/host/arm-buildroot-linux-gnueabi/sysroot/usr/include/SDL2/
INCLUDE += -I/mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/usr/include/
INCLUDE += -I/mnt/user/ISHNN/DEV_KLAK/external-toolchain/arm-linux-gnueabi/libc/usr/include/arm-linux-gnueabi/

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Test.out

#This is the target that compiles our executable
all:FORCE
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(INCLUDE) $(LINKER_FLAGS) -o $(OBJ_NAME) 
FORCE:
