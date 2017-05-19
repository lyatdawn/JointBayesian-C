#ifndef __DATA_FUNCTION_H_
#define __DATA_FUNCTION_H_

#include "variables.hpp"

template <class T>
int LoadDataFromFile(const char dat_name[], T& M, int start_loc = 0);

template <class T>
int WriteDataToFile(const char dat_name[], T& M, int start_loc = 0);

#endif
