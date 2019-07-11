CROSS_COMPILE := mipsel-linux-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++

SYSROOT     := $(shell $(CC) --print-sysroot)
SDL_CFLAGS  := $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS    := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)

DEFINES = -DBYPASS_PROTECTION

LDFLAGS := -lSDL_mixer
CXXFLAGS := -g -O -MMD -Wall -Wpedantic $(SDL_CFLAGS) $(DEFINES)

SRCS = aifcplayer.cpp bitmap.cpp file.cpp engine.cpp graphics_soft.cpp \
	script.cpp mixer.cpp pak.cpp resource.cpp resource_mac.cpp resource_nth.cpp \
	resource_win31.cpp resource_3do.cpp scaler.cpp screenshot.cpp systemstub_sdl1.cpp sfxplayer.cpp \
	staticres.cpp unpack.cpp util.cpp video.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

rawgl: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(SDL_LIBS) -lz

clean:
	rm -f $(OBJS) $(DEPS)

-include $(DEPS)
