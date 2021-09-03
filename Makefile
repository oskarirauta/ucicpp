all: uci_test

CXX?=g++
CXXFLAGS?=--std=c++17
INCLUDES:=-I./include -I.

SHARED_OBJS:= \
	objs/common.o

UCI_OBJS:= \
	objs/uci_helper.o

EXAMPLE_OBJS:= \
	objs/uci_main.o

LIBS:=
UCI_LIBS:=-luci
JSON_LIBS:=-ljsoncpp

objs/common.o: shared/common.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci_helper.o: uci/uci_helper.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci_main.o: example/uci_main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

uci_test: $(UCI_OBJS) $(SHARED_OBJS) $(EXAMPLE_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) $(UCI_LIBS) $(JSON_LIBS) -o $@;

clean:
	rm -f objs/*.o uci_test
