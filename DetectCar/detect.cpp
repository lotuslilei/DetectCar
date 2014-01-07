#include "detect.h"
#include "hof.h"
#include "ShapeDetect.h"

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

vector<float> DetectObj::setSVM( string SVMXMLFile ){

	MySVM svm;

	char filename[256];
	strcpy(filename,SVMXMLFile.c_str());
	svm.load(filename);

	int DescriptorDim = svm.get_var_count();				//the number of eigenvectors.
	int supportVectorNum = svm.get_support_vector_count();	//the number of support vectors.

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);

	//get data to supportVectorMat
	for ( int i=0; i<supportVectorNum; i++ )
	{
		const float *pSVData = svm.get_support_vector(i);
		for ( int j=0; j<DescriptorDim; j++ )
		{
			supportVectorMat.at<float>(i,j) = pSVData[j];
		}
	}

	//get data to alphaMat
	double *pAlphaData = svm.get_alpha_vector();
	for ( int i=0; i<supportVectorNum; i++ )
	{
		alphaMat.at<float>(0,i) = pAlphaData[i];
	}

	//calculate resultMat = -(alphaMat * supportVectorMat)
	resultMat = -1 * alphaMat *supportVectorMat;

	//result: detector
	vector<float> myDetector;
	for ( int i=0; i<DescriptorDim; i++ )
	{
		myDetector.push_back(resultMat.at<float>(0,i));
	}
	myDetector.push_back(svm.get_rho());

	return myDetector;
}

int countCrop = 19744;

Mat DetectObj::imgDetect( string SVMXMLFile, Mat inimage, int detect_width, int detect_heigh ){

	//input
	Mat img = inimage;

	//detect
	ShapeDetect myshape;
	HOGDescriptor hog(cv::Size(detect_width,detect_heigh), cv::Size(16,16), cv::Size(8,8), cv::Size(8,8), 9);
	//HOGDescriptor hog;
	hog.setSVMDetector( setSVM(SVMXMLFile) );
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	fflush(stdout);
	vector<Rect> found, found_filtered;
	double t = (double)getTickCount();
	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).

	hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
	t = (double)getTickCount() - t;
	printf("End! detection time = %gms\n", t*1000./cv::getTickFrequency());
	size_t i, j;
	for( i = 0; i < found.size(); i++ )
	{
		Rect r = found[i];
		for( j = 0; j < found.size(); j++ )
			if( j != i && (r & found[j]) == r)
				break;
		if( j == found.size() )
			found_filtered.push_back(r);
	}
	for( i = 0; i < found_filtered.size(); i++ )
	{
		Rect r = found_filtered[i];
		// the HOG detector returns slightly larger rectangles than the real objects.
		// so we slightly shrink the rectangles to get a nicer output.
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);

		//1: original
		//rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
		//2: remove up 
		cv::Size s = img.size();
		double center_x = r.x+r.width/2.0;
		double center_y = r.y+r.height/2.0;
		double tmp = center_y - s.height*center_x/(2.0*s.width);	// y - (h/2w)x = 0
		double tmp2 = center_y + s.height*center_x/(2.0*s.width) - s.height/2.0;	// y + (h/2w)x - h/2 = 0
		if(tmp >0 && tmp2>0){
			//rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
			
			//car use
			rectangle(img, r.tl(), r.br(), cv::Scalar(255,0,0), 3);

			//moto use
			if(!(r.x<0||r.y<0||r.x+r.width>=img.cols||r.y+r.height>=img.rows)){
				Mat simg = img(r);
				simg = myshape.DetectCarPlateOrLight(simg,1);
				int re = myshape.DetectShape(simg);
				if ( re == 1 )
				{
					rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
				}
			}
			
			//output text
			//(r.x, r.y), (r.x+r.width, r.y+r.height)

			//output picture
			if(!(r.x<0||r.y<0||r.x+r.width>=img.cols||r.y+r.height>=img.rows)){
				Rect myRect(r.x, r.y, r.width, r.height);
				Mat myCrop = img(myRect);
				char saveName[256];
				sprintf(saveName,"D:\\Data\\PR\\Neg\\Car\\Neg\\noperson%06d.jpg",countCrop++);
				imwrite(saveName, myCrop);
			}
		}


		
	}
	//imshow("N", img);
	//waitKey();
	return img;

}

