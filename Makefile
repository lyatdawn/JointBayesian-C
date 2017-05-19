# Dlib dependency
DLIB_PATH := /path/dlib-19.2
INC_PATH := -I$(DLIB_PATH)
LIB := $(DLIB_PATH)/examples/build/dlib_build/libdlib.a

# Basic parameters
CXX = g++

CXX_FLAGS =  -DUSE_AVX_INSTRUCTIONS -DUSE_SSE2_INSTRUCTIONS -DUSE_SSE4_INSTRUCTIONS 

LINK_FLAGS =  -lglog 

# debug switch
DEBUG := 0
ifeq ($(DEBUG), 1)
    CXX_FLAGS += -g -O0
else
    CXX_FLAGS += -O2
endif

SRC= matlab_function.cpp loadData.cpp test_main.cpp JointBayesian.cpp
OBJ = $(SRC:.cpp=.o)

test: $(OBJ)
	$(CXX) -o $@ $^ $(LIB) $(LINK_FLAGS) -fopenmp

%.o: %.cpp
	$(CXX) -c $< $(INC_PATH) -fopenmp $(CXX_FLAGS)

clean:
	rm -rf *.o test
.PHONY: clean
