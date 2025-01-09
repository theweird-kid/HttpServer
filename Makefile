CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -g -O2 -Iinclude/
LDFLAGS=

SRCDIR := src
BINDIR := bin

SRC = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/**/*.cc $(SRCDIR)/**/**/*.cc)

all:
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/main $(SRC) $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/main

.PHONY: all clean
