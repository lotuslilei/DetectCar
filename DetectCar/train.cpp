#include "train.h"

class MySVM : public CvSVM
{
public:
	//get the alpha array in SVM decision function.
	double * get_alpha_vector()
	{
		return this->decision_func->alpha;
	}

	//get the rho parameter(offset values) in SVM.
	float get_rho()
	{
		return this->decision_func->rho;
	}
};

Train::Train(){
	PosSamNO = 0;
	NegSamNO = 0;
	HardExampleNO = 0;

	ObjWidth = 0;
	ObjHeigh = 0;
}

int Train::setSamNO(int pos_num, int neg_num, int hard_num){
	if ( pos_num > 0 && neg_num >= 0 && hard_num >= 0 )
	{
		PosSamNO = pos_num;
		NegSamNO = neg_num;
		HardExampleNO = hard_num;
	}else{
		return -1;
	}
	return 0;
}

int Train::setObjSize(int obj_width, int obj_heigh){
	if ( obj_heigh > 0 && obj_width > 0 )
	{
		ObjWidth = obj_width;
		ObjHeigh = obj_heigh;
	}else{
		return -1;
	}

	return 0;
}

int Train::trainHOGSVM(string PosSamPos, string PosSamListFile, string NegSamPos, string NegSamListFile, 
	string HardSamPos, string HardSamListFile, string OutputSampleFeatureMatrixFile, string OutputHOGSVMFile){
	//check the user set something or not.
	if ( PosSamNO == 0 )
	{
		cout<<"Please set sample numbers first."<<endl;
		return -1;
	}else if ( ObjWidth == 0 )
	{
		cout<<"Please set object size first."<<endl;
		return -1;
	}


	////////////// use HOG to train svm classifier //////////////////////////
	//detect window size
	HOGDescriptor hog(Size(ObjWidth,ObjHeigh), Size(16,16), Size(8,8), Size(8,8), 9);
	//the dimensions of HOG descriptor
	int DescriptorDim;
	//svm classifier
	MySVM svm;

	////---- image input ----////
	//the picture name
	string ImgName;
	//a file store the good and bad image examples' name
	ifstream finPos( PosSamListFile );
	ifstream finNeg( NegSamListFile );
	ifstream finHard( HardSamListFile );

	//---- matrix of eigenvectors(numbers of images*DescriptorDim) for training examples ----
	Mat sampleFeatureMat;
	//---- matrix of kinds of training examples(numbers of images*1[1 means have people and -1 means not]) ----
	Mat sampleLabelMat;

	///////////- 1. read good image examples to generate HOG descriptor -/////////////
	////////////////////////////////////////////////////////////////////////////////////
	//here we need to input the car image cut out
	//we also need to adjust the image size.
	/////////////////////////////////////////////////////////////////////////////////
	try{
		for ( int num=0; num<PosSamNO && getline(finPos,ImgName); num++ )
		{
			//get image
			cout<<"Handle: "<<ImgName<<endl;
			ImgName = PosSamPos + ImgName;
			Mat src = imread(ImgName);

			//compute HOG descriptor
			vector<float> descriptor;
			hog.compute(src,descriptor,Size(8,8));

			//use the first example to initial the result matrix.
			if ( num == 0)
			{
				DescriptorDim = descriptor.size();
				sampleFeatureMat = Mat::zeros(PosSamNO+NegSamNO+HardExampleNO, DescriptorDim, CV_32FC1);
				sampleLabelMat = Mat::zeros(PosSamNO+NegSamNO+HardExampleNO, 1, CV_32FC1);
			}

			//store the HOG descriptor computed.
			for ( int i=0; i<DescriptorDim; i++ )
			{
				sampleFeatureMat.at<float>(num,i) = descriptor[i];
			}
			sampleLabelMat.at<float>(num,0) = 1;
		}
	}catch(int e){

	}

	///////////- 2. read bad image examples to generate HOG descriptor -/////////////
	try{
		for ( int num=0; num<NegSamNO && getline(finNeg,ImgName); num++ )
		{
			cout<<"Handle: "<<ImgName<<endl;
			ImgName = NegSamPos + ImgName;
			Mat src = imread(ImgName);

			vector<float> descriptor;
			hog.compute(src,descriptor,Size(8,8));

			for ( int i=0; i<DescriptorDim; i++ )
			{
				sampleFeatureMat.at<float>(num+PosSamNO,i) = descriptor[i];
			}
			sampleLabelMat.at<float>(num+PosSamNO,0) = -1;
		}
	}catch(int e){

	}

	///////////- 3. read hard image examples to generate HOG descriptor -/////////////
	try{
		for ( int num=0; num<HardExampleNO && getline(finHard,ImgName); num++ )
		{
			cout<<"Handle: "<<ImgName<<endl;
			ImgName = HardSamPos + ImgName;
			Mat src = imread(ImgName);

			vector<float> descriptor;
			hog.compute(src,descriptor,Size(8,8));

			for ( int i=0; i<DescriptorDim; i++ )
			{
				sampleFeatureMat.at<float>(num+PosSamNO+NegSamNO,i) = descriptor[i];
			}
			sampleLabelMat.at<float>(num+PosSamNO+NegSamNO,0) = -1;
		}
	}catch(int e){

	}
	/*
	////- output files of vector matrix -////
	ofstream fout( OutputSampleFeatureMatrixFile );
	for ( int i=0; i<PosSamNO+NegSamNO+HardExampleNO; i++ )
	{
		fout<<i<<endl;
		for( int j=0; j<DescriptorDim; j++ ){
			fout<<sampleFeatureMat.at<float>(i,j)<<" ";
		}
		fout<<endl;
	}
	*/

	////- train SVM classifier -////
	//iteration finish condition: over 1000 times or mistake less than FLT_EPSILON
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	//SVM parameters: type, dim, relaxation factor
	CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
	cout<<"Train SVM Classifier..."<<endl;
	svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);
	cout<<"Train SVM finished."<<endl;

	char outsave[512];
	strcpy(outsave,OutputHOGSVMFile.c_str());
	svm.save( outsave );
	
	return 0;
}