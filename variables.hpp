#ifndef _VARIABLES_H_ 
#define _VARIABLES_H_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>
#include <time.h> 
#include <math.h>
#include <dlib/matrix.h>
#include <dlib/statistics/statistics.h>

using namespace dlib;
using namespace std;

typedef double Dtype;
typedef matrix<Dtype, 0, 0, default_memory_manager, column_major_layout>  DataMat;
typedef matrix<int, 0, 0, default_memory_manager, column_major_layout>  IntMat;
typedef matrix<Dtype, 0, 1> DataVec;
typedef matrix<Dtype, 1, 0> DataRowVec;
typedef matrix<int, 0, 1> IntVec;
typedef matrix<int, 1, 0> IntRowVec;

const char train_filename[] = "lbp_WDRef.dat";
const char trainlabels_filename[] = "id_WDRef.dat";
const char test_filename[] = "lbp_lfw.dat";
const char test_filename_IntraPersonPair[] = "test_filename_IntraPersonPair.dat";
const char test_filename_ExtraPersonPair[] = "test_filename_ExtraPersonPair.dat";
const char train_filename_mapping_A[] = "mapping_A.dat";
const char train_filename_mapping_G[] = "mapping_G.dat";

const char SCOREFILE[] = "SCORE.dat";
const char COEFFFILE[] = "COEFF.dat";
const char train_x_FILE[] = "train_x.dat";
const char train_Intra_FILE[] = "train_Extra.dat"; 
const char train_Extra_FILE[] = "train_Intra.dat";
const char group_train_FILE[] = "group_train.dat";
const char training_FILE[] = "training.dat";
const char normX_FILE[] = "normX.dat";
const char group_sample_FILE[] = "group_sample.dat";
const char sample_FILE[] = "sample.dat";

const int samples = 71846;
const int lbp_num = 5900;
const int feature = 2000;
const int lbp_lfw_nr = 13233;
const int lbp_lfw_nc = 5900;
const int IntraPersonPair_nr = 3000;
const int IntraPersonPair_nc = 2;
const int ExtraPersonPair_nr = 3000;
const int ExtraPersonPair_nc = 2;
const int mapping_A_nr = 2000;
const int mapping_A_nc = 2000;
const int mapping_G_nr = 2000;
const int mapping_G_nc = 2000;  

#endif
