# Dlib dependency
DLIB_PATH := /home/ly/dlib-19.0
INC_PATH := -I$(DLIB_PATH)
LIB := $(DLIB_PATH)/examples/build/dlib_build/libdlib.a
#cd examples
#mkdir build
#cd build
#cmake ..
#cmake --build . --config Release
#利用dlib上提供的编译的教程-cmake。那么在build/dlib_build/libdlib.a
#将生成libdlib.a 这个静态库，那么以后再编译dlib的例子时，就可以
#g++ libdlib.a 了。不用再写-lpthread -lX11等。

# Basic parameters
CXX = g++

CXX_FLAGS =  -DUSE_AVX_INSTRUCTIONS -DUSE_SSE2_INSTRUCTIONS -DUSE_SSE4_INSTRUCTIONS 
#调用DUSE_AVX_INSTRUCTIONS DUSE_SSE2_INSTRUCTIONS DUSE_SSE4_INSTRUCTIONS
#来实现加速，因为程序设计到矩阵的计算，所以使用AVX库、SSE库进行加速。
#是一些指令集！！
#-DDLIB_USE_LAPACK -DDLIB_USE_BLAS #-DEIGEN_USE_MKL_ALL  -I/share/home/guan/intel/compilers_and_libraries_2016.3.210/linux/mkl/include/
#但是LAPACK、BLAS的配置有一些问题，可以先不用这两个库进行加速。

LINK_FLAGS =  -lglog 
#/share/home/guan/OpenBlasMPI/lib/libopenblas_haswellp-r0.2.18.a /share/home/guan/intel/compilers_and_libraries_2016.3.210/linux/mkl/lib/intel64/libmkl_blacs_intelmpi_lp64.a

# debug switch
DEBUG := 0
ifeq ($(DEBUG), 1) #判断DEBUG 和 1 是否相等。相等那个 g++ -O0,
#即编译不做优化，否则利用 -O2来做优化。
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