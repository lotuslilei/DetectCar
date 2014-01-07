/************************************************************************/
/* 

here use to generate images from video used in training.

*/
/************************************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h> //srand() rand()
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h> //time() 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

class GenerateImage{
private:
	VideoCapture useVideo;
	Mat useframe;
	long totalFrameNumber; 

public:
	GenerateImage();

	//use to store all
	int VideoToImgStore( string inputVideo, string outputImgPos, string outputImgNameListPos, string outputImgNameListName );
	
	//use to show one by one
	int setVideo( string inputVideo );
	CvSize getVideoSize();
	Mat getImg( long frame );
	bool hasImg( long frame );
};

