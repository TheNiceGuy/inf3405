EXEC := server

SRCDIR := Server
BUILD  := Build

BASE_CCFLAGS := -Wall -Wextra -pedantic -std=c++11 -g -I./$(SRCDIR) -fpermissive
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
           $(BUILD)/Server.o \
           $(BUILD)/Utils.o \
           $(BUILD)/Database.o \
           $(BUILD)/User.o \
           $(BUILD)/Message/Auth.o \
           $(BUILD)/Message/Quit.o \
           $(BUILD)/Message/Status.o \
           $(BUILD)/Message/ServerText.o \
           $(BUILD)/Message/ClientText.o \
           $(BUILD)/Message/SerializableObject.o

all: dirs $(OBJECTS)
	$(CC) -o $(BUILD)/$(EXEC) $(OBJECTS) $(LDFLAGS)

$(BUILD)/Main.o: $(SRCDIR)/Main.cpp
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Client.o: $(SRCDIR)/Client.cpp \
                   $(SRCDIR)/Client.h \
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

$(BUILD)/Message/Auth.o: $(SRCDIR)/Message/Auth.cpp \
                         $(SRCDIR)/Message/Auth.h \
                         $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message/Quit.o: $(SRCDIR)/Message/Quit.cpp \
                         $(SRCDIR)/Message/Quit.h \
                         $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message/Status.o: $(SRCDIR)/Message/Status.cpp \
                           $(SRCDIR)/Message/Status.h \
                           $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message/ServerText.o: $(SRCDIR)/Message/ServerText.cpp \
                               $(SRCDIR)/Message/ServerText.h \
                               $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message/ClientText.o: $(SRCDIR)/Message/ClientText.cpp \
                               $(SRCDIR)/Message/ClientText.h \
                               $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

$(BUILD)/Message/SerializableObject.o: $(SRCDIR)/Message/SerializableObject.cpp \
                                       $(SRCDIR)/Message/SerializableObject.h
	$(CC) -o $@ -c $< $(CCFLAGS)

dirs:
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/Message

clean:
	rm -vf $(OBJECTS)
	rm -vf $(BUILD)/$(EXEC)

mrproper:
	rm -rf $(BUILD)
