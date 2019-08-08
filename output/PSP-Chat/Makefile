TARGET = psp-chat
OBJS = ../../src/PSP-Chat/main.o ../../src/PSP-Chat/common.o ../../src/PSP-Chat/intrafont/intraFont.o ../../src/PSP-Chat/intrafont/libccc.o ../../src/PSP-Chat/intrafont/glib2d.o ../../src/PSP-Chat/valloc.o
INCDIR = $(PSPPATH)/include include include/archive include/menus
CFLAGS = -O3 -Wall -mpreferred-stack-boundary=4
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371
PSP_LARGE_MEMORY = 1

LDFLAGS =
STDLIBS = -losl -lpng -lz \
          -lpsphprm -lpspsdk -lpspctrl -lpsprtc -lpsppower -lpspgu -lpspgum -lpsphttp -lpspssl -lpspwlan \
          -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg \
          -lpspusb -lpspusbstor \
          -lpspmp3 -lmad -lpspaudiolib -lpspaudio -lpspaudiocodec -lmikmod -lstdc++ -lmmio -lpspvfpu \
		  -lpspvram -lpspreg -lpspumd
LIBS    = $(STDLIBS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_TITLE = PSP-Chat

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
