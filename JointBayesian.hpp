#ifndef __JOINTBAYESIANMASTER_H_
#define __JOINTBAYESIANMASTER_H_

#include "variables.hpp"
#include "matlab_function.hpp"

int JointBayesian(const DataMat& train_x, const IntVec& labels, DataMat& mapping_A,  DataMat& mapping_G, int iter);

#endif
