#############################################################################
# CASPIAN - A Programmable Neuromorphic Computing Architecture
# Parker Mitchell, 2019
# Simon D. Levy, 2026
#############################################################################

# Directories
INC = ../../include
SRC = ../../src
STATIC_OBJ=../../obj_static
STATIC_SRC=../../src_static

# Framework Directories
ROOT          = ../../../..
ROOT_INCLUDE  = $(ROOT)/include

# Compilation Flags
CFLAGS ?= -Wall -Wextra -pipe -O3 -g
CFLAGSBASE = $(CFLAGS) -std=c++14 -I$(INC) -I$(ROOT_INCLUDE) -DWITH_USB
LFLAGS = -lpthread -lftdi1

#########################
## Caspian library
LIBCASPIAN = lib/libcaspian.a
LIBFRAMEWORK = $(ROOT)/lib/libframework.a
LIBRARY=lib/libproc2.a
STATIC_LIB=lib/libproc1.a

$(LIBFRAMEWORK): $(ROOT_INCLUDE)/framework.hpp library
	$(MAKE) -C $(ROOT)

library: $(LIBRARY) $(STATIC_LIB)

#########################
## Directories
bin obj lib:
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

OBJECTS    := $(patsubst $(SRC)/%.cpp,obj/%.o,$(SOURCES))
TL_OBJECTS := $(patsubst $(SRC)/%.cpp,obj/%.o,$(TL_SOURCES))

$(OBJECTS): obj/%.o : $(SRC)/%.cpp $(HEADERS) | obj
	g++ $(CFLAGSBASE) -c $< -o $@

$(TL_OBJECTS): obj/%.o : $(SRC)/%.cpp $(HEADERS) $(TL_HEADERS) \
	$(ROOT_INCLUDE)/framework.hpp | obj
	g++ $(CFLAGSBASE) -c $< -o $@

$(LIBCASPIAN): $(OBJECTS) $(TL_OBJECTS) $(V_OBJECTS) | lib
	ar r $@ $^
	ranlib $@

$(LIBRARY): obj/network.o obj/network_conversion.o obj/processor.o obj/simulator.o
	ar r $(LIBRARY) obj/network.o obj/network_conversion.o \
		obj/processor.o obj/simulator.o
	ranlib $(LIBRARY)

$(STATIC_LIB): $(STATIC_OBJ)/static_proc.o
	ar r $(STATIC_LIB) $(STATIC_OBJ)/static_proc.o
	ranlib $(STATIC_LIB)

$(STATIC_OBJ)/static_proc.o: $(STATIC_SRC)/static_proc.cpp
	g++ $(CFLAGSBASE) -c $(STATIC_SRC)/static_proc.cpp -o $@

bin/json_example : src/json_example.cpp $(LIBCASPIAN) $(LIBFRAMEWORK) | bin
	g++ $(CFLAGSBASE) $< -o $@ $(LIBCASPIAN) $(LIBFRAMEWORK) $(LFLAGS)

clean:
	rm -rf bin obj lib

