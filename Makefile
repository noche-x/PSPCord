TARGET = psp-chat
OBJS = src/PSP-Chat/main.o \
# src/PSP-Chat/framework/utils/common.o \
# src/framework/utils/console_logger.o \
# src/PSP-Chat/framework/gfx/RenderUtil.o \
# src/PSP-Chat/framework/gfx/intrafont/glib2d.o \
# src/PSP-Chat/framework/utils/Input.o \
# src/PSP-Chat/framework/utils/MemoryUsage.o \
# src/PSP-Chat/framework/utils/Timer.o \
# src/PSP-Chat/framework/utils/valloc.o \
# src/PSP-Chat/framework/gfx/intrafont/libccc.o \
# src/PSP-Chat/framework/gfx/intrafont/intraFont.o \
# src/PSP-Chat/framework/connection/ConnectionManager.o \

INCDIR = $(PSPPATH)/include lib/Stardust-Engine/include lib/Stardust-Engine/libs/include #/usr/local/pspdev/psp/sdk/include
#INCDIR += include include/archive include/menus src/PSP-Chat/

CFLAGS = -O3 -g3 -mpreferred-stack-boundary=4 -ffast-math -fno-rtti -fno-exceptions 
CXXFLAGS = -std=gnu++17
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=500
PSP_LARGE_MEMORY = 1 #PHAT SUPPORT! Our app isn't that big! 

LDFLAGS =
# -losl -lmmio -lpng

LIBDIR = lib/Stardust-Engine/lib lib/Stardust-Engine/libs/lib
LIBS    = -lstardust -ljson -lintraFont -lpng -lpspmath -lpspwlan -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lpspusb -lpspusbstor -lpspmp3 -lmad -lpspvram -lpspreg -lpspumd -lpsphprm -lpspaudiolib -lpspaudio -lmikmod -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp -ljpeg -lpspssl
# LIBS = -lpspaudiolib -lpspaudio -lmikmod -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_TITLE = PSP-Chat

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
