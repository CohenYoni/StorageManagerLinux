.DEFAULT_GOAL := install
.PHONY: clean
#.SILENT:

SOURCES = $(PLAIN_SOURCES) $(LIB_SOURCES)
PLAIN_SOURCES = driver.cpp
LIB_SOURCES = product.cpp database.cpp parser.cpp

CXX = g++ #Compiler for c++
CXXFLAGS += -O2 -pipe -march=core2 #c++ options
LDFLAGS += -Wl,-O,1 -lfcgi++ -lfcgi -lsqlite3 #Linker options

%.o: %.cpp
	$(CXX) -MMD -shared -fpic -c -o $@ $< $(CXXFLAGS)

run: $(PLAIN_SOURCES:%.cpp=%.o) libstorage.so
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -L. -lstorage -Wl,-rpath,.

libstorage.so: $(LIB_SOURCES:%.cpp=%.o)
	$(CXX) -o $@ $^ -shared -fpic $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(SOURCES:%.cpp=%.o) $(SOURCES:%.cpp=%.d) run libstorage.so

install: run
	sudo spawn-fcgi -p 8000 -n ./run &
	echo "the server is running..."

-include $(SOURCES:%.cpp=%.d)
