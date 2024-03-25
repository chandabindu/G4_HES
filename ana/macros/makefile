CC=gcc
CXX=g++
#CC=icc
#CXX=icpc
#CFLAGS  = -parallel -par-report3 -par-threshold0 -O3

CFLAGS  = -O2 -MMD

BINDIR = ./bin
LIBDIR = ./lib
#PHCDIR = ./PHC

# for k0ana0
#ROOT	= /usr/local/cern/root_v5.22.00/bin
# for k0ana1 & k0ana2
#ROOT	= /usr/local/cern/root_v5.34.36/bin
# for farm
#ROOT	= /usr/local/cern/root_v5.32.04.x86_64_fc8_gcc4.1.2/bin
ROOT	= /usr/local/cern/root6.20.04/bin


ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)
CXXFLAGS = -Wall -O2 $(ROOTFLAGS) 
CXXLIBS = $(ROOTLIBS)

INCLUDE= -I./ -I$(PHCDIR) -I../../

TARGET1=    solid 
OBJS1  =    solid.o
TARGET2 =   getMatrix
OBJS2   =   $(LIBDIR)/getMatrix.o $(LIBDIR)/calcMatrix.o $(LIBDIR)/calcMatrix_oku.o $(LIBDIR)/readvalue.o 
TARGET3 =   resolution
OBJS3   =   resolution.o 

all: $(TARGET1) \
	 $(TARGET2) \
	 $(TARGET3) 

$(LIBDIR)/%.o : %.cc
	$(CXX) $(CFLAGS) -c -o $@ $< $(CXXFLAGS)

$(LIBDIR)/%.o : %.C
	$(CXX) $(CFLAGS) -c -o $@ $< $(CXXFLAGS)

$(TARGET1): $(patsubst %,$(LIBDIR)/%,$(OBJS1))
	$(CXX) $(CFLAGS) -o $(BINDIR)/$@ $< $(CXXLIBS) $(CXXFLAGS)

${TARGET2}: ${OBJS2}
	${CXX} ${CFLAGS} -o $(BINDIR)/$@ ${OBJS2} ${CXXLIBS} 

$(TARGET3): $(patsubst %,$(LIBDIR)/%,$(OBJS3))
	$(CXX) $(CFLAGS) -o $(BINDIR)/$@ $< $(CXXLIBS) $(CXXFLAGS)


.PHONY: clean
clean:
	rm -f $(LIBDIR)/*.o core $(BINDIR)/* $(LIBDIR)/*.d *.o

-include $(DEPENDS6)
