#include "matlab_function.hpp"
#include "JointBayesian.hpp"
#include "loadData.hpp"

using namespace dlib;
using namespace std;

const int iter = 500;

int main()
{
	DataMat train_x;
	IntVec labels;
	
	// load('lbp_WDRef.mat')
    // load('id_WDRef.mat')
	loadData_train(train_x, labels);

	// X = double(lbp_WDRef); X = sqrt(X);
	train_x = double_cpp(train_x);
	train_x = sqrt(train_x);

	// X = bsxfun(@rdivide,X,sum(X,2));
	DataVec result(train_x.nr());
	result = rowsum_cpp(train_x);	
	for(int i = 0; i < train_x.nr(); i++){
		for(int j = 0; j < train_x.nc(); j++){
                if (result(i) != 0){
			       train_x(i, j) = train_x(i, j)/result(i);
                }
		}
	}
	

	/*
	// train_mean = mean(X, 1);
	DataRowVec train_mean(train_x.nc());
	train_mean = colsum_cpp(train_x)/train_x.nr();
	for(int i=0; i<train_x.nc(); i++){
			train_mean(i)=result_sum_col(i)/train_x.nr();
	}
	cout<<"train_x:\n"<<train_mean<<endl;
	
	//train_mean is not use. 
	*/


	//[COEFF,SCORE] = princomp(X,'econ');
	DataMat SCORE(train_x.nr(), train_x.nc());
	DataMat COEFF(train_x.nc(), train_x.nc());
	
	pca_dlib_cpp(train_x, COEFF, SCORE);
    //WriteDataToFile(SCOREFILE, SCORE);
	//WriteDataToFile(COEFFFILE, COEFF);
	//LoadDataFromFile(SCOREFILE, SCORE);
	//LoadDataFromFile(COEFFFILE, COEFF);


	//train_x = SCORE(:,1:2000);
	train_x = colm(SCORE, range(0, 1999));
	//WriteDataToFile(train_x_FILE, train_x);
 
 	// [mappedX, mapping] = JointBayesian(train_x, labels);
	// JointBayesian(train_x, labels, mapping_A, mapping_G, iter);
    DataMat  mapping_A = zeros_matrix<Dtype>(mapping_A_nr, mapping_A_nc);
    // LoadDataFromFile(train_filename_mapping_A, mapping_A);
    
    DataMat  mapping_G = zeros_matrix<Dtype>(mapping_G_nr, mapping_G_nc);
    // LoadDataFromFile(train_filename_mapping_G, mapping_G); 
	// You can use JointBayesian.cpp training the mapping_A and mapping_G. 
    
    JointBayesian(train_x, labels, mapping_A, mapping_G, iter);
    
    // [classes, bar, labels] = unique(labels);
	IntVec classes; 
	classes = unique_cpp(labels);
	int nc= classes.size();


	// train_Intra(2*i-1,:) = randperm(sum(labels == i),2) + find(labels == i,1,'first') - 1;
	// train_Intra(2*i,:) = randperm(sum(labels == i),2) + find(labels == i,1,'first') - 1;
	IntMat train_Intra = zeros_matrix<int>(2 * nc, 2);
    for(int i = 0; i < nc; i++){
        IntRowVec a = randperm_cpp(sum(labels == (i+1)), 2);
        int c = 0;
        find_first_cpp(labels, i+1, c);
        IntRowVec b = randperm_cpp(sum(labels == (i+1)), 2);
        
        train_Intra(2 * i, 0) = a(0) + c;
        train_Intra(2 * i, 1) = a(1) + c;
        
        train_Intra(2 * i + 1, 0) = b(0) + c;
        train_Intra(2 * i + 1, 1) = b(1) + c;
    }
    //WriteDataToFile_INT(train_Intra_FILE, train_Intra); 
	//LoadDataFromFile_INT(train_Intra_FILE, train_Intra);
	
	
	// train_Extra = reshape(randperm(length(labels),20000),10000,2);
	IntMat train_Extra_1(10000, 2);
	train_Extra_1 = reshape(randperm_cpp(labels.size(), 20000), 10000, 2);
	train_Extra_1 = remove_element_cpp(train_Extra_1, labels);
	
	// train_Extra(labels(train_Extra(:,1))==labels(train_Extra(:,2)),:)=[];
	IntMat train_Extra(train_Intra.nr(), train_Extra_1.nc());
	for(int i = 0; i < train_Intra.nr(); i++){
		for(int j = 0; j < train_Extra_1.nc(); j++){
			train_Extra(i, j) = train_Extra_1(i, j);
		}
	}
	//WriteDataToFile(train_Extra_FILE, train_Extra);
    //LoadDataFromFile_INT(train_Extra_FILE, train_Extra);
    
 	// Dis_train_Intra Dis_train_Extra
	DataVec Dis_train_Intra = zeros_matrix<Dtype>(train_Intra.nr(), 1);
	DataVec Dis_train_Extra = zeros_matrix<Dtype>(train_Intra.nr(), 1);

	for(int i = 0; i < train_Intra.nr(); i++){
		Dis_train_Intra(i) = rowm(train_x, train_Intra(i, 0) - 1) * mapping_A * trans(rowm(train_x, train_Intra(i, 0) - 1)) + 
        rowm(train_x, train_Intra(i, 1) - 1) * mapping_A * trans(rowm(train_x, train_Intra(i, 1) - 1)) -
        2 * (rowm(train_x, train_Intra(i, 0) - 1) * mapping_G * trans(rowm(train_x, train_Intra(i, 1) - 1)));
		
		Dis_train_Extra(i) = rowm(train_x, train_Extra(i, 0) - 1) * mapping_A * trans(rowm(train_x, train_Extra(i, 0) - 1)) + 
        rowm(train_x, train_Extra(i, 1) - 1) * mapping_A * trans(rowm(train_x, train_Extra(i, 1) - 1)) -
        2 * (rowm(train_x, train_Extra(i, 0) - 1) * mapping_G * trans(rowm(train_x, train_Extra(i, 1) - 1)));
	}


	// group_train = [ones(size(Dis_train_Intra,1),1);zeros(size(Dis_train_Extra,1),1)];
	// training = [Dis_train_Intra;Dis_train_Extra];
	DataVec group_train = zeros_matrix<Dtype> (Dis_train_Intra.size() + Dis_train_Extra.size(), 1);
	for(int i = 0; i < Dis_train_Intra.size(); i++){
		group_train(i) = 1;
	}
	
	DataVec training(Dis_train_Intra.size() + Dis_train_Extra.size(), 1);
	for(int i = 0; i < Dis_train_Intra.size(); i++){
		training(i) = Dis_train_Intra(i);
	}
	for(int i = Dis_train_Intra.size(); i < Dis_train_Intra.size() + Dis_train_Extra.size(); i++){
		training(i) = Dis_train_Extra(i - Dis_train_Intra.size());
	}
   	//WriteDataToFile(group_train_FILE, group_train);
	//WriteDataToFile(training_FILE, training);


	// TEST.
	//load('lbp_lfw.mat') 
	DataMat normX = zeros_matrix<Dtype>(lbp_lfw_nr, lbp_lfw_nc);
	LoadDataFromFile(test_filename, normX);


	//load('pairlist_lfw.mat')
	DataMat test_Intra = zeros_matrix<Dtype>(IntraPersonPair_nr, IntraPersonPair_nc);
	LoadDataFromFile(test_filename_IntraPersonPair, test_Intra);

	DataMat test_Extra = zeros_matrix<Dtype>(ExtraPersonPair_nr, ExtraPersonPair_nc) ;
	LoadDataFromFile(test_filename_ExtraPersonPair, test_Extra);


	//normX = sqrt(normX); normX = bsxfun(@rdivide,normX,sum(normX,2)); 
    normX = double_cpp(normX); 
    normX = sqrt(normX);

	// normX = bsxfun(@rdivide,normX,sum(normX,2));
	DataVec result_norm(normX.nr());
	result_norm = rowsum_cpp(normX);
	for(int i = 0; i < normX.nr(); i++){
		for(int j = 0; j < normX.nc(); j++){
			normX(i, j) = normX(i, j)/result_norm(i);
		}
	}
	
	//normX = normX * COEFF(:,1:2000); 
	normX = normX * (colm(COEFF, range(0, 1999)));
	//WriteDataToFile(normX_FILE, normX);


	// result_Intra; result_Extra;
	DataVec result_Intra = zeros_matrix<Dtype> (3000, 1);
	DataVec result_Extra = zeros_matrix<Dtype> (3000, 1); 
	for(int i = 0; i < 3000; i++){
		result_Intra(i) = rowm(normX, test_Intra(i, 0) - 1) * mapping_A * trans(rowm(normX, test_Intra(i, 0) - 1)) + 
        rowm(normX, test_Intra(i, 1) - 1) * mapping_A * trans(rowm(normX, test_Intra(i, 1) - 1)) -
        2 * (rowm(normX, test_Intra(i, 0) - 1) * mapping_G * trans(rowm(normX, test_Intra(i, 1) - 1)));
		
		result_Extra(i) = rowm(normX, test_Extra(i, 0) - 1) * mapping_A * trans(rowm(normX, test_Extra(i, 0) - 1)) + 
        rowm(normX, test_Extra(i, 1) - 1) * mapping_A * trans(rowm(normX, test_Extra(i, 1) - 1)) -
        2 * (rowm(normX, test_Extra(i, 0) - 1) * mapping_G * trans(rowm(normX, test_Extra(i, 1) - 1)));
	}
	
	
	// group_sample = [ones(3000,1);zeros(3000,1)];
	DataVec group_sample = zeros_matrix<Dtype> (6000, 1);
	for(int i = 0; i < 3000; i++){
		group_sample(i) = 1;
	}
	
	DataVec sample(6000, 1);
	for(int i = 0; i < 3000; i++){
		sample(i) = result_Intra(i);
	}
	for(int i = 3000; i < 6000; i++){
		sample(i) = result_Extra(i - 3000);
	}
	//WriteDataToFile(group_sample_FILE, group_sample);
	//WriteDataToFile(sample_FILE, sample);
	
	
	// the method of classification 
	accuracyCompute(group_train, training, group_sample, sample);
    
	return 0;
}
