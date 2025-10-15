# We build two versions of the shared library
# Nothing special is required to load the C++ libraries into python using ROOT
# If we use ctypes and load the functions by hand in python, they must be
# linked using the extern "C" setting.  Otherwise python will not be able to
# recognize the names of the functions (c.f. name mangling)


# here we access the root configuration, include files, and libraries
ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs) -lMathMore
ROOTGLIBS  = $(shell root-config --glibs)
ROOTFLAGS   = $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) 
CXXFLAGS  = $(ROOTCFLAGS) -I$(ODELIB) -Wall -O3 -fPIC -shared
LDFLAGS    = $(ROOTLIBS) $(ROOTGLIBS)
GXX	   = g++ $(CXXFLAGS)

all: mylib.so mylib.soc mycpplib.so

mylib.so: mylib.cpp mylib.h # build shared library
# 	g++ -O3 -Wall -fPIC -shared -omylib.so mylib.cpp
	$(GXX) $(CXXFLAGS) -omylib.so mylib.cpp $(LDFLAGS)

mylib.soc: mylib.cpp mylib.h  # build "so" w/ C style linkage
# 	g++ -O3 -Wall -fPIC -DEXTERNC -shared -omylib.soc mylib.cpp
	$(GXX) $(CXXFLAGS) -DEXTERNC -omylib.soc mylib.cpp $(LDFLAGS)

mycpplib.so: mycpplib.cpp mycpplib.h
# 	g++ -O3 -Wall -fPIC -shared -omycpplib.so mycpplib.cpp
	$(GXX) $(CXXFLAGS) -omycpplib.so mycpplib.cpp $(LDFLAGS)


clean:
	rm -f mylib.so mylib.soc mycpplib.so *~

cleanall: clean
	rm -rf __pycache__ .ipynb_checkpoints
