EXEC := server

SRCDIR := Server
BUILD  := Build

BASE_CCFLAGS := -Wall -Wextra -pedantic -std=c++11 -g
BASE_LDFLAGS :=

LINUX_CC := g++
LINUX_LD := g++
LINUX_CCFLAGS := $(BASE_CCFLAGS) -D__LINUX__
LINUX_LDFLAGS := $(BASE_LDFLAGS) -lpthread

WIN32_CC := x86_64-w64-mingw32-g++
WIN32_CC := x86_64-w64-mingw32-g++
WIN32_CCFLAGS := $(BASE_CCFLAGS) -D__WIN32__ -D_WIN32_WINNT=0x0601
WIN32_LDFLAGS := $(BASE_LDFLAGS) -lws2_32

ifndef $(TARGET)
	ifeq ($(TARGET), linux)
		CC := $(LINUX_CC)
		LD := $(LINUX_LD)
		CCFLAGS := $(LINUX_CCFLAGS)
		LDFLAGS := $(LINUX_LDFLAGS)
	else ifeq ($(TARGET), win32)
		CC :=  $(WIN32_CC)
		LD := $(WIN32_LD)
		CCFLAGS := $(WIN32_CCFLAGS)
		LDFLAGS := $(WIN32_LDFLAGS)
	else
		CC := $(LINUX_CC)
		LD := $(LINUX_LD)
		CCFLAGS := $(LINUX_CCFLAGS)
		LDFLAGS := $(LINUX_LDFLAGS)
	endif
endif

OBJECTS := $(BUILD)/Main.o \
           $(BUILD)/Client.o \
           $(BUILD)/MessageTypes.o \
           $(BUILD)/Server.o \
           $(BUILD)/Utils.o \
           $(BUILD)/Database.o \
           $(BUILD)/User.o \
           $(BUILD)/Message.o

all: dirs $(OBJECTS)
	$(CC) -o $(BUILD)/$(EXEC) $(OBJECTS) $(LDFLAGS)

$(BUILD)/Main.o: $(SRCDIR)/Main.cpp
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Client.o: $(SRCDIR)/Client.cpp \
                   $(SRCDIR)/Client.h \
                   $(SRCDIR)/MessageTypes.h \
                   $(SRCDIR)/Utils.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/MessageTypes.o: $(SRCDIR)/MessageTypes.cpp \
                         $(SRCDIR)/MessageTypes.h \
                         $(SRCDIR)/Utils.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Server.o: $(SRCDIR)/Server.cpp \
                   $(SRCDIR)/Server.h \
                   $(SRCDIR)/Utils.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Utils.o: $(SRCDIR)/Utils.cpp \
                  $(SRCDIR)/Utils.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Database.o: $(SRCDIR)/Database.cpp \
                     $(SRCDIR)/Database.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/User.o: $(SRCDIR)/User.cpp \
                 $(SRCDIR)/User.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message.o: $(SRCDIR)/Message.cpp \
                    $(SRCDIR)/Message.h
	$(CC) -o $@ -c $< $(CCFLAGS)

dirs:
	mkdir -p $(BUILD)

clean:
	rm -vf $(OBJECTS)
	rm -vf $(BUILD)/$(EXEC)

mrproper:
	rm -rf $(BUILD)
