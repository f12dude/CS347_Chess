sources = $(wildcard source/*.cpp)
headers = $(wildcard source/*.h)
objects = $(sources:%.cpp=%.o)
deps = $(sources:%.cpp=%.d)
CFLAGS += -O2 -pedantic -Wall
CXXFLAGS += -std=c++0x -O2 -pedantic -Wall

override CPPFLAGS += -Isource/sexp

all: client

submit: client
	@echo "$(shell cd ..;sh submit.sh c)"

.PHONY: clean all subdirs

libclient_%.o: override CXXFLAGS += -fPIC
libclient_%.o: %.cpp *$(headers)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(objects) client libclient_network.o libclient_game.o libclient_getters.o libclient_util.o libclient.so
	$(MAKE) -C source/sexp clean

client: $(objects) source/sexp/sexp.a
	$(CXX) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -g -o client

libclient.so: libclient_network.o libclient_game.o libclient_getters.o libclient_util.o source/sexp/libclient_sexp.a
	$(CXX) -shared -Wl,-soname,libclient.so $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o libclient.so

source/sexp/sexp.a source/sexp/libclient_sexp.a:
	$(MAKE) -C $(dir $@) $(notdir $@)

-include $(deps)
