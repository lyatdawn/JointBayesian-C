#ifndef __MATLAB_FUNCTION_H__
#define __MATLAB_FUNCTION_H__

#include "variables.hpp"

IntVec unique_cpp(const IntVec& vec);

DataRowVec colsum_cpp(const DataMat& data);

DataVec rowsum_cpp(const DataMat& data);

void pca_dlib_cpp(DataMat input, DataMat& COEFF, DataMat& SCORE);

DataMat double_cpp(DataMat& input);

void remove_cpp(int **dat, int *lenm, int *lenn, int idx);

IntMat remove_element_cpp(const IntMat& M, const IntVec& labels);

void accuracyCompute(const DataVec& group_train, const DataVec& training, const DataVec& group_sample, const DataVec& sample);

int find(int *a, int size, int num);

int find_cpp(const IntVec& labels, const int& ii, int &lab);

IntRowVec randperm_cpp(const int N, const int m);

#endif
