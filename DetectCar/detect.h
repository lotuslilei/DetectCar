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

class DetectObj{

private:
	vector<float> setSVM( string SVMXMLFile );
	Mat imgDetect(string SVMXMLFile,  Mat inimage, int detect_width, int detect_heigh);
	Mat imgDetectHOF( string SVMXMLFile, Mat inimage, Mat outimage, int detect_width, int detect_heigh);

public:
	int startImgDetect( string SVMXMLFile, string imgFile, int detect_width, int detect_heigh );
	int startVideoDetect( string SVMXMLFile, string inputVideoFile, string outputVideoFile, int detect_width, int detect_heigh);
	int startVideoDetectHOF( string SVMXMLFile_X, string SVMXMLFile_Y, string inputVideoFile, string outputVideoFile, int detect_width, int detect_heigh );
};