#Architecture
ARCH := $(shell uname -m)

#cairo
CAIRO_BLD = `pkg-config --cflags --libs gtk+-3.0`

#Compilers
CC          := g++ -std=c++14
DGEN        := doxygen

#The Target Binary Program
TARGET      := simulator

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := .
INCDIR      := ..
BUILDDIR    := ./build
TARGETDIR   := ./bin
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
LIB         := -lpthread -lelma -lgtest -lwiringPi
INC         := -I$(INCDIR) -I$(INCDIR)/elma_port
INCDEP      := -I$(INCDIR)
LIBDIR		:= -L../elma_port/lib
ELMALIB		:= ../elma_port/lib/libelma.a

#Files
DGENCONFIG  := docs.config
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.cc)
OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))
PNGFILES    := $(wildcard *.png)

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET)

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)
	cp $(PNGFILES) $(TARGETDIR)

# Make the documentation
$(DGENCONFIG):
	$(DGEN) -g $(DGENCONFIG)

docs: $(SOURCES) $(HEADERS) $(DGENCONFIG)
	$(DGEN) $(DGENCONFIG)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TARGETDIR)/$(TARGET) $(DGENCONFIG) *.db
	@$(RM) -rf build bin html latex

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) $(LIB) -o $(TARGETDIR)/$(TARGET) $^ $(LIB) $(CAIRO_BLD)

#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS) $(ELMALIB)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(CAIRO_BLD)

#	$(TARGETDIR)/%: %.cc $(ELMALIB)
#	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBDIR) $(LIB) -o $@

.PHONY: directories remake clean cleaner apidocs $(BUILDDIR) $(TARGETDIR)
