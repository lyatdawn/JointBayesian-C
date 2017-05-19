#include "loadData.hpp"

template <class T>
int LoadDataFromFile(const char dat_name[], T& M, int start_loc) {
  std::ifstream file;
  file.open(dat_name, std::ifstream::in | std::ifstream::binary);
  if (file.fail()) {
    std::cerr << "Fail to open " << dat_name << std::endl;
    return -1;
  }
  file.seekg(start_loc, file.beg);
  file.read(reinterpret_cast<char*>(M.begin()), sizeof(Dtype) * M.size());
  file.close();
  return 0;
}


int LoadDataFromFile_INT(const char dat_name[], IntMat& M, int start_loc) {
  std::ifstream file;
  file.open(dat_name, std::ifstream::in | std::ifstream::binary);
  if (file.fail()) {
    std::cerr << "Fail to open " << dat_name << std::endl;
    return -1;
  }
  file.seekg(start_loc, file.beg);
  file.read(reinterpret_cast<char*>(M.begin()), sizeof(int) * M.size());
  file.close();

  return 0;
}
 

template <class T>
int WriteDataToFile(const char dat_name[], T& M, int start_loc) {
  std::ofstream file;
  file.open(dat_name);
  if (file.fail()) {
    std::cerr << "Fail to open " << dat_name << std::endl;
    return -1;
  }

  file.write(reinterpret_cast<char*>(M.begin()), sizeof(Dtype) * M.size());
  file.close();
}

int WriteDataToFile_INT(const char dat_name[], IntMat& M, int start_loc) {
  std::ofstream file;
  file.open(dat_name);
  if (file.fail()) {
    std::cerr << "Fail to open " << dat_name << std::endl;
    return -1;
  }

  file.write(reinterpret_cast<char*>(M.begin()), sizeof(int) * M.size());
  file.close();
}


int loadData_train(DataMat& train_x, IntVec& labels){
	train_x = zeros_matrix<Dtype>(samples, lbp_num);
	LoadDataFromFile(train_filename, train_x);
	
	DataMat labels_tmp(samples, 1);
	LoadDataFromFile(trainlabels_filename, labels_tmp);
	labels = matrix_cast<int>(labels_tmp);
	
	return 0;
}
