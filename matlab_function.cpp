#include "matlab_function.hpp"

IntVec unique_cpp(const IntVec& vec) {
  std::vector<int> tmp_vec(vec.begin(), vec.end());
  std::sort(tmp_vec.begin(), tmp_vec.end());
  std::vector<int>::iterator it;
  it = std::unique(tmp_vec.begin(), tmp_vec.end());
  tmp_vec.resize(std::distance(tmp_vec.begin(), it));
  IntVec result = mat(tmp_vec.data(), tmp_vec.size());
  return result;
}


DataRowVec colsum_cpp(const DataMat& data) {
  const int col_num = data.nc();
  DataRowVec result(col_num);
  for (int i = 0; i < col_num; ++i) {
    result(i) = sum(colm(data, i));
  }
  return result;
}


DataVec rowsum_cpp(const DataMat& data) {
  const int row_num = data.nr();
  DataVec result(row_num);
  for (int i = 0; i < row_num; ++i) {
    result(i) = sum(rowm(data, i));
  }
  return result;
}


void pca_dlib_cpp(DataMat input, DataMat& COEFF, DataMat& SCORE){
     
     DataMat X = trans(input); 
     DataVec X1 = rowsum_cpp(X)/X.nc();
     for (int i = 0; i < X.nr(); i++){
         for (int j = 0; j < X.nc(); j++){
             X(i, j) = X(i, j) - X1(i);
         }
     }
     
     DataMat C = X * trans(X) * (1.0 / X.nc()); 
     DataMat temp, eigen;
     DataVec eigenvalues; 
     
     svd(C, temp, eigen, COEFF);
     eigenvalues = diag(eigen);
     
     rsort_columns(COEFF, eigenvalues);
     
     DataRowVec C1 = colsum_cpp(input)/input.nr();
     for (int j = 0; j < input.nc(); j++){
        for (int i = 0; i < input.nr(); i++){
            input(i, j) = input(i, j) - C1(j);
        }
     }
     
     SCORE = input * COEFF;
}


DataMat double_cpp(DataMat& input)
{
	for(int i=0; i < input.nr(); i++){
		for(int j=0; j < input.nc(); j++){
			input(i,j) = double(input(i,j));
		}
	}
	return input;
}


inline void remove_cpp(int **dat, int *lenm, int *lenn, int idx)
{
    (*lenm)--;
	
    for (int i = idx; i < *lenm; i++){
		for(int j=0; j < *lenn; j++){
			dat[i][j] = dat[i+1][j];
		}
	}
}


IntMat remove_element_cpp(const IntMat& M, const IntVec& labels)
{
	int lenm = M.nr();
	int lenn = M.nc();
	
	int**  a = new int *[lenm];
	for (int i = 0; i < lenm; i++){
        a[i] = new int[lenn];
    }
    
	for (int i = 0; i < lenm; i++){
		for(int j = 0; j < lenn; j++){
			a[i][j] = M(i, j);
		}
	}
	
	for (int i = 0; i < lenm; i++){
        int aa = M(i, 0);
        int bb = M(i, 1);
		if( labels(aa) == labels(bb)){
			remove_cpp(a, &lenm, &lenn, i);
		}
	}

	IntMat N(lenm, 2);
	
	for(int i = 0; i < lenm; i++){
		for(int j = 0; j < lenn; j++){
			N(i,j) = a[i][j];
		}
	}
	
    for(int i = 0; i < lenn; i++) {
       delete[] a[i];
    }
    delete[] a;

	
	return N;
}

/*
// Compute input covarience
DataMat cov_cpp(const DataMat& input)
{	
	const int col_num=input.nc();
	const int row_num=input.nr();
	
	DataMat output(row_num, col_num);
	DataRowVec result_sum_col(col_num);
	result_sum_col = colsum_cpp(input);

	DataRowVec result_ave_col(clo_num);
	for(int i=0; i<col_num; i++){
		resylt_ave_col(i)=result_sum_col(i)/row_num;
	}

	for(int i=0; i<row_num; i++){
		for(int j=0; j<col_num; j++){
			output(i,j)=input(i,j)-result_ave_col(i);
		}
	}

	return trans(output)*output/double(row_num-1);
}
*/


void accuracyCompute(const DataVec& group_train, const DataVec& training, const DataVec& group_sample, const DataVec& sample)
{
	int m, n;
	m = group_train.nr();
	n = group_train.nc();
	double starderd = 0, eg1 = 0, num1 = 0, eg2 = 0, num2 = 0;

	for (long i = 0; i < m; ++i)
	{
		if (group_train(i) == 0){
	     	eg2 = eg2 + training(i);
		    num2 = num2 + 1;
        }
		else{
			eg1 = eg1 + training(i);
		    num1 = num1 + 1;
        }
	}
	
	starderd = (eg1 + eg2)/(num1+num2);
	
	m = sample.nr();
	n = sample.nc();
	
	IntVec label(m, 1);
	label = zeros_matrix<int> (m, 1);
	
	double accuracy = 0;
	for (long i = 0; i < m; ++i)
	{
		if (sample(i) > starderd)
			label(i) = 1;
		else
			label(i) = 0;
		if (label(i) == group_sample(i))
			accuracy = accuracy + 1;
	}
	
	cout<<"accuracy is: \n"<<accuracy/m<<endl;
}


int find(int *a, int size, int num)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] == num)
        {
            return 0;
        }
    }
 
    return 1;
}


int find_cpp(const IntVec& labels, const int& ii, int &lab)
{
	double* label = new double[labels.size()];
	for(int i = 0; i < labels.size(); i++){
		label[i] = labels(i);
	}

	double* iterator = std::find(
		static_cast<double*>(label),
		static_cast<double*>(label)+labels.size(),
		ii); 
	
	int flag1 = 0;
	int flag2 = 0;
	int len_labels =  labels.size();
	
	for(int idx = 0; idx < len_labels; idx++){
		if(labels(idx) == *iterator){
            flag1 = 1;
		}
		if(flag1 == 1){
                 lab = idx;
                 flag2 = 1;
                 break;
                 
        }
        if(flag2 == 1) break;
	}
	delete []label;

}


IntRowVec randperm_cpp(const int N, const int m) {
  IntRowVec result(1, m);
  
  int* a = new int[m];
  int num = 0;
  std::srand(clock());
  bool exit = false;
  while (!exit)
    {
      int x = (std::rand() %  (N - 1 + 1)) + 1;
      bool next = false;
 
      while (!find(a, num + 1, x))
      {
          x = (std::rand() % (N - 1 + 1)) + 1;
      }
      a[num] = x;
      ++num;
      if (m <= num)
      {
          exit = true;
      }
    }
    
   for (int i = 0; i < m; i++){
       result(i) = a[i];
   }
   delete []a;
  
  return result;
}
