#############################################################################
# CASPIAN - A Programmable Neuromorphic Computing Architecture
# Parker Mitchell, 2019
#############################################################################

# Applications
CC ?= gcc
CXX ?= g++
AR ?= ar
RANLIB ?= ranlib

# Directories
BIN = bin
INC = ../../include
LIB = lib
OBJ = obj
SRC = ../../src
STATIC_OBJ=../../obj_static
STATIC_SRC=../../src_static
UTILS = utils

# Framework Directories
ROOT          = ../../../..
ROOT_INCLUDE  = $(ROOT)/include

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(dir $(MKFILE_PATH))

# Compilation Flags
CFLAGS ?= -Wall -Wextra -pipe -O3 -g
CFLAGSBASE = $(CFLAGS) -std=c++14 -I$(INC) -I$(ROOT_INCLUDE) -DWITH_USB
LFLAGS = -lpthread -lftdi1

## Targets
.PHONY: all clean test run_test python utils
all: $(LIB)/libcaspian.a library

clean:
	rm -rf $(BIN) $(OBJ) $(LIB)

#########################
## Caspian library
LIBCASPIAN = $(LIB)/libcaspian.a
LIBFRAMEWORK = $(ROOT)/lib/libframework.a
LIBRARY=$(LIB)/libproc2.a
STATIC_LIB=$(LIB)/libproc1.a

$(LIBFRAMEWORK): $(ROOT_INCLUDE)/framework.hpp library
	$(MAKE) -C $(ROOT)

library: $(LIBRARY) $(STATIC_LIB)

#########################
## Directories
$(BIN) $(OBJ) $(LIB):
	mkdir -p $@

#########################
## Sources
HEADERS     = $(INC)/backend.hpp \
              $(INC)/constants.hpp \
	      $(INC)/network.hpp \
	      $(INC)/simulator.hpp \
	      $(INC)/ucaspian.hpp

TL_HEADERS  = $(INC)/processor.hpp \
              $(INC)/network_conversion.hpp

SOURCES     = $(SRC)/network.cpp \
	      $(SRC)/simulator.cpp

TL_SOURCES  = $(SRC)/processor.cpp \
              $(SRC)/network_conversion.cpp

SOURCES += $(SRC)/ucaspian.cpp
LFLAGS += -lftdi1

OBJECTS    := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))
TL_OBJECTS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(TL_SOURCES))

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.cpp $(HEADERS) | $(OBJ)
	$(CXX) $(CFLAGSBASE) -c $< -o $@

$(TL_OBJECTS): $(OBJ)/%.o : $(SRC)/%.cpp $(HEADERS) $(TL_HEADERS) $(ROOT_INCLUDE)/framework.hpp | $(OBJ)
	$(CXX) $(CFLAGSBASE) -c $< -o $@

$(LIBCASPIAN): $(OBJECTS) $(TL_OBJECTS) $(V_OBJECTS) | $(LIB)
	$(AR) r $@ $^
	$(RANLIB) $@

$(LIBRARY): obj/network.o obj/network_conversion.o obj/processor.o obj/simulator.o
	ar r $(LIBRARY) obj/network.o obj/network_conversion.o obj/processor.o obj/simulator.o
	ranlib $(LIBRARY)

$(STATIC_LIB): $(STATIC_OBJ)/static_proc.o
	ar r $(STATIC_LIB) $(STATIC_OBJ)/static_proc.o
	ranlib $(STATIC_LIB)

$(STATIC_OBJ)/static_proc.o: $(STATIC_SRC)/static_proc.cpp
	$(CC) $(CFLAGSBASE) -c $(STATIC_SRC)/static_proc.cpp -o $@

## flto flag compiler version specific fix.
# '-flto=auto' introduced since GCC 10.1:
# * https://gcc.gnu.org/onlinedocs/gcc-9.5.0/gcc/Optimize-Options.html#Optimize-Options (no)
# * https://gcc.gnu.org/onlinedocs/gcc-10.1.0/gcc/Optimize-Options.html#Optimize-Options (yes)
# Since GCC 12.1, the abundance of a parameter produces a warning if compiling multiple targets.
FLTOFLAGS = 
ifeq ($(CXX), g++)
	CXXMAJORVERSION = $(shell g++ --version | grep ^g++ | sed 's/^.* //g' | sed 's/\..*//')
	ifeq ($(shell test $(CXXMAJORVERSION) -ge 10; echo $$?),0)
		FLTOFLAGS = -flto=auto
	endif
endif

#########################
## Utilities
UTILITIES  = $(BIN)/json_example

$(UTILITIES): $(BIN)/% : $(UTILS)/%.cpp $(LIBCASPIAN) $(LIBFRAMEWORK) | $(BIN)
	$(CXX) $(CFLAGSBASE) $< -o $@ $(LIBCASPIAN) $(LIBFRAMEWORK) $(LFLAGS)

utils: $(UTILITIES)
