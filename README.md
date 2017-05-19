Bayesian Face Revisited: A Joint Formulation

This is the implementation for Bayesian Face Revisited: A Joint Formulation. It is a C++ implementation based the implementation of MATLAB in https://github.com/ucla-vision/entropy-sgd.

First, you should install the dlib. And, cmake .. && cmake --build . --config Release to get the library of dlib: libdlib.a

This implementation is not use the accelerated library, like BLAS, LAPACK and so on. Moreever, there are must flaws in the implementation. Therefore, the implementation is not faster.
 
