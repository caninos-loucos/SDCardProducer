
CPPFLAGS := `wx-config --cxxflags` 
LIBS     := `wx-config --libs std`
CPP      := g++
CURDIR   := $(shell pwd)
SRCDIR   := $(CURDIR)/src
RESDIR   := $(CURDIR)/resources
LINUXDIR := $(CURDIR)/linux

.PHONY: clean all

all: clean
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/myframe.cpp -o $(LINUXDIR)/myframe.o
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/config.cpp -o $(LINUXDIR)/config.o
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/imgfile.cpp -o $(LINUXDIR)/imgfile.o
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/main.cpp -o $(LINUXDIR)/main.o
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/about.cpp -o $(LINUXDIR)/about.o
	$(Q)$(CPP) $(CPPFLAGS) -c $(SRCDIR)/info.cpp -o $(LINUXDIR)/info.o
	$(Q)$(CPP) $(LINUXDIR)/main.o $(LINUXDIR)/info.o $(LINUXDIR)/about.o $(LINUXDIR)/config.o $(LINUXDIR)/imgfile.o $(LINUXDIR)/myframe.o -o $(LINUXDIR)/cardproducer $(LIBS)

clean:
	$(Q)rm -rf $(LINUXDIR)
	$(Q)mkdir -p $(LINUXDIR)