Mat DetectObj::imgDetectHOF( string SVMXMLFile, Mat inimage, Mat outimage, int detect_width, int detect_heigh ){

	//input
	Mat img = inimage;

	//detect
	HOGDescriptor hog(cv::Size(detect_width,detect_heigh), cv::Size(16,16), cv::Size(8,8), cv::Size(8,8), 9);
	hog.setSVMDetector( setSVM(SVMXMLFile) );
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	fflush(stdout);
	vector<Rect> found, found_filtered;
	double t = (double)getTickCount();
	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
	t = (double)getTickCount() - t;
	printf("End! detection time = %gms\n", t*1000./cv::getTickFrequency());
	size_t i, j;
	for( i = 0; i < found.size(); i++ )
	{
		Rect r = found[i];
		for( j = 0; j < found.size(); j++ )
			if( j != i && (r & found[j]) == r)
				break;
		if( j == found.size() )
			found_filtered.push_back(r);
	}
	for( i = 0; i < found_filtered.size(); i++ )
	{
		Rect r = found_filtered[i];
		// the HOG detector returns slightly larger rectangles than the real objects.
		// so we slightly shrink the rectangles to get a nicer output.
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(outimage, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
	}

	return outimage;

}

int DetectObj::startImgDetect( string SVMXMLFile, string imgFile, int detect_width, int detect_heigh ){
	Mat inimage = imread(imgFile);
	Mat img = imgDetect(SVMXMLFile,inimage,detect_width,detect_heigh);
	imshow("image",img);
	int c = waitKey(0) & 255;
	return 0;
}

int DetectObj::startVideoDetect(string SVMXMLFile, string inputVideoFile, string outputVideoFile, int detect_width, int detect_heigh){
	//set input video
	VideoCapture myvideovapture;
	myvideovapture.open(inputVideoFile);
	CvSize oc;
	int framenumber;
	oc.width = myvideovapture.get(CV_CAP_PROP_FRAME_WIDTH);
	oc.height = myvideovapture.get(CV_CAP_PROP_FRAME_HEIGHT);
	framenumber = myvideovapture.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"Frames Total: "<<framenumber<<endl;
	//set output video
	VideoWriter myvideowriter;
	myvideowriter.open(outputVideoFile,CV_FOURCC('D','I','V','X'),60,oc,true);

	Mat img;
	for (int n=1;n<framenumber;n++)
	{
		//get image from inputvideo
		myvideovapture >> img;

		//handle
		cout<<"Frame:"<<n<<" Handling..."<<endl;
		img = imgDetect(SVMXMLFile,img,detect_width,detect_heigh);

		//write image to outputvideo
		myvideowriter.write ( img );
	}

	return 0;
}

int DetectObj::startVideoDetectHOF(string SVMXMLFile_X, string SVMXMLFile_Y, string inputVideoFile, string outputVideoFile, int detect_width, int detect_heigh){

	//set input video
	VideoCapture myvideovapture;
	myvideovapture.open(inputVideoFile);
	CvSize oc;
	int framenumber,infps;
	oc.width = myvideovapture.get(CV_CAP_PROP_FRAME_WIDTH);
	oc.height = myvideovapture.get(CV_CAP_PROP_FRAME_HEIGHT);
	framenumber = myvideovapture.get(CV_CAP_PROP_FRAME_COUNT);
	infps = myvideovapture.get(CV_CAP_PROP_FPS);
	cout<<"Frames Total: "<<framenumber<<endl;
	//set output video
	VideoWriter myvideowriter;
	myvideowriter.open(outputVideoFile,CV_FOURCC('D','I','V','X'),infps,oc,true);
	//initial HOF
	HOF myhof;

	Mat imgpre;
	Mat img;
	myvideovapture >> img;	//frame 1
	for (int n=2;n<framenumber;n++)
	{
		//get the imgpre from img
		imgpre = img;

		//get image from inputvideo
		myvideovapture >> img;

		//handle
		cout<<"Frame:"<<n-1<<","<<n<<" HOF Handling..."<<endl;
		Mat gimg,gimgpre;
		cvtColor(img,gimg,CV_BGR2GRAY);
		cvtColor(imgpre,gimgpre,CV_BGR2GRAY);

		myhof.opticalFlow(gimgpre,gimg);
		Mat vx = myhof.getVelX();
		Mat vy = myhof.getVelY();

		/*imshow("1.jpg",vx);
		imshow("2.jpg",vy);
		cvWaitKey(0);*/

		vx.convertTo(vx, CV_8UC3);
		vy.convertTo(vy, CV_8UC3);

		Mat vxhof = imgDetectHOF(SVMXMLFile_X,vx,img,detect_width,detect_heigh);
		Mat vyhof = imgDetectHOF(SVMXMLFile_Y,vy,img,detect_width,detect_heigh);

		//write image to outputvideo
		imshow("test.jpg",vxhof);
		cvWaitKey(0);
		//myvideowriter.write ( vxhof );

	}

	return 0;
}