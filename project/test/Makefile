#Architecture
ARCH := $(shell uname -m)

#Compilers
CC          := g++ -std=c++14

#The Target Library

#The Directories, Source, Includes, Objects, Binary and Resources
SRCEXT      := cc

#Asan library and RPi Def
ifeq ($(ARCH),armv7l)
        ASAN := -static-libasan
        RPI  := -D_RPI_
else
        ASAN := -lasan
        RPI  := -D_NOTRPI_
endif

#Flags, Libraries and Includes
CFLAGS      := -ggdb $(RPI)
# LIB         := -lpthread -lasan -lelma
LIB         := -lpthread -lelma -lgtest
INCLUDE		:= -I..
LIBDIR		:= -L../../elma_port/lib
ELMALIB		:= ../../elma_port/lib/libelma.a

#Files
TARGETDIR	 := ./bin
SOURCES      := $(wildcard *.cc) # ../servo.cc ../UDPCommunications.cc
TARGETS		 := $(patsubst %.cc,%,$(wildcard *.cc))
FULL_TARGETS := $(addprefix $(TARGETDIR)/, $(TARGETS))

#Default Make
all: dirs $(FULL_TARGETS)

dirs: $(TARGETDIR)
	@mkdir -p $(TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(TARGETDIR)

# Compile
$(TARGETDIR)/%: %.cc $(ELMALIB)
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBDIR) $(LIB) -o $@

# bin/basic: basic.cc
# 	$(CC) $(CFLAGS) $(INCLUDE) $<  $(LIBDIR) $(LIB) -o $@

.PHONY: directories remake clean cleaner $(BUILDDIR) $(TARGETDIR)
