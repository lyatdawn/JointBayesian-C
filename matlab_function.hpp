#ifndef __MATLAB_FUNCTION_H__
#define __MATLAB_FUNCTION_H__

#include "variables.hpp"

IntVec unique_cpp(const IntVec& vec);

DataRowVec colsum_cpp(const DataMat& data);

DataVec rowsum_cpp(const DataMat& data);

Dtype norm_cpp(const DataMat& mat);

DataMat repmat_cpp(const DataMat& A, const int M, const int N);

DataMat cov_cpp(const DataMat& input);

void pca_dlib_cpp(DataMat input, DataMat& COEFF, DataMat& SCORE);

DataMat double_cpp(DataMat& input);

void remove_cpp(int **dat, int *lenm, int *lenn, int idx);

IntMat remove_element_cpp(const IntMat& M, const IntVec& labels);

void accuracyCompute(const DataVec& group_train, const DataVec& training, const DataVec& group_sample, const DataVec& sample);

int find(int *a, int size, int num);

int find_first_cpp(const IntVec& labels, const int& ii, int &lab);

IntVec find_cpp(const IntVec& vec, const int &ii);

IntRowVec randperm_cpp(const int N, const int m);

#endif
