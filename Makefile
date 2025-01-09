CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -g -O2 -pg -Iinclude/
LDFLAGS=-pg

SRCDIR := src
BINDIR := bin

SRC = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/**/*.cc $(SRCDIR)/**/**/*.cc)

all: $(BINDIR)/main

$(BINDIR)/main: $(SRC)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/main $(SRC) $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/main gmon.out profile.txt

.PHONY: all clean
