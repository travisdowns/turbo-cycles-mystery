.PHONY: all clean libpfc libpfc-clean

HEADERS := $(wildcard *.h) $(wildcard *.hpp) $(wildcard *.hxx)

CPPFLAGS := -Wall -g -O2 -march=native

OBJECTS := main.o

LDFLAGS += -Llibpfc '-Wl,-rpath=$$ORIGIN/libpfc/'
LDLIBS += -lpfc
LIBPFC_DEP += libpfc


all: mystery

clean: libpfc-clean
	rm -f *.o mystery
	
mystery: $(OBJECTS) $(LIBPFC_DEP)
	g++ $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@
	
%.o : %.cpp $(HEADERS) 
	g++ $(CPPFLAGS) -c -std=c++11 -o $@ $<

libpfc:
	cd libpfc && $(MAKE)
	
libpfc-clean:		
	cd libpfc && $(MAKE) clean

