CXX=c++
CXXFLAGS = -g -std=c++14 -Wall -Wextra

default: Test.x

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .cc .x

.PHONY: default

Test.x: Test.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f Test.x *~

.PHONY: clean
