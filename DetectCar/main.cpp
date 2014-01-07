#include "train.h"
#include "illumination.h"
#include "generateImage.h"
#include "generateTrainImage.h"
#include "generatenegativeimages.h"
#include "resize.h"
#include "detect.h"
#include "edge.h"
#include "ColorDetect.h"
#include "ShapeDetect.h"

using namespace std;

void main(){

	//user choose to train or detect
	int c;

	cout<<"###################################################"<<endl;
	cout<<"Please Choose The Option You Want!"<<endl;
	cout<<"0: Video to Image Sequence "<<endl;
	cout<<"1: Illumination "<<endl;
	cout<<"2: train Car_HOG_SVM "<<endl;
	cout<<"3: train Mot_HOG_SVM "<<endl;
	cout<<"4: Generate Train Image "<<endl;
	cout<<"5: Resize Image "<<endl;
	cout<<"6: Train Neg Images"<<endl;
	cout<<"7: Img Detect"<<endl;
	cout<<"8: Video Detect"<<endl;
	cout<<"9: Resize Image List"<<endl;
	cout<<"10:Test color shape detect"<<endl;
	cout<<"11:Convert Car_Neg Image to Mot_Neg Image"<<endl;
	cout<<"###################################################"<<endl;

	cin>>c;
	if ( c==0 )
	{
		cout<<"Start Video to Image Sequence."<<endl;

		//video to images
		GenerateImage mygeneratetoimage;

		//use to show frame NO.xx;
		mygeneratetoimage.setVideo("D:\\Data\\PR\\test.avi");
		int fi = 10;
		if( mygeneratetoimage.hasImg(fi) ){
			Mat frame = mygeneratetoimage.getImg(fi);
			imshow("frame",frame);
			waitKey(0);
		}
		
		//Use to generate images and store automic.
		//mygeneratetoimage.VideoToImgStore("D:\\Data\\PR\\test.avi","D:\\Data\\PR\\Video\\","D:\\Data\\PR\\Video\\","ImagesName.txt");

	}else if ( c == 1 )
	{
		cout<<"Start Train."<<endl;

		//illmination
		Illumination myillumination;
		string ImgPos = "D:\\Data\\PR\\Pos\\Car\\";
		string inImgName,outImgName;
		ifstream fin_ill_pos_car("D:\\Data\\PR\\Pos\\TrainPositiveCarImageList.txt");
		while(getline(fin_ill_pos_car,inImgName)){
			cout<<"Illumination Handle: "<<inImgName<<endl;
			outImgName = "ill_"+inImgName;
			myillumination.singleIllumination(ImgPos,inImgName,ImgPos,outImgName);
		}


	}else if ( c == 2 )
	{
		//start train.
		Train mytrain;
		mytrain.setObjSize(128,128);
		mytrain.setSamNO(1231,19743,1);
		mytrain.trainHOGSVM(
			"D:\\Data\\PR\\Pos\\Car\\",
			"D:\\Data\\PR\\Pos\\TrainPositiveCarImageList.txt",
			"D:\\Data\\PR\\Neg\\Car\\",
			"D:\\Data\\PR\\Neg\\TrainNegativeCarImageList.txt",
			"D:\\Data\\PR\\Hard\\Car\\",
			"D:\\Data\\PR\\Hard\\TrainHardCarImageList.txt",
			"D:\\Data\\PR\\matrix.txt",
			"D:\\Data\\PR\\hog_svm.xml");

	}else if ( c == 3 )
	{
		//start train.
		Train mytrain;
		mytrain.setObjSize(48,144);
		mytrain.setSamNO(903,59229,1);
		mytrain.trainHOGSVM(
			"D:\\Data\\PR\\Pos\\Mot\\",
			"D:\\Data\\PR\\Pos\\TrainPositiveMotImageList.txt",
			"D:\\Data\\PR\\Neg\\Mot\\",
			"D:\\Data\\PR\\Neg\\TrainNegativeMotImageList.txt",
			"D:\\Data\\PR\\Hard\\Mot\\",
			"D:\\Data\\PR\\Hard\\TrainHardMotImageList.txt",
			"D:\\Data\\PR\\matrix.txt",
			"D:\\Data\\PR\\hog_svm.xml");

	}else if ( c == 4 ) {
		cout<<"Start Generate Train Image."<<endl;

		GenerateTrainImage myGTI;
		myGTI.generateTrainImage();

	}else if ( c == 5 )
	{
		cout<<"Here use to test crop image."<<endl;
		ResizeImage myresizeimage;
		myresizeimage.resizeImage("D:\\Data\\PR\\Pos\\Car\\","o_000002.jpg",128,128);
	}
	else if ( c == 6 )
	{
		cout<<"Here use to train neg images."<<endl;
		TrainNegImg myneg;
		myneg.trainNeg(128,128,"D:\\Data\\PR\\","D:\\Data\\PR\\Neg\\TrainNegativeCarImageList.txt","D:\\Data\\PR\\Neg\\Car\\","D:\\Data\\PR\\Neg\\Car\\namelist.txt");
	}
	else if ( c == 7 )
	{
		cout<<"Start image detect."<<endl;
		DetectObj mydetect;

		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\Mot.jpg",48,144);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\Mot2.jpg",48,144);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\Mot3.jpg",48,144);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\Mot4.jpg",48,144);
		/*
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test1.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test2.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test3.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test4.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test5.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test6.jpg",128,128);
		mydetect.startImgDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\test7.jpg",128,128);
		*/
		
	}
	else if ( c == 8 )
	{
		cout<<"Start video detect."<<endl;
		DetectObj mydetect;
		mydetect.startVideoDetect("data\\hog_svm_car.xml", "data\\00030003_xvid.avi","data\\test_end.avi",128,128);
		//mydetect.startVideoDetect("data\\hog_svm.xml", "data\\00030003_xvid.avi","data\\test_end.avi",48,144);
		//mydetect.startVideoDetect("D:\\Data\\PR\\hog_svm.xml", "D:\\Data\\PR\\TrainVideo\\Vehicle\\11291051_0031.avi","D:\\Data\\PR\\test_end.avi",128,128);
	}
	else if ( c == 9 )
	{
		ResizeImage myresizeimage;
		ofstream fin_carimagesname("D:\\Data\\PR\\Pos\\TrainPositiveCarImageList.txt");
		//ofstream fin_carimagesname("D:\\Data\\PR\\Neg\\TrainNegativeCarImageList.txt");
		for(int i=1;;i++) {
			char filename[256], sname[256];
			sprintf(filename,"o_%06d.jpg",i);
			myresizeimage.resizeImage("D:\\Data\\PR\\Pos\\Car\\",filename, 128, 128);
			sprintf(sname,"c_o_%06d.jpg",i);
			//sprintf(filename,"noperson%06d.jpg",i);
			//myresizeimage.resizeImage("D:\\Data\\PR\\Neg\\Car\\",filename,128,128);
			//sprintf(sname,"c_noperson%06d.jpg",i);
			fin_carimagesname<<sname<<endl;
		}
	}
	else if ( c == 10)
	{
		cout<<"Start test color shape detect."<<endl;
		ShapeDetect myshape;

		IplImage *srcimage=cvLoadImage("m9.jpg");
		IplImage *pHisEquaImage = myshape.EqualizeHistColorImage(srcimage);
		Mat src = pHisEquaImage;
		Mat dst = myshape.DetectCarPlateOrLight(src,1);
		int result = myshape.DetectShape(dst);
		cout<<"Result"<<result<<endl;
			
	}
	else if ( c == 11)
	{
		cout<<"Start Convert Car_Neg Image to Mot_Neg Image."<<endl;
		GenerateTrainImage myGTI;
		myGTI.splitNegImage();
	}
	else{
	}
}