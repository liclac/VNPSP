TARGET = vnpsp
OBJS   = src/main.o src/App.o src/Scene.o src/Novel.o \
			src/MainMenuScene.o

INCDIR   = lib
CFLAGS   = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS  = $(CFLAGS)

LIBDIR  = 
LDFLAGS = 
LIBS    = -lm -lstdc++ \
			-Llib/oslib -losl \
			-lpng -ljpeg  -lz \
			-lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
			-lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lpspaudio -lpspgu -lpsppower -lpsphprm

BUILD_PRX = 1

EXTRA_TARGETS   = EBOOT.PBP
PSP_EBOOT_TITLE = VNPSP
PSP_EBOOT_ICON = ICON0.png
//PSP_EBOOT_PIC1 = PIC1.png
//PSP_EBOOT_SND0 = SND0.at3

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
