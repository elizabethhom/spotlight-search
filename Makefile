CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11

gerp:  hashTable.o FSTree.o DirNode.o main.o
	${CXX} -o $@ $^

%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<