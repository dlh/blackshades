CC := gcc
CXX := g++
LINKER := g++
ASM := nasm
SRCDIR := ./Source
BINDIR := ./objs
EXE := $(BINDIR)/blackshades

CFLAGS := -g $(shell sdl-config --cflags) -I$(SRCDIR) -include Source/Support.h -DPOOLOOPS
CXXFLAGS := $(CFLAGS)
LDFLAGS := $(shell sdl-config --libs) -lGL -lGLU -lopenal # -framework QuickTime

SRCS := Camera.cpp \
        Decals.cpp \
        Fog.cpp \
        Frustum.cpp \
        GameDraw.cpp \
        Globals.cpp \
        Maths.cpp \
	Person.cpp \
        Quaternions.cpp \
	Skeleton.cpp \
        Sprites.cpp \
	Support.cpp \
        TGALoader.cpp \
        Text.cpp \
	Timer.cpp

# Files.cpp
# GameInitDispose.cpp
# GameLoop.cpp
# GameTick.cpp
# Main.cpp
# Models.cpp
# Person.cpp

OBJS1 := $(SRCS:.c=.o)
OBJS2 := $(OBJS1:.cpp=.o)
OBJS3 := $(OBJS2:.asm=.o)
OBJS := $(foreach f,$(OBJS3),$(BINDIR)/$(f))
SRCS := $(foreach f,$(SRCS),$(SRCDIR)/$(f))

CLEANUP = $(wildcard *.exe) $(wildcard *.obj) \
          $(wildcard $(BINDIR)/*.exe) $(wildcard $(BINDIR)/*.obj) \
          $(wildcard *~) $(wildcard *.err) \
          $(wildcard .\#*) core $(EXE)


# Rules for compiling individual source files...

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: all bindir blackshades
all: blackshades

blackshades: $(BINDIR) $(OBJS)
	$(LINKER) -o $(EXE) $(OBJS) $(LDFLAGS)

$(BINDIR) :
	$(MAKE) bindir

bindir:
	mkdir -p $(BINDIR)

distclean: clean

clean:
	rm -f $(CLEANUP)
	rm -rf $(BINDIR)

# end of Makefile.


