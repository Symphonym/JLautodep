# Project name goes in the PROJ_NAME variable, it has to be passed
# to the makefile externally

# Default compiler flags and options
CXX := g++
CXXFLAGS := -Iinclude -std=c++11

CXX_DEBUG_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../debug/lib
CXX_RELEASE_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../release/lib

# List of source/object files
SRCS := $(wildcard *.cpp)
SRCS_OBJECT := ${SRCS:.cpp=.o}

# release target, for maximum performance
.PHONY: release
release: ${SRCS_OBJECT}
	${CXX} -o ../release/${PROJ_NAME} ${SRCS_OBJECT} ${CXX_RELEASE_FLAGS} ${CXXFLAGS} -Wall

# debug target, uses debug symbols when compiling
.PHONY: debug
debug: CXXFLAGS += -g
debug: ${SRCS_OBJECT}
	${CXX} -o ../debug/${PROJ_NAME} ${SRCS_OBJECT} ${CXX_DEBUG_FLAGS} ${CXXFLAGS} -Wall

# clean target, cleans the directory from object (.o) files
.PHONY: clean
clean:
	rm *.o


# For effective, fast and correct compiling(not skipping files):
# specify each .h file that each .o file is dependent on in the
# form of:
# objectfile.o: headerfile.h
main.o: MakeDependencyGenerator.h
