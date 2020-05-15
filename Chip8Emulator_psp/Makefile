TARGET = Chip8

OBJS = main.o chip8.o Graphics.o Input.o common.o

# USE_LOCAL_HEADERS=0

INCDIR = ../include
CFLAGS = -G0 -Wall -gdwarf-2
CXXFLAGS = $(CFLAGS) -O2 -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../lib
LDFLAGS = 
LIBS= -lSDL2 -lGL -lGLU -lglut -lz\
         -lstdc++ -lpsprtc -lpspsdk -lpspvfpu -lpsphprm -lpspctrl -lpspumd -lpsppower -lpspgum -lpspgu -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -lpspvram


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Chip8
PSP_FW_VERSION=500

PSP_LARGE_MEMORY = 0
# BUILD_PRX = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak