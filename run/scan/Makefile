CC = gcc 
CXX = g++ 

BINDIR = ./bin
LIBDIR = ./lib
CURDIR = .


inloop :
	$(CXX) -std=c++0x  inloop.cc -o $@

.PHONY: clean
clean:
	rm -f $(LIBDIR)/*.o core $(BINDIR)/* $(LIBDIR)/*.d *.o inloop 
