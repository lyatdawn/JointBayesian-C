#include "JointBayesian.hpp"

int JointBayesian(const DataMat& train_x, const IntVec& labels, DataMat& mapping_A, DataMat& mapping_G, const int iter )  
{
	const int m = labels.size();
	const int n = train_x.nc();
	
    //make sure labels are nice
	IntVec classes = unique_cpp(labels);
	const int nc = classes.size();
	DataMat Sw = zeros_matrix<Dtype>( train_x.nc(),train_x.nc()) ;
	double withinCount = 0;
	IntVec numberBuff(1000); numberBuff = 0;
	std::vector<int> maxNumberInOneClass ;
	
	std::vector<DataMat> cur(nc);
	cur.clear();
	for ( int i = 0; i < nc; i++ )
	{
		IntVec find = find_cpp(labels, i+1);
		
		DataMat temp_x(find.nr(),n);
		
		for(int k=0; k<find.nr(); k++)
		{
			
			set_rowm(temp_x,k) = rowm(train_x,find(k));
		}
		
		cur.push_back(temp_x);
		
		double np = cur[i].nr();
		if (np>=1)
		{
			withinCount = withinCount + np;
			if ( numberBuff(np)==0 )
			{
				numberBuff(np)=1;
				maxNumberInOneClass.push_back(np);
			}
		}
	}
	
	cout << nc<<"  "<<withinCount<<endl;
	cout<<"prepare done, maxNumberInOneClass="<< maxNumberInOneClass.size() <<endl;
	DataMat u(n,nc);
	DataMat ep(n,withinCount);
	u = 0; ep = 0;
	double nowp = 0;
	// sum over classes
	for (int i = 0; i < nc; i++)
	{
		// Update within-class scatter 
		DataRowVec result_sum_col(cur[i].nc());
		result_sum_col=colsum_cpp(cur[i]);
		
		for(int j=0; j<(cur[i].nc()); j++)
		{
			result_sum_col(j) = result_sum_col(j)/(double)cur[i].nr();
		}
		set_colm(u,i) = trans(result_sum_col);
		
		if (cur[i].nr() > 1 )
		{
			set_colm(ep,range(nowp, nowp+cur[i].nr()-1)) = trans(cur[i])-repmat_cpp(colm(u,i),1,cur[i].nc());
			nowp = nowp + cur[i].nr();
			
		}
	}
	// WriteDataToFile("u0.dat", u);
	DataMat Su = cov_cpp(trans(u));
	// WriteDataToFile("Su0.dat", Su);
	Sw = cov_cpp(trans(ep));
	DataMat oldSw = Sw;
	std::vector<DataMat> SuFG(1000), SwG(1000);
	
	for (int l = 0; l < iter; l++)
	{
		DataMat F = inv(Sw);
		ep = 0;
		nowp = 0;
		for (int g = 0; g < 1000; g++)
		{
			if (numberBuff(g)==1)
			{
				DataMat G = -1*inv(g*Su + Sw) * Su * inv(Sw);
				SuFG[g] = Su * (F +g * G);
				SwG[g] = Sw * G;
			}
			
		}
		
		for (int i = 0; i < nc; ++i)
		{
			int nnc = cur[i].nr();
			set_colm(u,i) = rowsum_cpp(SuFG[nnc] * trans(cur[i])) ;
			set_colm(ep , range(nowp,nowp+nnc-1))= trans(cur[i]) - repmat_cpp(rowsum_cpp(SwG[nnc]*trans(cur[i])),1,cur[i].nc()); 
			nowp = nowp +nnc;
		}
		
		cout<<"u="<<u.nr()<< u.nc()<<endl;
		Su = cov_cpp(trans(u));
		Sw = cov_cpp(trans(ep));
		Dtype cc = norm_cpp((Sw-oldSw)/norm_cpp(Sw));
		
		if (cc< 1e-6)
		{
			break;
		}
		
		oldSw = Sw;
	}
	
	DataMat  F = inv (Sw);
	mapping_G =  -1*inv(2*Su + Sw) * Su * inv(Sw);
	mapping_A =  inv(Su + Sw) - (F + mapping_G);
	// WriteDataToFile("mapping_G.dat", mapping_G);
	// WriteDataToFile("mapping_A.dat", mapping_A); // You can use JointBayesian.cpp training the mapping_A and mapping_G.
	
	return 0;
}
