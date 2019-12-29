TARGET = psp-chat
OBJS = src/PSP-Chat/main.o \
src/PSP-Chat/Globals.o \
src/PSP-Chat/framework/utils/common.o \
src/PSP-Chat/framework/utils/Input.o \
src/PSP-Chat/framework/utils/MemoryUsage.o \
src/PSP-Chat/framework/utils/Timer.o \
src/PSP-Chat/framework/utils/valloc.o \
src/PSP-Chat/framework/gfx/intrafont/libccc.o \
src/PSP-Chat/framework/gfx/intrafont/intraFont.o \
src/PSP-Chat/framework/gfx/RenderUtil.o \
src/PSP-Chat/framework/gfx/intrafont/glib2d.o \
src/PSP-Chat/framework/state/StateManager.o \

INCDIR = $(PSPPATH)/include
INCDIR += include include/archive include/menus src/PSP-Chat/
CFLAGS = -O3 -G0 -mpreferred-stack-boundary=4
CXXFLAGS = -std=gnu++17
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371
PSP_LARGE_MEMORY = 0 #PHAT SUPPORT! Our app isn't that big! 

LDFLAGS =
# -losl -lmmio
STDLIBS = -lpng -lz \
          -lpsphprm -lpspsdk -lpspctrl -lpsprtc -lpsppower -lpspgum -lpspgu -lpsphttp -lpspssl -lpspwlan \
          -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg \
          -lpspusb -lpspusbstor \
          -lpspmp3 -lmad -lpspaudiolib -lpspaudio -lpspaudiocodec -lmikmod -lstdc++ -lpspvfpu \
		  -lpspvram -lpspreg -lpspumd
LIBS    = $(STDLIBS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_TITLE = PSP-Chat

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
