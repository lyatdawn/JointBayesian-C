#ifndef _LOADDATA_H_
#define _LOADDATA_H_

#include "variables.hpp"

template <class T>
int LoadDataFromFile(const char dat_name[], T& M, int start_loc = 0);

int LoadDataFromFile_INT(const char dat_name[], IntMat& M, int start_loc = 0);

template <class T>
int WriteDataToFile(const char dat_name[], T& M, int start_loc = 0);

int WriteDataToFile_INT(const char dat_name[], IntMat& M, int start_loc = 0);

int loadData_train(DataMat& train_x, IntVec& labels);

#endif
